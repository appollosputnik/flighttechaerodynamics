#ifndef CAIRFOILDESIGNERWINDOW_H
#define CAIRFOILDESIGNERWINDOW_H

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLFunctions>
#include <QTimer>
// C++ Implementation. To find the point of
// intersection of two lines
//#include <bits/stdc++.h>
using namespace std;

// This pair is used to store the X and Y
// coordinates of a point respectively
//#define pdd pair<double, double>
class CAirfoilDesigner : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    static bool fModify;
    explicit CAirfoilDesigner(QWidget *parent = 0);
    ~CAirfoilDesigner();
    //void displayPoint(pdd P);
    //pdd lineLineIntersection(pdd A, pdd B, pdd C, pdd D);
    int  determinantOfMatrix(int **matrix, int n);
    void subMatrix(int **mat, int **temp, int p, int q, int n);
    void make3D();
    void read3DDataNPlot();
    void _solverEulerCompressibleFlow();
    void draw_star_wing(int, float *xu, float *yu, float *zu, float *xl, float *yl, float *zl );
    void draw_port_wing(int, float *xu, float *yu, float *zu, float *xl, float *yl, float *zl );
    void drawFuselage(int n1, int n2, float **xr, float **yr, float **zr, float **xl, float **yl, float **zl);
    void set_ifDrawAnchors();
    void set_ifDrawKnots();
    void set_ifDrawHGrid();
    void set_ifDrawFuselage();
    void generateGridOverAirfoil(float n);
    void selectionSort(float arr[], int size);
    void set_anchors(int n, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
    void set_bezier_knots(int n, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    float _anchor1, _anchor2, _anchor3, _anchor4;
    float *PtsSurfX, *PtsSurfY, *PtsSurfZ, **PtsSolidX, **PtsSolidY, **PtsSolidZ;
    int Nsurf, Nj, Ni;
    float a[21], xPoly[21], yPoly[21], zPoly[21];
    //////////////////////////////////////////////////////////////////////////
    /// \brief ifDrawFuselage
    ///
    double x1, y1, z1, x2, y2, z2;
    int breakIteration;
    float density_altitude;
    float *diff_dens, *diff_press, *diff_temp, *diff_visc, *del_visc_kinematic;
    float ***_visc_ij, ***_visc_kinematic_ij, ***_dens_ij, ***_press_ij, R, ***_temp_ij, ***_velo_u_ij, ***_velo_v_ij,
    delX, delY, ***_energy_ij, altitude;
    static bool ifDrawFuselage;
    int _nOfPointsOnFuselage, _nOfSlicesOnFuselage;
    float **xRightFuselage, **yRightFuselage, **zRightFuselage;
    float **xLeftFuselage, **yLeftFuselage, **zLeftFuselage;
    static bool ifDrawAnchors;
    static bool ifDrawKnots;
    static bool ifDrawHGrid;
    static bool ifDrawPortWing, ifDrawStarboardWing;
    int nHGrid;
    float inter;
    float * xHyperbolic, *yHyperbolic, *chi, *eta;
    float _cx, _cy, _cz;
    QPointF old_pos, new_pos;
    float trans[3];
    float dx, dy;
    float zoomFactor;
    float xMin, xMax, yMin, yMax;
    int nAnchors, nKnots;
    float *_anchorXu, *_anchorYu, *_anchorZu, *_anchorXl, *_anchorYl, *_anchorZl;
    float *_knotsXu, *_knotsYu, *_knotsZu, *_knotsXl, *_knotsYl, *_knotsZl;
    float *_starboardXu, *_starboardYu, *_starboardZu, *_starboardXl,
    *_starboardYl, *_starboardZl;
    float *_portXu, *_portYu, *_portZu, *_portXl, *_portYl, *_portZl;
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
    GLint widthImg, heightImg;
    GLint *indices;
    GLfloat *vertices, *colors;
    GLuint list, object, theTorus;
    GLfloat m_rot[3];
    QBasicTimer timer;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
};
#endif // CAIRFOILDESIGNERWINDOW_H
