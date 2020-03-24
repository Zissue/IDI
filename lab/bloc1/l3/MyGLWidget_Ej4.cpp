
//#include <GL/glew.h>
#include "MyGLWidget.h"
#include "glm/gtc/matrix_transform.hpp"
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
  scl = 0.5;
  rte = 0.0;
  trans = glm::vec3(0., 0., 0.0);
  scale = glm::vec3(1., 1., 1.0);

  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor (0., 0., 0.0, 0.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::keyPressEvent (QKeyEvent *e) {
  makeCurrent ();           // fa actiu el nostre context d’OpenGL
  switch ( e->key() ) {
    case Qt::Key_S:
      scale.x += 0.05;
      scale.y += 0.05;
      //glUniform1f (varLoc, scl);
      break;
    case Qt::Key_D:
      scale.x -= 0.05;
      scale.y -= 0.05;
      //glUniform1f (varLoc, scl);
      break;

    case Qt::Key_Up:
      trans.y += 0.05;
      break;
    case Qt::Key_Down:
      trans.y -= 0.05;
      break;
    case Qt::Key_Left:
      trans.x -= 0.05;
      break;
    case Qt::Key_Right:
      trans.x += 0.05;
      break;

    default: e->ignore ();  // propagar al pare
  }
  update ();                // provoca que es torni a pintar l’escena
}

void MyGLWidget::modelTrans() {
  glm::mat4 TG(1.0);
  rte += M_PI/4;
  TG = glm::rotate(TG, rte, glm::vec3(0, 0, 1));
  TG = glm::scale(TG, scale);
  TG = glm::translate(TG, trans);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
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

  glUniform1f(varLoc,scl);
  modelTrans();

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);

  glm::vec3 Color[3];  // Tres vèrtexs amb X, Y i Z
  Color[0] = glm::vec4(1, 0, 1, 1);
  Color[1] = glm::vec4(1, 1, 0, 1);
  Color[2] = glm::vec4(0, 1, 1, 1);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);
  
  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color), Color, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas) 
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
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
  colorLoc = glGetAttribLocation (program->programId(), "vcolor");
  varLoc = glGetUniformLocation (program->programId(), "scale");
  transLoc = glGetUniformLocation (program->programId(), "TG");
}