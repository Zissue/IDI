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

  public slots:
    void FOVslider(int va);

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
    // mouseMoveEvent - Es crida quan es mou el ratoli
    void mouseMoveEvent(QMouseEvent *e);

  private:
    void creaBuffers();
    void creaTerra();
    void carregaShaders();
    void modelTransformH(glm::vec3 pos, glm::vec3 rotat, float alfa);
    void modelTransformT();
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
    GLuint VAO_model;
    GLuint VAO_Terra;
    
    float rte;
        
    // Program
    QOpenGLShaderProgram *program;
    
    // Viewport
    GLint ample, 
          alt;
    
    // Camera
    float FOV, FOVini;
    float raw;
    float zNear;
    float zFar;
    glm::vec3 obs, vrp, up;
    
    glm::vec3 sceneMin, sceneMax, sceneCenter;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    float radi, distancia;
    float escala;
    glm::vec3 centerM;

    bool ort;

    // Angles Euler
    float theta, psi, phi;

    // Pos ant mouse
    float xold, yold;
    float delta;

    int fovsliderQT;
    // Zoom
    float FOVoffset;

    // Internal vars
    float scale;
    glm::vec3 pos;
    Model model;
};
