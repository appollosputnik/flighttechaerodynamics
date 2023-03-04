#include "CAirfoilDesignerWindow.h"
#include <fstream>
#include <QVector2D>
#include <QVector3D>
#include "math.h"
#include <bits/stdc++.h>
using namespace std;
#define N 3

typedef enum
{
    PAN = 0,
    ROTATE,
    NOMOVE
} STATE;

float pi = 4*atan(1.0);

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

bool CAirfoilDesigner::ifDrawAnchors = false;
bool CAirfoilDesigner::ifDrawKnots = false;
bool CAirfoilDesigner::ifDrawHGrid = false;
bool CAirfoilDesigner::ifDrawFuselage = false;
bool ifSolver = false;
bool bConverged = false;
bool if3D = false;
bool CAirfoilDesigner::ifDrawStarboardWing = false;
bool CAirfoilDesigner::ifDrawPortWing = false;
ofstream out("test.txt");
STATE state;

float *newarray;
float ret = 0.0;

CAirfoilDesigner::CAirfoilDesigner(QWidget *parent) :
    QGLWidget(parent),
    xMin(-5.0),
    xMax(5.0),
    yMin(-5.0),
    yMax(5.0)
{
    state = NOMOVE;
    _cx = 0.0;
    _cy = 0.0;
    _cz = 0.0;
    trans[0] = 0.0;
    trans[1] = 0.0;
    trans[2] = 0.0;
    m_rot[0] = 0.0;
    m_rot[1] = 0.0;
    m_rot[2] = 0.0;
    zoomFactor = 1.0;
    nHGrid = 11;
}

CAirfoilDesigner::~CAirfoilDesigner()
{
}

void CAirfoilDesigner::make3D()
{
    if3D = true;
    ifDrawAnchors = false;
    updateGL();
}

void CAirfoilDesigner::set_ifDrawFuselage()
{
    if(ifDrawFuselage==true)
        ifDrawFuselage = false;
    else {
        ifDrawFuselage = true;
    }
    updateGL();
}

void CAirfoilDesigner::set_ifDrawAnchors()
{
    if(ifDrawAnchors==true)
        ifDrawAnchors = false;
    else {
        ifDrawAnchors = true;
    }
    updateGL();
}

void CAirfoilDesigner::set_ifDrawKnots()
{
    if(ifDrawKnots==true)
        ifDrawKnots = false;
    else {
        ifDrawKnots = true;
    }
    updateGL();
}

void CAirfoilDesigner::set_ifDrawHGrid()
{
    if(ifDrawHGrid==true)
        ifDrawHGrid = false;
    else {
        ifDrawHGrid = true;
    }
    updateGL();
}


void CAirfoilDesigner::generateGridOverAirfoil(float n)
{
        nHGrid = n;
        float *chi = new float[nHGrid], *eta = new float[nHGrid];

        inter = 1.0 / (nHGrid-1);

        out << "chi " << " eta " << " x " << " y " << "\n";

        chi[0] = 0.0, eta[0] = 0.0;

        for (int i = 1; i < nHGrid; i++)
        {
            chi[i] = chi[i - 1] + inter;
            eta[i] = eta[i - 1] + inter;
        }

        xHyperbolic = new float[nHGrid], yHyperbolic = new float[nHGrid];

        out << nHGrid << "\n";
        xHyperbolic[0] = 0.0, yHyperbolic[0] = 0.0;
        out << chi[0] << " " << eta[0] << " " << xHyperbolic[0] << " " << yHyperbolic[0] << "\n";
        for (int i = 1; i < nHGrid; i++)
        {
            yHyperbolic[i] = yHyperbolic[i - 1] + sqrt(1 + pow(((eta[i] - eta[i - 1]) / (chi[i] - chi[i - 1])), 2));
            xHyperbolic[i] = (2 * xHyperbolic[i - 1] + sqrt(2 * xHyperbolic[i - 1] * 2 * xHyperbolic[i - 1] - 4 * ((xHyperbolic[i - 1] * xHyperbolic[i - 1]) - (yHyperbolic[i] - yHyperbolic[i - 1]) * (yHyperbolic[i] - yHyperbolic[i - 1]))))/2;          
            out << chi[i] << " " << eta[i] << " " << xHyperbolic[i] << " " << yHyperbolic[i] << "\n";
        }

        //Alignment of grid with wall...
        for(int i=0; i<nHGrid; i++)
        {
            yHyperbolic[i]  = (yHyperbolic[i] / (yHyperbolic[nHGrid-1])) * 100.0;
            xHyperbolic[i]  = (xHyperbolic[i] / (xHyperbolic[nHGrid-1])) * 100.0;
            out << "\n" << xHyperbolic[i] << " " << yHyperbolic[i] << "\n";
        }


        ifDrawHGrid = true;

        float *x = new float[nHGrid];
        float *y = new float[nHGrid];

        for(int i=0; i<nHGrid; i++) {
            x[i]   = xHyperbolic[i];
            y[i]   = yHyperbolic[i];
        }

        selectionSort(x, nHGrid);
        selectionSort(y, nHGrid);



        xMin = x[0]; xMax = x[nHGrid-1];
        yMin = y[0]; yMax = y[nHGrid-1];

        float width = xMax - xMin;
        float height = yMax - yMin;

        /* for(int i=0; i<nHGrid; i++) {
            xHyperbolic[i] = xHyperbolic[i]/width;
            yHyperbolic[i] = yHyperbolic[i]/height;
            yHyperbolic[i] += 0.5;
        } */

        delete [] x; x = nullptr;
        delete [] y; y = nullptr;

        x = new float[nHGrid];
        y = new float[nHGrid];

        for(int i=0; i<nHGrid; i++) {
            x[i] = xHyperbolic[i];
            y[i] = yHyperbolic[i];
        }

        for( int i=0; i<nHGrid; i++)
        {
            yHyperbolic[i] -= y[nHGrid/2];
        }

        ///////////////////////////////////////////////////////////////////////

        //Discard body points/cells...
        //11 X 11 determinant...
        //_knotsXu where number of knots on upper & lower surfaces = 11...
        //_knotsXu[11], _knotsXl[11]
        //_knotsXu[0], ----, _knotsXu[10], _knotsXl[9], ------, _knotsXl[0] => 21 points

        //make the polynomial for 21 points...21 uknowns...20 order
        //a20.x^20+a19.x^19+a18.x^18+ ----- + a2.x^2+a1.x+a0=0 //21 unknowns
        //a20.y^20+a19.y^19+a18.y^18+ ----- + a2.y^2+a1.y+a0=0 //21 unknowns
        //a20.z^20+a19.z^19+a18.z^18+ ----- + a2.z^2+a1.z+a0=0 //21 unknowns

        //b20.x^20+b19.x^19+b18.x^18+ ----- + b2.x^2+b1.x+b0=0 //21 unknowns
        //b20.y^20+b19.y^19+b18.y^18+ ----- + b2.y^2+b1.y+b0=0 //21 unknowns
        //b20.z^20+b19.z^19+b18.z^18+ ----- + b2.z^2+b1.z+b0=0 //21 unknowns

        //?? how to solve this 21 unknowns ? This is a determinant
        //a20 x1^19 x1^18 x1^17    ...... x1^2 x a0
        //a20 x2^19 x2^18 x2^17    ...... x2^2 x a0
        //a20 x3^19 x3^18 x3^17    ...... x3^2 x a0
        //--------------------------------------
        //a20 x21^19 x21^18 x21^17 ....... x3^2 x a0

        ////////////////////////////////////////////////////////////////////////

        for(int i=0; i<nKnots; i++) {

            xPoly[i] = _knotsXu[i];
            yPoly[i] = _knotsYu[i];
            zPoly[i] = _knotsZu[i];
        }

        for(int i=0; i<(nKnots-1); i++) {

            xPoly[nKnots+i] = _knotsXl[nKnots-2-i];
            yPoly[nKnots+i] = _knotsYl[nKnots-2-i];
            zPoly[nKnots+i] = _knotsZl[nKnots-2-i];
        }

        //Arranging the determinant....

        ///////////////////////////////////////////////////////////////////

        //Another method of intersecting lines...

        selectionSort(x, nHGrid);
        selectionSort(y, nHGrid);

        xMin = x[0]; xMax = x[nHGrid-1];
        yMin = y[0]; yMax = y[nHGrid-1];

        _cx = xMin + ((xMax - xMin)/2.0);
        _cy = yMin + ((yMax - yMin)/2.0);
        _cz = 0.0;

        out.close();
        updateGL();
}

void CAirfoilDesigner::subMatrix(int **mat, int **temp, int p, int q, int n) {
    mat = new int*[N];
    for(int i=0; i<N; i++)
        mat[i] = new int[N];

    temp = new int*[N];
    for(int i=0; i<N; i++)
        temp[i] = new int[N];

   int i = 0, j = 0;
   // filling the sub matrix
   for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
         // skipping if the current row or column is not equal to the current
         // element row and column
         if (row != p && col != q) {
            temp[i][j++] = mat[row][col];
            if (j == n - 1) {
               j = 0;
               i++;
            }
         }
      }
   }
}
int CAirfoilDesigner::determinantOfMatrix(int **matrix, int n) {
    matrix = new int*[N];
    for(int i=0; i<N; i++)
        matrix[i] = new int[N];

   int determinant = 0;
   if (n == 1) {
      return matrix[0][0];
   }
   if (n == 2) {
      return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
   }
   int **temp, sign = 1;
   temp = new int*[N];
   for(int i=0; i<N; i++)
       temp[i] = new int[N];
   for (int i = 0; i < n; i++) {
      subMatrix(matrix, temp, 0, i, n);
      determinant += sign * matrix[0][i] * determinantOfMatrix(temp, n - 1);
      sign = -sign;
   }
   return determinant;
}

void CAirfoilDesigner::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position

    //gluUnProject(old_pos.x(), old_pos.y(), 0.0f,0, 0, 0, &x1, &y1, &z1);

    old_pos = e->pos();

    if(e->button()==Qt::LeftButton)
    {
        state = PAN;
    }
    else if(e->button()==Qt::RightButton)
    {
        state = ROTATE;
    }
}

void CAirfoilDesigner::mouseReleaseEvent(QMouseEvent *e)
{
    state = NOMOVE;
}

void CAirfoilDesigner::mouseMoveEvent(QMouseEvent *e)
{
    new_pos = e->pos();
    //gluUnProject(new_pos.x(), new_pos.y(), 0.0f,0, 0, 0, &x2, &y2, &z2);

    dx = (new_pos.x() - old_pos.x());
    dy = (new_pos.y() - old_pos.y());

    if(state == PAN) //pan
    {
        trans[0] += dx;
        trans[1] += dy;
        old_pos = new_pos;
        updateGL();

    }
    else if(state == ROTATE) //rotate
    {
        m_rot[0] += dx;
        m_rot[1] += dy;
        #define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
        clamp(m_rot[0]);
        clamp(m_rot[1]);
        old_pos = new_pos;
        updateGL();

   }

    else
        QString str = "No operation at this point of time...";


}
void CAirfoilDesigner::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
    {
        zoomFactor /= 0.75;
    }
    else if(e->delta() <= 0)
    {
        zoomFactor *= 0.75;
    }

    updateGL();
}

void CAirfoilDesigner::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void CAirfoilDesigner::resizeGL(int w, int h)
{
    glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(xMin-100, xMax+100, yMax+100, yMin-100, -500.0, 500);
}


//5th GL Window...
//Drawing Airfoil & Wings, Fuselage, Tail, Fin and Grid with solver...

void CAirfoilDesigner::_solverEulerCompressibleFlow()
{
    //draw grid hyperbolic...
    if(ifSolver)
    {
        //Computing flow: Inviscid, compressible flows - Euler Solver...
        //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
        //time step= 0
        //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

        //Algorithms:
        for (int t = 0; t<10000; t++) {
         for(int j=0; j<nHGrid; j++) {
          for(int i=0; i<nHGrid; i++) {
        //predictor step
        density_altitude= _dens_ij[i][j][t];
        _press_ij[i][j][t] = _dens_ij[i][j][t] * R * _temp_ij[i][j][t];
        _velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-1][t]/delX) - (1/_dens_ij[i][j][t])-((_press_ij[i][j][t]) - (_press_ij[i-1][j][t]/delX)))/((_velo_u_ij[i][j][t]/delX) + (_velo_v_ij[i][j][t]/delY) - ((_velo_u_ij[i-1][j][t]/delX)));
        _velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-1][t]/delX) - (1/_dens_ij[i][j][t])-((_press_ij[i][j][t]) - (_press_ij[i-1][j][t]/delY)))/((_velo_u_ij[i][j][t]/delY) + (_velo_v_ij[i][j][t]/delX) - ((_velo_v_ij[i-1][j][t]/delY)));
        //corrector step
        _press_ij[i][j][t] = (_press_ij[i - 1][j][t] / delY) - _dens_ij[i][j][t] * (_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) + (_velo_u_ij[i][j][t] / delX) - (_velo_v_ij[i - 1][j][t] / delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j - 1][t] / delX));
        _dens_ij[i][j][t] = (_press_ij[i-1][j][t]/delY - _press_ij[i][j][t]) / (_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) - (_velo_v_ij[i-1][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-1][t]/delX);



        //DNS Turbulence model
        //_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) * (_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] * _velo_v_ij[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) + (_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) + (_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
        //k-e Turbulence Model
        //_energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t]) ) ;
        //k-w Turbulence Model
        //_energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t])/t);

        float Courant_no= 0.5 ; //~ 0.95 Conmvergeance criteria
        if (t == 1) {
        float diff_dens = _dens_ij[i][j][1] - _dens_ij[i][j][0];
        float diff_press = _press_ij[i][j][1] - _press_ij[i][j][0];
        float diff_temp = _temp_ij[i][j][1] - _temp_ij[i][j][0];
        float diff_visc = _visc_ij[i][j][1] - _visc_ij[i][j][0];
        float del_visc_kinematic = _visc_kinematic_ij[i][j][1] - _visc_kinematic_ij[i][j][0];

        float array_diff[5] = {diff_dens, diff_press, diff_temp, diff_visc, del_visc_kinematic };
        selectionSort(array_diff, 5);
        float leastValue = array_diff[0];
        if (leastValue <= 10e-6 * Courant_no) //Converged
        {

            bConverged = true; break;

            breakIteration = t;

        }
        else

        continue;

        }
          }
         }
        }

        ofstream out2("solver.txt");
        out2 << nHGrid << "\n";
        out2 << "Density " << " " << "Pressure " << " " << "Temperature " << " " << " Viscosity " << " Kinematic Viscosity \n";
        out2 << "\n\n";

        for(int t=0; t< breakIteration; t++)
        {
            for(int j=0; j<nHGrid; j++) {
             for(int i=0; i<nHGrid; i++) {

                 out2 << _dens_ij[i][j][t] << " " << _press_ij[i][j][t] << " " << _temp_ij[i][j][t] << " " << _visc_ij[i][j][t] << " " << _visc_kinematic_ij[i][j][t] << "\n";

             }
             }
             }

        out2.close();
        }
}

void CAirfoilDesigner::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glOrtho(xMin-100, xMax+100, yMax+100, yMin-100, -500.0, 500.0);



    glRotatef(m_rot[0], 1.0, 0.0, 0.0);
    glRotatef(m_rot[1], 0.0, 1.0, 0.0);
    glTranslatef(trans[0], trans[1], 0.0f);
    glTranslatef(_cx, _cy, _cz);
    glScalef(zoomFactor, zoomFactor, zoomFactor);
    glTranslatef(-_cx, -_cy, -_cz);


    if(ifDrawAnchors) {
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    for(int i=0; i<nAnchors; i++)
    {
        glVertex2f(_anchorXu[i], _anchorYu[i]);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    for(int i=0; i<nAnchors; i++)
    {
        glVertex2f(_anchorXl[i], _anchorYl[i]);
    }
    glEnd();

    }

    if(ifDrawKnots) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    for(int i=0; i<nKnots; i++) {
        glVertex3f(_knotsXu[i], _knotsYu[i], _knotsZu[i]);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    for(int i=0; i<nKnots; i++) {
        glVertex3f(_knotsXl[i], _knotsYl[i], _knotsZl[i]);
    }
    glEnd();
    }


    if(ifDrawStarboardWing)
    {
        for(int j=0; j< nKnots-1; j++)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(_knotsXu[j], _knotsYu[j], _knotsZu[j]);
            glVertex3f(_knotsXu[j+1], _knotsYu[j+1], _knotsZu[j+1]);
            glVertex3f(_starboardXu[j], _starboardYu[j], _starboardZu[j]);

            glBegin(GL_TRIANGLES);
            glVertex3f(_starboardXu[j], _starboardYu[j], _starboardZu[j]);
            glVertex3f(_starboardXu[j+1], _starboardYu[j+1], _starboardZu[j+1]);
            glVertex3f(_knotsXu[j+1], _knotsYu[j+1], _knotsZu[j+1]);
        }

        for(int j=0; j< nKnots-1; j++)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(_knotsXl[j], _knotsYl[j], _knotsZl[j]);
            glVertex3f(_knotsXl[j+1], _knotsYl[j+1], _knotsZl[j+1]);
            glVertex3f(_starboardXl[j], _starboardYl[j], _starboardZl[j]);

            glBegin(GL_TRIANGLES);
            glVertex3f(_starboardXl[j], _starboardYl[j], _starboardZl[j]);
            glVertex3f(_starboardXl[j+1], _starboardYl[j+1], _starboardZl[j+1]);
            glVertex3f(_knotsXl[j+1], _knotsYl[j+1], _knotsZl[j+1]);
        }

    }

    if(ifDrawPortWing)
    {
        for(int j=0; j< nKnots-1; j++)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(_knotsXu[j], _knotsYu[j], _knotsZu[j]);
            glVertex3f(_knotsXu[j+1], _knotsYu[j+1], _knotsZu[j+1]);
            glVertex3f(_portXu[j], _portYu[j], _portZu[j]);

            glBegin(GL_TRIANGLES);
            glVertex3f(_portXu[j], _portYu[j], _portZu[j]);
            glVertex3f(_portXu[j+1], _portYu[j+1], _portZu[j+1]);
            glVertex3f(_knotsXu[j+1], _knotsYu[j+1], _knotsZu[j+1]);
        }

        for(int j=0; j< nKnots-1; j++)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(_knotsXl[j], _knotsYl[j], _knotsZl[j]);
            glVertex3f(_knotsXl[j+1], _knotsYl[j+1], _knotsZl[j+1]);
            glVertex3f(_portXl[j], _portYl[j], _portZl[j]);

            glBegin(GL_TRIANGLES);
            glVertex3f(_portXl[j], _portYl[j], _portZl[j]);
            glVertex3f(_portXl[j+1], _portYl[j+1], _portZl[j+1]);
            glVertex3f(_knotsXl[j+1], _knotsYl[j+1], _knotsZl[j+1]);
        }
    }

    //draw Fuselage...
    if(ifDrawFuselage)
    {
        //for right surface
        for(int i=0; i<_nOfSlicesOnFuselage-1; i++)
        {
            for(int j=0; j<_nOfPointsOnFuselage; j++)
            {
                glBegin(GL_TRIANGLES);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(xRightFuselage[i][j], yRightFuselage[i][j], zRightFuselage[i][j]);
                glVertex3f(xRightFuselage[i][j+1], yRightFuselage[i][j+1], zRightFuselage[i][j+1]);
                glVertex3f(xRightFuselage[i+1][j], yRightFuselage[i+1][j], zRightFuselage[i+1][j]);

                glBegin(GL_TRIANGLES);
                glVertex3f(xRightFuselage[i+1][j], yRightFuselage[i+1][j], zRightFuselage[i+1][j]);
                glVertex3f(xRightFuselage[i+1][j+1], yRightFuselage[i+1][j+1], zRightFuselage[i+1][j+1]);
                glVertex3f(xRightFuselage[i][j+1], yRightFuselage[i][j+1], zRightFuselage[i][j+1]);


                glBegin(GL_TRIANGLES);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(xLeftFuselage[i][j], yLeftFuselage[i][j], zLeftFuselage[i][j]);
                glVertex3f(xLeftFuselage[i][j+1], yLeftFuselage[i][j+1], zLeftFuselage[i][j+1]);
                glVertex3f(xLeftFuselage[i+1][j], yLeftFuselage[i+1][j], zLeftFuselage[i+1][j]);

                glBegin(GL_TRIANGLES);
                glVertex3f(xLeftFuselage[i+1][j], yLeftFuselage[i+1][j], zLeftFuselage[i+1][j]);
                glVertex3f(xLeftFuselage[i+1][j+1], yLeftFuselage[i+1][j+1], zLeftFuselage[i+1][j+1]);
                glVertex3f(xLeftFuselage[i][j+1], yLeftFuselage[i][j+1], zLeftFuselage[i][j+1]);

           }
        }
    }

    //draw grid hyperbolic...
    if(ifDrawHGrid)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(12.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_POINTS);
            for(int i=0; i<nHGrid; i++) {
                float pointX, pointY;
                for(int k=0; k<(nKnots*2-1); k++) {
                    for(int j=0; j<nHGrid; j++) {
                        for(int i=0; i<nHGrid; i++) {
                            //m1 = y2-y1/x2-x1
                            float m1 = (yHyperbolic[i+1] - yHyperbolic[i])/(xHyperbolic[i+1]-xHyperbolic[i]);
                            float m2 = (yPoly[k+1] - yPoly[k])/(xPoly[k+1]-xPoly[k]);

                            if(m1*m2!=1)
                            {
                                pdd A = make_pair(xPoly[k+1], yPoly[k+1]);
                                pdd B = make_pair(xPoly[k], yPoly[k]);
                                pdd C = make_pair(xHyperbolic[i+1], yHyperbolic[i+1]);
                                pdd D = make_pair(xHyperbolic[i], yHyperbolic[i]);
                                pdd intersection = lineLineIntersection(A, B, C, D);
                                pointX = intersection.first; pointY = intersection.second;
                            }

                        }
                    }
                }

                if(pointX==xHyperbolic[i] && pointY==yHyperbolic[j]) {
                    xHyperbolic[i] = xPoly[i]; yHyperbolic[j] = yPoly[j];
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
                    continue;
                }
                else
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
            }
            glEnd();
        }


        glColor3f(0.0, 1.0, 0.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_LINE_STRIP);
            for(int i=0; i<nHGrid; i++) {
                float pointX, pointY;
                for(int k=0; k<(nKnots*2-1); k++) {
                    for(int j=0; j<nHGrid; j++) {
                        for(int i=0; i<nHGrid; i++) {
                            //m1 = y2-y1/x2-x1
                            float m1 = (yHyperbolic[i+1] - yHyperbolic[i])/(xHyperbolic[i+1]-xHyperbolic[i]);
                            float m2 = (yPoly[k+1] - yPoly[k])/(xPoly[k+1]-xPoly[k]);

                            if(m1*m2!=1)
                            {
                                pdd A = make_pair(xPoly[k+1], yPoly[k+1]);
                                pdd B = make_pair(xPoly[k], yPoly[k]);
                                pdd C = make_pair(xHyperbolic[i+1], yHyperbolic[i+1]);
                                pdd D = make_pair(xHyperbolic[i], yHyperbolic[i]);
                                pdd intersection = lineLineIntersection(A, B, C, D);
                                pointX = intersection.first; pointY = intersection.second;
                            }

                        }
                    }
                }

                if(pointX==xHyperbolic[i] && pointY==yHyperbolic[j]) {
                    xHyperbolic[i] = xPoly[i]; yHyperbolic[j] = yPoly[j];
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
                    continue;
                }
                else
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);

            }
            glEnd();
        }



        glColor3f(1.0, 0.0, 1.0);
        glPointSize(12.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_POINTS);
            for(int j=0; j<nHGrid; j++) {
                float pointX, pointY;
                for(int k=0; k<(nKnots*2-1); k++) {
                    for(int j=0; j<nHGrid; j++) {
                        for(int i=0; i<nHGrid; i++) {
                            //m1 = y2-y1/x2-x1
                            float m1 = (yHyperbolic[i+1] - yHyperbolic[i])/(xHyperbolic[i+1]-xHyperbolic[i]);
                            float m2 = (yPoly[k+1] - yPoly[k])/(xPoly[k+1]-xPoly[k]);

                            if(m1*m2!=1)
                            {
                                pdd A = make_pair(xPoly[k+1], yPoly[k+1]);
                                pdd B = make_pair(xPoly[k], yPoly[k]);
                                pdd C = make_pair(xHyperbolic[i+1], yHyperbolic[i+1]);
                                pdd D = make_pair(xHyperbolic[i], yHyperbolic[i]);
                                pdd intersection = lineLineIntersection(A, B, C, D);
                                pointX = intersection.first; pointY = intersection.second;
                            }

                        }
                    }
                }

                if(pointX==xHyperbolic[i] && pointY==yHyperbolic[j]) {
                    xHyperbolic[i] = xPoly[i]; yHyperbolic[j] = yPoly[j];
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
                    continue;
                }
                else
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
            }
            glEnd();
        }


        glColor3f(0.0, 0.0, 1.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<nHGrid; j++) {              
                float pointX, pointY;
                for(int k=0; k<(nKnots*2-1); k++) {
                    for(int j=0; j<nHGrid; j++) {
                        for(int i=0; i<nHGrid; i++) {
                            //m1 = y2-y1/x2-x1
                            float m1 = (yHyperbolic[i+1] - yHyperbolic[i])/(xHyperbolic[i+1]-xHyperbolic[i]);
                            float m2 = (yPoly[k+1] - yPoly[k])/(xPoly[k+1]-xPoly[k]);

                            if(m1*m2!=1)
                            {
                                pdd A = make_pair(xPoly[k+1], yPoly[k+1]);
                                pdd B = make_pair(xPoly[k], yPoly[k]);
                                pdd C = make_pair(xHyperbolic[i+1], yHyperbolic[i+1]);
                                pdd D = make_pair(xHyperbolic[i], yHyperbolic[i]);
                                pdd intersection = lineLineIntersection(A, B, C, D);
                                pointX = intersection.first; pointY = intersection.second;
                            }

                        }
                    }
                }

                if(pointX==xHyperbolic[i] && pointY==yHyperbolic[j]) {
                    xHyperbolic[i] = xPoly[i]; yHyperbolic[j] = yPoly[j];
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
                    continue;
                }
                else
                    glVertex2f(xHyperbolic[i], yHyperbolic[j]);
            }
            glEnd();
        }

    }

    if(if3D)
    {
        //Wings
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0, 1.0, 0.0);
        for(int i=0; i<nKnots; i++) {
            glVertex3f(_knotsXu[i]*.3, _knotsYu[i]*.3, 10.0f);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3f(0.0, 1.0, 0.0);
        for(int i=0; i<nKnots; i++) {
            glVertex3f(_knotsXl[i]*.3, _knotsYl[i]*.3, 10.0f);
        }
        glEnd();

        //Grid...
        for(int i=0; i<(nKnots-1); i++)
        {
            glBegin(GL_QUADS);
            glVertex3f(_knotsXu[i], _knotsYu[i], _knotsZu[i]);
            glVertex3f(_knotsXu[i+1], _knotsYu[i+1], _knotsZu[i+1]);
            glVertex3f(_knotsXu[i+1], _knotsYu[i+1], 10.0f);
            glVertex3f(_knotsXu[i], _knotsYu[i], 10.0f);
            glEnd();
        }

        for(int i=0; i<(nKnots-1); i++)
        {
            glBegin(GL_QUADS);
            glVertex3f(_knotsXl[i], _knotsYl[i], _knotsZl[i]);
            glVertex3f(_knotsXl[i+1], _knotsYl[i+1], _knotsZl[i+1]);
            glVertex3f(_knotsXl[i+1], _knotsYl[i+1], 10.0f);
            glVertex3f(_knotsXl[i], _knotsYl[i], 10.0f);
            glEnd();
        }


        //3D Grid...highlights nodes...
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(12.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_POINTS);
            for(int i=0; i<nHGrid; i++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 5.0f);
            }
            glEnd();
        }


        glColor3f(0.0, 1.0, 0.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_LINE_STRIP);
            for(int i=0; i<nHGrid; i++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 5.0f);
            }
            glEnd();
        }


        //node highlights
        glColor3f(1.0, 0.0, 1.0);
        glPointSize(12.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_POINTS);
            for(int j=0; j<nHGrid; j++) {

                glVertex3f(xHyperbolic[i], yHyperbolic[j], 5.0f);
            }
            glEnd();
        }

        //grid...
        glColor3f(0.0, 0.0, 1.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<nHGrid; j++) {

                glVertex3f(xHyperbolic[i], yHyperbolic[j], 5.0f);
            }
            glEnd();
        }

        ///////////////////////////////////////////////////////////////////////////
        //nodes...
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(12.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_POINTS);
            for(int i=0; i<nHGrid; i++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }

        //grid...
        glColor3f(0.0, 1.0, 0.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_LINE_STRIP);
            for(int i=0; i<nHGrid; i++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }


        //nodes...
        glColor3f(1.0, 0.0, 1.0);
        glPointSize(12.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_POINTS);
            for(int j=0; j<nHGrid; j++) {

                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }

        //grid...
        glColor3f(0.0, 0.0, 1.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<nHGrid; j++) {

                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }
        ////////////////////////////////////////////////////////////////////////////
        /// \brief glColor3f
        //grid hyperbolic...
        glColor3f(0.0, 1.0, 0.0);
        for(int j=0; j<nHGrid; j++) {
            glBegin(GL_LINES);
            for(int i=0; i<nHGrid; i++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 0.0f);
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }

        glColor3f(0.0, 0.0, 1.0);
        for(int i=0; i<nHGrid; i++) {
            glBegin(GL_LINES);
            for(int j=0; j<nHGrid; j++) {
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 0.0f);
                glVertex3f(xHyperbolic[i], yHyperbolic[j], 10.0f);
            }
            glEnd();
        }

        //3D grid ends
////////////////////////////////////////////////////////////////////

    }

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
}

// Function used to display X and Y coordinates
// of a point
void CAirfoilDesigner::displayPoint(pdd P)
{
    cout << "(" << P.first << ", " << P.second
         << ")" << endl;
}

pdd CAirfoilDesigner::lineLineIntersection(pdd A, pdd B, pdd C, pdd D)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = B.second - A.second;
    double b1 = A.first - B.first;
    double c1 = a1*(A.first) + b1*(A.second);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.second - C.second;
    double b2 = C.first - D.first;
    double c2 = a2*(C.first)+ b2*(C.second);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return make_pair(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return make_pair(x, y);
    }
}

void CAirfoilDesigner::draw_star_wing(int n, float *xu, float *yu, float *zu, float *xl, float *yl, float *zl )
{
     nKnots = n;
     _starboardXu = xu; _starboardYu = yu; _starboardZu = zu;
     _starboardXl = xl; _starboardYl = yl; _starboardZl = zl;
     updateGL();
}
void CAirfoilDesigner::draw_port_wing(int n, float *xu, float *yu, float *zu, float *xl, float *yl, float *zl )
{
    nKnots = n;
    _portXu = xu; _portYu = yu; _portZu = zu;
    _portXl = xl; _portYl = yl; _portZl = zl;

    updateGL();
}

void CAirfoilDesigner::drawFuselage(int n1, int n2, float **xr, float **yr, float **zr, float **xl, float **yl, float **zl)
{
 _nOfPointsOnFuselage = n1;
 _nOfSlicesOnFuselage = n2;
 xRightFuselage = xr;
 xRightFuselage = yr;
 xRightFuselage = zr;
 xLeftFuselage = xl;
 yLeftFuselage = yl;
 zLeftFuselage = zl;
 ifDrawAnchors = false;
 ifDrawKnots = false;
 ifDrawFuselage = true;
 updateGL();
}


void CAirfoilDesigner::set_anchors(int n, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{
     nAnchors = n;
    _anchorXu = x1; _anchorYu = y1; _anchorZu = z1;
    _anchorXl = x2; _anchorYl = y2; _anchorZl = z2;
    if(!ifDrawAnchors)
    ifDrawAnchors = true;

    float *x = new float[n*2];
    int i = 0;
    for(i=0; i<(n); i++)
        x[i] = x1[i];
    for(i=n; i<(n*2); i++)
        x[i] = x2[i-n];

    float *y = new float[n*2];
    i = 0;
    for(i=0; i<(n); i++)
        y[i] = y1[i];
    for(i=n; i<(n*2); i++)
        y[i] = y2[i-n];

    selectionSort(x, n);
    selectionSort(y, n*2);

    xMin = x[0]; xMax = x[n*2-1];
    yMax = y[0]; yMin = y[n*2-1];

    _cx = xMin + ((xMax - xMin)/2.0);
    _cy = yMin + ((yMax - yMin)/2.0);
    _cz = 0.0;
    updateGL();
}

void CAirfoilDesigner::set_bezier_knots(int n, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{
     nKnots = n;
    _knotsXu = x1; _knotsYu = y1; _knotsZu = z1;
    _knotsXl = x2; _knotsYl = y2; _knotsZl = z2;
    if(!ifDrawKnots)
    ifDrawKnots = true;
    updateGL();
}

void CAirfoilDesigner::selectionSort(float arr[], int size)
{
    int indexOfMin, pass, j;

    for(pass=0; pass<size-1; pass++)
    {
        indexOfMin = pass;

        for(j=pass+1; j<size; j++)
            if(arr[j] < arr[indexOfMin])
                indexOfMin = j;

        std::swap(arr[pass], arr[indexOfMin]);
    }
}
