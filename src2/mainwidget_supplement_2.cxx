#include "mainwidget_supplement_2.h"
#include <fstream>
#include <QVector2D>
#include <QVector3D>
#include "math.h"

using namespace  std;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

#include <QMouseEvent>

#include <math.h>

//INTERNATIONAL STANDARD WEATHER PREDICTOR
//AERODYNAMIC DESIGNER & FLOW PREDICTOR
//AERODYNAMIC POST PROCESSING ANALYZER
//LUNAR MISSION DESIGNER

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//Lunar Mission:
1.  Design Lunar Vehicle (Voyger)
2.  Design Path (Trajectory)
3.  Landing on Moon
4.  Rover for driving on Moon
5.  Finding water (probe) and row Trees
6.  Electronics Associated.
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*

Aerofoil Geoometry
   a. Draw a symmetric aerofoil with le(0.0) and te(100.0)
   b. maximum thickness position (30.0, 0.0)
   c. Bezier control points (0.0,0.0), (30.0, 15.0), (100.0, 0.0), (30.0, -15.0), (0.0, 0.0)
   d. x, y. B(t) = SIGMA(i=0, i=n). nCi. (1-t)^(n-i). t^i. Pi
   e. get aerofoilKnotList.
   f. get dy/dx aerofoilKnotList
   g. get d^2y/dx^2 aerofoilKnotList
   h. camber=> maximum camber(30.0, 0.0) = 0.05C
   i. Leading edge radius/curvature <= d^2y/dx^2    write option to modify
   j. Trailing edge cusp (Kutaa condition) => dy/dx write option to modify

   //Algoritm...
   float xLE = 0.0; yLE = 0.0; xTE = 100.0; yTE = 0.0;
   float maxThicknessPosX = 30.0, maxThicknessPosY = 0.0;
   int nBzrCtrlPts = 5;
   float nezierCtrlPosX[5] = {0.0, 30.0, 100.0, 30.0, 0.0};
   float nezierCtrlPosY[5] = {0.0, 15.0, 0.0, -15.0, 0.0};
   int nKnots = 100;
   float ctrlPts_DyDx[4]; ctrlPts_D2yDx2[3];
   float camber = 5.0;
   float maxThickness = 30.0;
   float DyDxTECuspBezrDyDx[m]; //smoothen between first and last points at the Trailing Edge...

   */
MainWidgetSupplment_2::MainWidgetSupplment_2(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(10),
    indexBuf(QOpenGLBuffer::IndexBuffer)
{
    pi = 4*atan(1.0);
    m_rot[0] = 0.0;
    m_rot[1] = 0.0;
    m_rot[2] = 0.0;

    //timerEvent(0);
    //update();
}

MainWidgetSupplment_2::~MainWidgetSupplment_2()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    //makeCurrent();
    //delete texture;
    //delete geometries;
    //arrayBuf.destroy();
    //indexBuf.destroy();
    //doneCurrent();
}

void MainWidgetSupplment_2::makeCube()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.

    /*
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort)); */
//! [1]
//!
}

void MainWidgetSupplment_2::makeCheckImage(void)
{
    int i, j, c;

    for(i=0; i<checkImageHeight; i++) {
        for(j=0; j<checkImageWidth; j++)
        {
            c = ((((i&0x8==0)^((j&0x8)))==0))*255;
            //checkImage[i][j][0] = (GLubyte) c;
            //checkImage[i][j][1] = (GLubyte) c;
            //checkImage[i][j][2] = (GLubyte) c;
            //checkImage[i][j][3] = (GLubyte) 255;
        }
    }
}

//! [0]
void MainWidgetSupplment_2::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    //drawWing();
}

void MainWidgetSupplment_2::idleFunc()
{
    m_rot[0] += 2;
    m_rot[1] -= 2;
}

void MainWidgetSupplment_2::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed = acc;
}

void MainWidgetSupplment_2::timerEvent(QTimerEvent *)
{

//    Decrease angular speed (friction)
      //angularSpeed *= 0.99;
      //angularSpeed = 3.99;
/*
    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        // Request an update
        update();
    }

*/
      // Update rotation
      //rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
      // Request an update
      //update();

    //m_rot[0] =  15.50f;
    //m_rot[1] = -15.50f;
    #define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
    clamp(m_rot[0]);
    clamp(m_rot[1]);
    update();

//! [1]
}

void MainWidgetSupplment_2::LoadGLTextures( const char * name )
{
   // QImage img;

    //if(!img.load("checker.png")){
        //std::cerr << "ERROR in loading image" << std::endl;
    //}

    //QImage t = QOpenGLWidget::convertToGLFormat(img);
    //int wInter = t.width();
    //int hInter = t.height();
    int n = 0;
    //glGenTextures(6, texName_2);

    for(int j=0; j<2; j++)
    {
        for(int i=0; i<3; i++)
        {
            n = i*j+i+j;
            //glBindTexture(GL_TEXTURE_2D, texName_2[n]);
            //glTexImage2D(GL_TEXTURE_2D, 0, 3, wInter, hInter, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
            //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            //glBindTexture( GL_TEXTURE_2D, 0 );

       }
        n++;
    }


}


void MainWidgetSupplment_2::initializeGL()
{
    //initializeOpenGLFunctions();
    // Generate 2 VBOs
    //arrayBuf.create();
    //indexBuf.create();

    //drawCube();

    //glClearColor(0.0, 0.0, 0.0, 1.0);

//! [2]
    // Enable depth buffer
    //glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);


    //LoadGLTextures("cube.png");

/*  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName_2);
    glBindTexture(GL_TEXTURE_2D, texName_2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage); */


    //makeCube();

    //timer.start(12, this);
    //timerEvent(nullptr);
    //update();
    //idleFunc();
}

//! [3]
void MainWidgetSupplment_2::initShaders()
{
    // Compile vertex shader
    //if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        //close();

    // Compile fragment shader
    //if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        //close();

    // Link shader pipeline
    //if (!program.link())
        //close();

    // Bind shader pipeline for use
    //if (!program.bind())
        //close();
}
//! [3]

//! [4]
void MainWidgetSupplment_2::initTextures()
{
    // Load cube.png image
    //texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    //texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    //texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    //texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidgetSupplment_2::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    /* GLdouble aspect = qreal(w) / qreal(h ? h : 1);
    glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    //glOrtho(-5.0, 5.0, 5.0, -5.0, -5.0, 5.0);
    gluPerspective(60.0, aspect, 1.0, 100.0);
    gluLookAt(0, 0, -10.0, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); */

    // Calculate aspect ratio
/*    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
*/
//timerEvent(nullptr);
    //update();
}

void MainWidgetSupplment_2::paintGL()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_TEXTURE_2D);
   // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //glBindTexture(GL_TEXTURE_2D, *texName_2);

    //texture->bind();


    // Calculate model view transformation
    //QMatrix4x4 matrix;
    //matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(m_rot[0], 1.0, 0.0, 0.0);
    //matrix.rotate(m_rot[0], 0.0, 1.0, 0.0);
    //matrix.rotate(rotation);

    // Set model view-projection matrix
    //program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    //program.setUniformValue("texture", 0);

/*
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);

    glEnd();
*/
    //glTranslatef(0.0f, 0.0f, 10.0f);
    /*glRotatef(m_rot[0], 1.0, 0.0, 0.0);
    glRotatef(m_rot[1], 0.0, 1.0, 0.0);
    object = glGenLists(1);
    glNewList(object, GL_COMPILE);*/

    //front view
    //glBindTexture(GL_TEXTURE_2D, texName_2[0]);
    /* glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.0f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(3.0, 3.0, -3.0);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-3.0, 3.0, -3.0);
    glEnd(); */

    //right view
    //glBindTexture(GL_TEXTURE_2D, texName_2[1]);
    /* glBegin(GL_QUADS);
    glColor3f(0.0, 0.5, 0.0);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.33f, 1.0f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0, 3.0, -3.0);
    glEnd(); */


    //rear view
    //glBindTexture(GL_TEXTURE_2D, texName_2[2]);
    /*
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.0, 0.0);
    glTexCoord2f(0.33f, 0.0f); glVertex3f(-3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.0f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(-3.0, 3.0, 3.0);
    glEnd();


    //left view
    //glBindTexture(GL_TEXTURE_2D, texName_2[3]);
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glTexCoord2f(.33, .5); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(.33, 1.0f); glVertex3f(-3.0, -3.0, 3.0);
    glTexCoord2f(.66f, 1.0f); glVertex3f(-3.0, 3.0, 3.0);
    glTexCoord2f(.66f, .5f); glVertex3f(-3.0, 3.0, -3.0);
    glEnd();

    //bottom view
    //glBindTexture(GL_TEXTURE_2D, texName_2[4]);
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.66f, 0.0f); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(-3.0, -3.0, 3.0);
    glEnd();

    //top view
    //glBindTexture(GL_TEXTURE_2D, texName_2[5]);
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.5);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(-3.0, 3.0, -3.0);
    glTexCoord2f(0.66f, 1.0f); glVertex3f(3.0, 3.0, -3.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-3.0, 3.0, 3.0);
    glEnd();
    glEndList();
    glCallList(object);
    glFlush();
    glDisable(GL_TEXTURE_2D); */
    //rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    //update();
}
