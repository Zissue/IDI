#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  rte = 0.0f;
  scale = 1.0f;

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffers();
  creaTerra();

  calcCapsaCont();
  calcCenterScene();
  calcRadiScene();

  // Camera
  initializeCamera();
}

void MyGLWidget::initializeCamera() {
    
  // Proj
  FOV = float(M_PI)/2.0f;
  raw = 1.0f;
  zNear = 0.4f;
  zFar = 3.0f;
  projectTransform();


  //View
  obs = glm::vec3(0,0,1);
  vrp = glm::vec3(0,0,0);
  up = glm::vec3(0,1,0);
  viewTransform();
}

void MyGLWidget::paintGL () 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per Homer 
  glBindVertexArray (VAO_Homer);
  modelTransformH();
  glDrawArrays(GL_TRIANGLES, 0, sizeof(GLfloat) * homer.faces().size() * 3 * 3);

  // Activem el VAO per terra
  glBindVertexArray (VAO_Terra);
  modelTransformT();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransformH () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, rte, glm::vec3(0,1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformT () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(1,1,1));
  //transform = glm::rotate(transform, -rte, glm::vec3(0,0,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective(FOV, raw, zNear, zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    glm::mat4 View = glm::lookAt(obs, vrp, up);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més petit
      rte += 0.65;
      break;
    }
    case Qt::Key_T: { // escalar a més petit
      rte -= 0.65;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaTerra() {
  
  glm::vec3 Vertices[6];
  Vertices[0] = glm::vec3(-2,-1,-2);
  Vertices[1] = glm::vec3(-2,-1,2);
  Vertices[2] = glm::vec3(2,-1,-2);
  Vertices[3] = glm::vec3(2,-1,-2);
  Vertices[4] = glm::vec3(-2,-1,2);
  Vertices[5] = glm::vec3(2,-1,2);
  
  glm::vec3 Color[6];
  Color[0] = glm::vec3(0,1,0);
  Color[1] = glm::vec3(0,0,1);
  Color[2] = glm::vec3(1,1,0);
  Color[3] = glm::vec3(1,1,0);
  Color[4] = glm::vec3(1,0,1);
  Color[5] = glm::vec3(1,0,0);
    
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(Vertices),
               Vertices, 
               GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(Color),
               Color, 
               GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{
  homer.load("./models/HomerProves.obj");

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(GLfloat) * homer.faces().size() * 3 * 3,
               homer.VBO_vertices(), 
               GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(GLfloat) * homer.faces().size() * 3 * 3,
               homer.VBO_matdiff(), 
               GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");

}

void MyGLWidget::calcCapsaCont() {
  float xmin, xmax, ymin, ymax, zmin, zmax;

  xmin = xmax = homer.vertices()[0];
  ymin = ymax = homer.vertices()[1];
  zmin = zmax = homer.vertices()[2];

  for (unsigned int i = 3; i < homer.vertices().size(); i += 3) {
    xmin = std::fmin(homer.vertices()[i+0], xmin);
    xmax = std::fmax(homer.vertices()[i+0], xmax);

    ymin = std::fmin(homer.vertices()[i+1], ymin);
    ymax = std::fmax(homer.vertices()[i+1], ymax);

    zmin = std::fmin(homer.vertices()[i+1], zmin);
    zmax = std::fmax(homer.vertices()[i+1], zmax);
  }

  sceneMax.y = ymax;
}

void MyGLWidget::calcCapsaScene() {
  sceneMax.x = 1.0;
  sceneMax.z = 1.0;

  sceneMin = glm::vec3(-1, -1, -1);
}

void MyGLWidget::calcRadiScene() {
  radi = distance(sceneMin, sceneMax)/2;
}

void MyGLWidget::calcCenterScene() {
  sceneCenter = (sceneMax + sceneMin) / glm::vec3(2.0, 2.0, 2.0);
}