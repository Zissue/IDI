#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void creaBuffers ();
    void creaTerra();
    void carregaShaders ();
    void modelTransformH ();
    void modelTransformT ();
    void projectTransform();
    void viewTransform();
    
    void initializeCamera();

    // Exercise 1
    void calcCapsaCont();
    void calcCapsaScene();
    void calcRadiScene();
    void calcCenterScene();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    
    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;
    GLuint terraLoc;
    
    // VAO 
    GLuint VAO_Homer;
    GLuint VAO_Terra;
    
    float rte;
        
    // Program
    QOpenGLShaderProgram *program;
    
    // Viewport
    GLint ample, 
          alt;
    
    // Camera
    float FOV;
    float raw;
    float zNear;
    float zFar;
    glm::vec3 obs, vrp, up;
    
    glm::vec3 sceneMin, sceneMax, sceneCenter;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    float radi, distancia;

    // Internal vars
    float scale;
    glm::vec3 pos;
    Model homer;
};

