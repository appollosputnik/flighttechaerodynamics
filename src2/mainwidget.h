#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLFunctions>
#include <QTimer>


class GeometryEngine;


/* Create a checker board texture */
#define checkImageWidth  64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName[6];


class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void LoadGLTextures( const char * name );
    void drawCube();
    void torus(int numc, int numt);
    void drawFuselage();
    void drawWing();
    void drawTail();
    void drawFin();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

    void makeCheckImage(void);
    void makeCube();

    void idleFunc();

public:
    void atmosphericData();
    void drawAirfoil();
    void ellipticMethod();
    void hyperbolicMethod();
    void unstructuredMethod();
    void DomainVertexMethod();
    void compressibleFlowsNSEquation();
    void Navier_Stokes_equn();
    void BoltzMannEqun();
    void diagonalFormAsDKS(); //discrete kinetic system of relaxation technique
    void relaxationTechniques(); //relaxation system
    void chapmanEnskogTypeCFDSolver();
    void MonteCarloSimulation();
    void compressible_euler_solver();
    void EulerSolverBoltzmanEquan();
    void ChapmanEnskogTypeEulerSolver();
    void diagonalFormRelaxationSystem();
    void EulerSolverDiscreteKineticSystem();
    void AdaptiveGrid();
private:
    float  pi;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Aircraft
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GLfloat *ctrlX, *ctrlY, *ctrlZ;
    GLfloat **wingX, **wingY, **wingZ;
    GLfloat **fuslgX, **fuslgY, **fuslgZ;
    GLfloat **tailX, **tailY, **tailZ;
    GLfloat **finX, **finY, **finZ;
    GLfloat angles[9];
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QMatrix4x4 projection;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    GLint widthImg, heightImg;
    GLint *indices;
    GLfloat *vertices, *colors;
    GLuint list, object, theTorus;
    GLfloat m_rot[3];
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    GeometryEngine *geometries;
    QOpenGLTexture *texture;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
