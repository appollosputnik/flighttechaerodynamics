#ifndef MAINWIDGET_SUPPLEMENT_3_H
#define MAINWIDGET_SUPPLEMENT_3_H
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
#include <GL/gl.h>
#include <GL/glu.h>

class GeometryEngine;


/* Create a checker board texture */
#define checkImageWidth  64
#define checkImageHeight 64
static GLuint texName_3[6];


class MainWidgetSupplment_3 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidgetSupplment_3(QWidget *parent = 0);
    ~MainWidgetSupplment_3();
    void LoadGLTextures( const char * name );
    void drawCube();
    void torus(int numc, int numt);
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
private:
    float pi;
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
#endif // MAINWIDGET_SUPPLEMENT_3_H
