#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    virtual void keyPressEvent (QKeyEvent *e);

    virtual void modelTrans1();
    virtual void modelTrans2();

  private:
    void creaBuffers ();
    void carregaShaders ();
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint varLoc;      // scale del VS
    GLuint transLoc;    // TG del VS

    float scl;
    float rte1, rte2;
    glm::vec3 trans1, trans2;
    glm::vec3 scale1, scale2;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1;
    GLuint VAO3;
    GLint ample, alt;
};
