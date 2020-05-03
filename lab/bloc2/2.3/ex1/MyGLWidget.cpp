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
  ort = false;

  theta = 0.0f;
  psi = 0.0f;
  phi = 0.0f;

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffers();
  creaTerra();

  calcCapsaCont();
  calcCapsaScene();
  calcCenterScene();
  calcRadiScene();

  // Camera
  initializeCamera();
}

void MyGLWidget::initializeCamera() {
  
  // k > 1
  float k = 2.0;
  // Distancia qualsevol sempre i guan d > r * k
  distancia = radi * k;
  
  // Proj
  FOVini = 2.0 * asin(radi/distancia);
  FOV = FOVini;
  raw = 1.0f;
  zNear = distancia - radi;
  zFar = distancia + radi;
  projectTransform();

  //View
  vrp = sceneCenter;
  obs = vrp + glm::vec3(0,0,distancia);
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

  // Activem el VAO per el model 
  glBindVertexArray (VAO_model);
  modelTransformH();
  glDrawArrays(GL_TRIANGLES, 0, sizeof(GLfloat) * model.faces().size() * 3);

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
  transform = glm::scale(transform, glm::vec3(escala));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, rte, glm::vec3(0,1,0));
  transform = glm::translate(transform, -centerM);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformT () 
{
  //transform = glm::scale(transform, glm::vec3(1,1,1));
  //transform = glm::rotate(transform, -rte, glm::vec3(0,0,0));

  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
  glm::mat4 Proj = glm::perspective(FOV, raw, zNear, zFar);
  if (ort) Proj = glm::ortho(-2.5f*raw, 2.5f*raw, 0.0f, 4.0f, zNear, zFar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
  glm::mat4 View(1.0f);
  View = glm::translate(View, -glm::vec3(0, 0, distancia));
  // View = glm::rotate(View, -phi, glm::vec3(0, 0, 1)); // Opcional
  View = glm::rotate(View, theta, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -psi, glm::vec3(0, 1, 0));
  View = glm::translate(View, -vrp);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
  float rav = float(w) / float(h);
  raw = rav;

  // if (raw < 1.0) FOV = 2.0 * atan(tan(FOVini/2.0)/raw);
  // else FOV = FOVini;

  FOV = 2.0 * atan(tan(FOV/2.0)/raw);
  projectTransform();
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
    case Qt::Key_R: { 
      rte += (float)M_PI/4;
      break;
    }
    case Qt::Key_T: { 
      rte -= (float)M_PI/4;
      break;
    }
    case Qt::Key_O: { 
      ort = !ort;
      projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaTerra() {
  
  glm::vec3 Vertices[6] = {
    glm::vec3(-2.5, 0.0, 2.5),
    glm::vec3(-2.5, 0.0,-2.5),
    glm::vec3( 2.5, 0.0, 2.5),
    glm::vec3( 2.5, 0.0, 2.5),
    glm::vec3(-2.5, 0.0,-2.5),
    glm::vec3( 2.5, 0.0,-2.5)
  };
  
  glm::vec3 Color[6] = {
    glm::vec3(0,1,0), 
    glm::vec3(0,0,1),
    glm::vec3(1,1,0),
    glm::vec3(1,1,0),
    glm::vec3(1,0,1),
    glm::vec3(1,0,0)
  };
    
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
  model.load("./models/Patricio.obj");

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_model);
  glBindVertexArray(VAO_model);

  GLuint VBO_model[2];
  glGenBuffers(2, VBO_model);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_model[0]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(GLfloat) * model.faces().size() * 3 * 3,
               model.VBO_vertices(), 
               GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_model[1]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(GLfloat) * model.faces().size() * 3 * 3,
               model.VBO_matdiff(), 
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
  xmin = xmax = model.vertices()[0];
  ymin = ymax = model.vertices()[1];
  zmin = zmax = model.vertices()[2];

  for (unsigned int i = 3; i < model.vertices().size(); i += 3) {
    xmin = std::fmin(model.vertices()[i+0], xmin);
    xmax = std::fmax(model.vertices()[i+0], xmax);

    ymin = std::fmin(model.vertices()[i+1], ymin);
    ymax = std::fmax(model.vertices()[i+1], ymax);

    zmin = std::fmin(model.vertices()[i+2], zmin);
    zmax = std::fmax(model.vertices()[i+2], zmax);
  }
  // sceneMax = glm::vec3(xmax, ymax, zmax);
  // sceneMin = glm::vec3(xmin, ymin, zmin);

  escala = 4.0/(xmax-xmin);
  centerM = glm::vec3((xmin+xmax)/2, ymin, (zmin+zmax)/2);

  // sceneMax.y = 4.0;
}

void MyGLWidget::calcCapsaScene() {
  sceneMin = glm::vec3(-2.5, 0, -2.5);
  sceneMax = glm::vec3(2.5, 4, 2.5);
}

void MyGLWidget::calcRadiScene() {
  float dx, dy, dz;
  dx = xmax - xmin;
  dy = ymax - ymin;
  dz = zmax - zmin;

  radi = sqrt(dx*dx+dy*dy+dz*dz)/2.0;
  //radi = distance(sceneMin, sceneMax)/2;
}

void MyGLWidget::calcCenterScene() {  
  // sceneCenter = sceneMax + sceneMin;
  // sceneCenter = sceneCenter / glm::vec3(2.0);

  sceneCenter.x = (sceneMax.x + sceneMin.x)/2.0;
  sceneCenter.y = (sceneMax.y + sceneMin.y)/2.0;
  sceneCenter.z = (sceneMax.z + sceneMin.z)/2.0;
}