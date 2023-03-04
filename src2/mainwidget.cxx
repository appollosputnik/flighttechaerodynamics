#include "mainwidget.h"
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
MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(10),
    indexBuf(QOpenGLBuffer::IndexBuffer)
{
    m_rot[0] = 0.0;
    m_rot[1] = 0.0;
    m_rot[2] = 0.0;
    pi = 4*atan(1.0);
    timerEvent(0);
    update();
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    arrayBuf.destroy();
    indexBuf.destroy();
    doneCurrent();
}

void MainWidget::atmosphericData()
{
//Computation of Atmospheric data according to altitudes...

//cout << "Showing chart !!!\nEnter any key!!!!";
double _lapseRatel = -0.00065; // Lapse rate in troposphere= 0 - 11 km
//stratosphere= 11-25 km
double _lapseRate2 = 0.0003; // Lapse rate in ionosphere= 25-47 km
// 47-53 km
double _lapseRate3 = -0.00045; // Lapse rate in ozonosphere = 53-79 km
//	79-90 km
double _lapseRate4 = 0.00040; // Lapse rate in Exosphere= 90-105 km

unsigned int _heighest = 105000;
double *_temperature, *_density, *_pressure, *_dynamicViscosity,
*_kinematicViscosity;
unsigned int step= 100; //Insteps of 100-metre
_temperature= new double[_heighest/step];
_density= new double[_heighest/step];
_pressure= new double[_heighest/step];
_dynamicViscosity = new double[_heighest/step];
_kinematicViscosity = new double[_heighest/step];

double Ts= 288.16; // Kelvin
double densS = 1.2256; // kg/mA3
double PressS = 101325; // Pascal.
double ViscosityS = 1.716e-5; //sea level viscosity= kg/ms
double go= 9.81;
double R = 287; // Gas constant
double S = 110.4; // Sutherland temperature
ofstream out("weather_data.txt", ios::out);
//cout << "h =" << "seaLevel=0" << "Temperature="<< Ts << "Density= " << densS << "Pressure="<< PressS <<"Viscosity="<< ViscosityS << "Kinematic Viscosity=" << ViscosityS / densS << "\n";
out << "h =" << "seaLevel=0" <<"Temperature="<< Ts<<" DensitY=" << densS<<" Pressure="<< PressS <<"Viscosity="<< ViscosityS <<" Kinematic Viscosity= " <<	ViscosityS / densS <<	"\n";

for (unsigned int i= step, k = 0; i <= _heighest; i += step, k++)
{
    //0-11 km

    if (i <= 11000) {
    _temperature[k] =Ts+ _lapseRatel * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/(_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = " <<	i  <<	" Temperature = " <<	_temperature[ k] << " Density= "  << _density[k] <<	" Pressure= "  << _pressure[k] << " Viscosity= "  << _dynamicViscosity[k] << "  Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = " <<	i  <<	" Temperature = " <<	_temperature[ k] << " Density= "  << _density[k] <<	" Pressure= "  << _pressure[k] << " Viscosity= "  << _dynamicViscosity[k] << "  Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }

    //11-25 km
    else if (i >= 11000 && i <= 25000) { Ts= _temperature[k-1];
    _temperature[k] =Ts+ 0.0 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/
    (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }

    //25-47 km
    else if (i >= 25000 && i <= 47000) {
    Ts = _temperature[k - 1];
    _temperature[k] =Ts+ _lapseRate2 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }
    //47-53 km
    else if (i >= 47000 && i <= 53000) { Ts= _temperature[k - 1];
    _temperature[k] =Ts+ 0.0 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/
    (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }

    //53-79 km
    else if (i >= 53000 && i <= 79000) { Ts= _temperature[k - 1];
    _temperature[k] =Ts+ _lapseRate3 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/
    (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }

    //79-90 km
    else if (i >= 79000 && i <= 90000) { Ts= _temperature[k - 1];
    _temperature[k] =Ts+ 0.0 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/
    (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/
    (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }


    //90-105 km
    else {
    Ts= _temperature[k - 1];
    _temperature[k] =Ts+ _lapseRate4 * ((double)i - 0.0);
    _density[k] = densS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R) + 1.0));
    _pressure[k] = PressS * pow((_temperature[k] / Ts), (-1.0) * (go/ (_lapseRatel * R)));
    _dynamicViscosity[k] = ViscosityS * pow((_temperature[k] / Ts), 1.5) *
    ((Ts+ S) / (_temperature[k] + S));
    _kinematicViscosity[k] = _dynamicViscosity[k] / _density[k];
    //cout <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    out <<	"h = "  <<	i <<	" Temperature = "  <<	_temperature[k] <<	" Density= " << _density[k] << " Pressure= "  << _pressure[k] << "  Viscosity= "  << _dynamicViscosity[k] << " Kinematic Viscosity= "  << _kinematicViscosity[k] << "\n";
    }
}
}
void MainWidget::drawAirfoil()
{
    //We're still left with clustering of Airfoils with tanh and exp functions to capture the Leading Edge and Trailing Edge.
    //y = (tanhx-1)/(tanhx+l) and y = (expAx-1)/(expAx+l) and thereby changing signs of numerator and denominators
    //for capturing LE and TE respectively

    //Computing Airfoil...Generating Airfoils based on 5 digit nomenclature.
    //Leading Edge Coord(0, 0);
    //N= total number of points= 11
    int N= 11;
    //Trailing Edge  choord(100, 0);
    //Maximum Thickness= 30% c.
    //Maximum Camber= 7% c. double _maxThickness= 30;
    float _maxCamber= 0.07;
    //cout << "Enter your airfoils Maximum thickness in% of chord where chord= 100 metres\n";
    float _maxThickness = 30;

    //cout << "Enter your airfoils Maximum camber in% of chord where chord= 100  : metres\n";
    //cin >> _maxCamber;
    float *Yu= new float[N];
    float *Yl= new float[N];
    //Upper surface= 11 points
    //Lower surface= 11 points
    int NoOfPoints_LE= 3;
    int NoOfPoints_TE= 7;
    //calculating camber & points
    double y_interval = _maxCamber / (NoOfPoints_LE);

    double y_interval_thickness = (_maxThickness/2.0) / (NoOfPoints_LE);
    double*Yc= new double[NoOfPoints_LE];
    double* Yul= new double[NoOfPoints_LE];
    double* Yll= new double[NoOfPoints_LE];
    for(int i=  0; i < NoOfPoints_LE; i++)
    {
    Yc[i]=  _maxCamber - (i + 1) * y_interval;
    Yul[i]= ((_maxThickness/2.0) -((i+1) * y_interval_thickness)) + Yc[i];
    Yll[i]=  -(_maxThickness / 2.0 - (i + 1) * y_interval_thickness) - Yc[i];
    }
    double y2_interval=  _maxCamber / NoOfPoints_TE;
    double y2_interval_thickness= (_maxThickness / 2.0) / (NoOfPoints_TE);
    double* Yc2= new double[NoOfPoints_TE];
    double* Yut= new double[NoOfPoints_TE];
    double* Ylt= new double[NoOfPoints_TE];
    for (int i=  0; i < NoOfPoints_TE; i++) {
    Yc2[i]=  _maxCamber - (i + 1) * y2_interval;
    Yut[i]= (_maxThickness / 2.0 - (i + 1) * y2_interval_thickness) + Yc[i];
    Ylt[i]=  -(_maxThickness / 2.0 - (i + 1) * y2_interval_thickness) - Yc[i];
    }
    ofstream data("airfoil_data_30307.txt", ofstream::out); data << N << "\n";
    //calculating points over airfoil
    for (int i=	0; i < N; i++) {
    if (i < NoOfPoints_LE) {
    Yu[i] =	Yul[NoOfPoints_LE - 1 - i];
    Yl[i] =	Yll[NoOfPoints_LE - 1 - i];
    }
    else if (i  ==	NoOfPoints_LE) {
    Yu[i]=		_maxCamber + _maxThickness / 2.0; Yl[i]=	- _maxCamber -	_maxThickness / 2.0;
    }
    else {
    Yu[i]=	Yut[i-4];
    Yl[i]=	Ylt[i-4];
    }
    data << i * 10 << "	"  << Yu[i] << "\n";
    }
     data <<  "\n";
    for (int i=	0; i < N; i++) { if (i < NoOfPoints_LE) {
    data << i * 10 << " "; data << Yl[i] << "\n";
    }
    else if (i==	NoOfPoints_LE) {
    data << i * 10 << " "; data << Yl[i] << "\n";
    }
    else {

    data<< i * 10 << ""; data<< Yl[i] << "\n";
    }
    }
}

void MainWidget::ellipticMethod()
{
    //Generating grid over airfoil: Elliptic method...
    // delA2(y)/d(chi)A2 + delA2(y)/del(eta)A2 = 0;
    // delA2(x)/d(chi)A2 + delA2(y)/del(eta)A2 = 0;

    // grid.conf
    // 0-10%, 10-20%, 20-30%, 30-40%, 40-50%, 50-60%, 60-70%, 70-80%, 80-90%,

    //90-100%
    //0.1

    // 0.1 , 0.25,	0.5,	0.6,	1,	1,	1,
    //Read Airfoil Geometry points

    //chi = 11 points.
    //eta = 11 points.

    float control_points_X[11], control_points_Y[11], chi[11], eta[11];
    float diff1_x_chi[10], diff1_y_chi[10], diff1_x_eta[10], diff1_y_eta[10];
    float diff2_x_chi[9], diff2_y_chi[9], diff2_x_eta[9], diff2_y_eta[9];

    for(int i=0; i<10; i++)
    {
        diff1_x_chi[i] = (control_points_X[i+1] - control_points_X[i])/(chi[i+1] - chi[i]);
        diff1_x_eta[i] = (control_points_X[i+1] - control_points_X[i])/(eta[i+1] - eta[i]);

        diff1_y_chi[i] = (control_points_Y[i+1] - control_points_Y[i])/(chi[i+1] - chi[i]);
        diff1_y_eta[i] = (control_points_Y[i+1] - control_points_Y[i])/(eta[i+1] - eta[i]);

    }

    for(int i=0; i<9; i++)
    {

        diff2_x_chi[i] = (diff1_x_chi[i+1] - diff1_x_chi[i]);
        diff2_x_eta[i] = (diff1_x_eta[i+1] - diff1_x_eta[i]);

        diff2_y_chi[i] = (diff1_y_chi[i+1] - diff1_y_chi[i]);
        diff2_y_eta[i] = (diff1_y_eta[i+1] - diff1_y_eta[i]);

        //Equation:
        //XchiXeta + YchiYeta = 0; => (X1-X0)^2 + (Y1-Y0)^2 = 0;
        //XchiYeta - XetaYchi = J(chi, eta); =>
        //Y1 = Y0 +- sqrt(1 + ((eta1-eta0)/(chi1-chi0))^2
        //X1^2 - 2. X1. X0 + [X0^2 - (Y1 - Y0)^2]

    }

/*
    for (int j = 0; j < (0.1 * 100 - 1); j++)
    {

    for (int i= 0; i < 10; i++)
    {
        double* chi = new double[10];
        double* eta =  new double[10];

        for (int i= 0; i < 10; i++)
        {

        //chi[j] = chi_initial + chi_step;
        //eta[j]   eta initial+ eta_step;
        }
    }

    for (int i= 0; i < 10; i++)
    {
        for (int j = 0; j < (0.1 * 100 - 1); j++)
        {
        //diff_chi[j] = chi[j + 1] - chi[j];
        //diff_eta[j]	eta[j + 1] - eta[j];
        //diff_x_chi[j] =
        //diff_y_chi[j] =
        //diff_x_eta[j] =
        //diff_y_eta[j] =
        }
    }
    double* x = new double[9];
    double* y = new double[9];
    //Code equations 1 - 2; find x2 & y2
    //continue till x9, y9
    //i=l; i< 10, process

    for (int i= 0; i < 10; i += 10) {
        //for (int j = 0; i < (0.1 * segment); j++) {
    //(x3 - x2)* (chi2 - chil) - (x2 - xl) * (chi3 - chi2)
    //(x3 - x2) * (eta2 - etal) - (x2 - xl) * (eta3 - eta2);
    //(y3 - y2)* (chi2 - chil) - (y2 - yl) * (chi3 - chi2) =

    //(y3 - y2) * (eta2 - etal) - (y2 - yl) * (eta3 - eta2);
    //}
    //}
    }
    }
*/

}

void MainWidget::hyperbolicMethod()
{
    //Generating grid over airfoil: Hyperbolic method...
    // XchiYeta + YchiXeta = 0 - Orthogonality
    // XchiXeta - YchiYeta = Jacobian (chi, eta) - Hyperbolicity

    // grid.conf
    // 0-10%, 10-20%, 20-30%, 30-40%, 40-50%, 50-60%, 60-70%, 70-80%, 80-90%,


    //Read Airfoil Geometry points


    //chi = 11 points.
    //eta = 11 points.

    float control_points_X[11], control_points_Y[11], chi[11], eta[11];
    float diff_x_chi[10], diff_y_chi[10], diff_x_eta[10], diff_y_eta[10];

    for(int i=0; i<10; i++)
    {
        diff_x_chi[i] = (control_points_X[i+1] - control_points_X[i])/(chi[i+1] - chi[i]);
        diff_x_eta[i] = (control_points_X[i+1] - control_points_X[i])/(eta[i+1] - eta[i]);

        diff_y_chi[i] = (control_points_Y[i+1] - control_points_Y[i])/(chi[i+1] - chi[i]);
        diff_y_eta[i] = (control_points_Y[i+1] - control_points_Y[i])/(eta[i+1] - eta[i]);

        //Equation:
        //XchiXeta + YchiYeta = 0; => (X1-X0)^2 + (Y1-Y0)^2 = 0;
        //XchiYeta - XetaYchi = J(chi, eta); =>
        //Y1 = Y0 +- sqrt(1 + ((eta1-eta0)/(chi1-chi0))^2
        //X1^2 - 2. X1. X0 + [X0^2 - (Y1 - Y0)^2]

    }


}

void MainWidget::Navier_Stokes_equn()
{
    //Computing flow: Viscous, compressible flows - Navier-Stokes solver...
    //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
    //time step= 0
    //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

    //Algorithms:
    //for (int t = 0; t < 10000; t++) {

    // for(int j=0; j<noOfPointsin_j_drn; j++) {
    //  for(int i=0; i<noOfPointsln_i_drn; i++) {
    //predictor [t]
    //altitude= 15000 metre
    // _lapseRate = -0.00065 K & Ts= 288.16 Kelvin
    // _temp_ij[i][j][t] =Ts+ (_lapseRate)·* altitude
    //density altitude= _dens_ij[i][j][t]
    //Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
    //_temp_ij[i][j][t]
    //_velo_u_ij[ij[j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
    //- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    //(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
    //- ((_velo_u_ij[i-l][j][t]/delX)))
    //_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
    //- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    //(_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j]/delY) + (_velo_ij[i][j][t]/delX) - ((_velo_v_ij[i-l][j][t]/delY)))
    //corrector step
    //_press_ij[i][j][t] = (_press_ij[i - l][j][t] / delY) - _dens_ij[i][j][t] *
    //(_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) + (_velo_u_ij[i][j][t] / delX)
    //- (_velo_v_ij[i - l][j][t] / delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j - l][t] / delX))
    //_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
    //((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
    //(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * '(_velo_v_ij[i][j-l][t]/delX));

    //Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
    //DNS Turbulence model
    //_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
    //(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
    //_velo_v[i][j][t]/delY)) - ((_velo u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
    //(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY)) + ((-1.0) *
    //(_temp_ij[i-l][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] - temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
    //(_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2][j])/delX))) - ((2/3 *
    //_mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
    //_velo_u[i-3][j])/(2*delX) +
    //((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delY))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    //_velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
    //2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    //_velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
    //_velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
    //((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
    //(_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *

    //(_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
    //(_velo_v_ij[i-3][j]))/(2 * delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
    //(_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/(2 * delX)) + (((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]-_velo_v_ij[i-2J[j]) / delX) *
    //((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/((_dens_ij[i][j][t]/delX) +
    //(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
    //(_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
    //k-e Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    //])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    //] ) ) ;
    //k-w Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    //])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    //]) /t);
    //	}
    //}
    //if (t	== 2) {

    //dens_ij[i][j][t-1] - dens[i][j][t-2] =?
    //press_ij[i][j][t-1] - press[i][j][t-2]  ?

        /*
    //Courant no= 0.5 ~ 0.95 Conmvergeance criteria
    //dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
    //visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
    */

    //SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
    /*
    if (leastValue <= 10e-6 * Courant_no) //Converged
    {



    }
    else

    bConverged = true; break;

    continue;

    if (bConverged) break;
    else
    continue;
    */

    /* for (int t = 0; t < 5000; t++) {

    ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////l/l//l/!////////I///////////I////////////////////IJ//////
    ///////////////////
    //dd_dens[t] = del_dens[2] - del_dens[l];
    //dd_press[t] = del_press[3] = del_press[2];
    //Least values: dd_dens[2] - dd_dens[l] =?
    //Convergeance factor

    //if (dd_dens[t]) {
    //least value=> C.F <= 10e-6 * courant no => converged
    //get index[t*2] - index[t*2-1]


    //retrieve
    //for(t=0;
    //	{

    dens[t,j] press[t,j] u_ij[t,j]
    v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

    // dens_ij_N
    // press_ij_N[t][j]

    //Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
    } */
    //}
    //}
}

void MainWidget::compressibleFlowsNSEquation()
{
//Computing flow: Viscous, compressible flows - Navier-Stokes solver...
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:
//for (int t = 0; t < 10000; t++) {

// for(int j=0; j<noOfPointsin_j_drn; j++) {
//  for(int i=0; i<noOfPointsln_i_drn; i++) {
//predictor [t]
//altitude= 15000 metre
// _lapseRate = -0.00065 K & Ts= 288.16 Kelvin
// _temp_ij[i][j][t] =Ts+ (_lapseRate)·* altitude
//density altitude= _dens_ij[i][j][t]
//Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
//_temp_ij[i][j][t]
//_velo_u_ij[ij[j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
//- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
//(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
//- ((_velo_u_ij[i-l][j][t]/delX)))
//_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
//- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
//(_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j]/delY) + (_velo_ij[i][j][t]/delX) - ((_velo_v_ij[i-l][j][t]/delY)))
//corrector step
//_press_ij[i][j][t] = (_press_ij[i - l][j][t] / delY) - _dens_ij[i][j][t] *
//(_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) + (_velo_u_ij[i][j][t] / delX)
//- (_velo_v_ij[i - l][j][t] / delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j - l][t] / delX))
//_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
//((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
//(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * '(_velo_v_ij[i][j-l][t]/delX));

//Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
//DNS Turbulence model
//_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
//(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
//_velo_v[i][j][t]/delY)) - ((_velo u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
//(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY)) + ((-1.0) *
//(_temp_ij[i-l][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] - temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
//(_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2][j])/delX))) - ((2/3 *
//_mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
//_velo_u[i-3][j])/(2*delX) +
//((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delY))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
//_velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
//2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
//_velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
//_velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
//((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
//(_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *

//(_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
//(_velo_v_ij[i-3][j]))/(2 * delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
//(_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/(2 * delX)) + (((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]-_velo_v_ij[i-2J[j]) / delX) *
//((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/((_dens_ij[i][j][t]/delX) +
//(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
//(_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
//k-e Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//] ) ) ;
//k-w Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//]) /t);
//	}
//}
//if (t	== 2) {

//dens_ij[i][j][t-1] - dens[i][j][t-2] =?
//press_ij[i][j][t-1] - press[i][j][t-2]  ?

    /*
//Courant no= 0.5 ~ 0.95 Conmvergeance criteria
//dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
//visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
*/

//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
/*
if (leastValue <= 10e-6 * Courant_no) //Converged
{



}
else

bConverged = true; break;

continue;

if (bConverged) break;
else
continue;
*/

/* for (int t = 0; t < 5000; t++) {

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////l/l//l/!////////I///////////I////////////////////IJ//////
///////////////////
//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];
//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t]) {
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]


//retrieve
//for(t=0;
//	{

dens[t,j] press[t,j] u_ij[t,j]
v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

// dens_ij_N
// press_ij_N[t][j]

//Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
} */
//}
//}
}

void MainWidget::DomainVertexMethod()
{
    /* Domain Verte method
    Finite Element method...

    PHI1 = (1-chi).(1-eta).(1-jita)
    PHI2 = chi.(1-eta).(1-jita)
    PHI3 = chi.eta.(1-jita)
    PHI4 = (1-chi).eta.(1-jita)
    PHI5 = (1-chi).(1-eta).jita
    PHI6 = chi.(1-eta).jita
    PHI7 = chi.eta.jita
    PHI8 = (1-chi).eta.jita


    //QUAD SURFACE MESH INTO 8 POINTS...
    x = PHI1x1 + PHI2x2 + PHI3x3 + PHI4x4 + PHI5x5 + PHI6x6 + PHI7x7 + PHI8x8
    y = PHI1y1 + PHI2y2 + PHI3y3 + PHI4y4 + PHI5y5 + PHI6y6 + PHI7y7 + PHI8y8
    z = PHI1z1 + PHI2z2 + PHI3z3 + PHI4z4 + PHI5z5 + PHI6z6 + PHI7z7 + PHI8z8
*/
    //AIRFOIL UPPER SURFACE 8 POINTS AND LOWER SURFACE 8 POINTS GENERATE AND MESH USING DOMAIN
    //VERTEX METHOD...
}

void MainWidget::unstructuredMethod()
{
    //Unstructured grid...
    //11 points on upper surface and 11 points on lower surface....Pu0-Pu10 &&
    //Pl0-Pl10

    //Make polynomial of Upper surface and Lower surface : Pu0-Pu10 and Pl0-Pl10/////////////f////////////////////////////////////////
    //y=an.xn+an-1.xn-l+alxl+aoxo+K find the values of an-ao & Kand find the polynomial	.
    //similarly find the polynomial for lower surface...
    //Use K = 100 and create the boundary outer surafce points 11 points and n =
    //10.
    //For upper surface
    int i= 0, n = 10; double* p, * q;
    do {

    //p[i + 1] ~p[i] intersection point=> q[i]; //10 points
    //make a triangle p[i+l], p[i] & q[i]
    //if q[i] satisfies the airfoil polynomial equation then discard it and freeze further marching.
    i++;
    if (i == n)
        {
        for (int j = 0; j < n; j++)
            {
            p[j] = q[j];
            }
        i = 0;
        n--;
        }
    } while (n >= 3);


    //similarly for lower surface.
}

void MainWidget::BoltzMannEqun()
{
//Computing flow: Viscous, compressible flows - Navier-Stokes solver...Boltzman's Equation...
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:
/* for (int t = 0; t < 10000; t++) {
// for(int j=0; j<noOfPointsin_j_drn; j++) {
//  for(int i=0; i<noOfPointsln_i_drn; i++) {
//predictor [t]
//altitude= 15000 metre
// _lapseRate = -0.00065 K & Ts= 288.16
// _temp_ij[i][j][t] =Ts+ (_lapseRate) * altitude
//density altitude= _dens_ij[i][j][t]
//Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *

_temp_ij[i][j][t]
//_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
- ((_velo_u_ij[i-l][j][t]/delX)))
//_velo_v_ij[i][j][t] =  ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
(_press_ij[i-l][j][t]/delV)))/((_velo_ij[i][j]/delY) + (_velo_ij[i][j][t]/delX) - ((_velo_v_ij[i-l][j][t]/delY)))
//corrector step
//F = (_dens_ij[i][j][t])/Io * sqrt(beta/pi) * exp(-beta*(chi -
_velo_ij[i][j][t]*_velo_ij[i][j][t])+I/Io);
// f = velocity distribution function
// f = _velo_ij[i][j][t] = sqrt(_velo_u_ij[i][j][t] * _velo_u_ij[i][j][t] +
_velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
// tR = 1
//_press_ij[i][j][t] = (F-f/tR);
//_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));

Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
//DNS Turbulence model
//_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delV)) + ((-1.0) *
(_temp_ij[i-l][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] -
_temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
(_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2J[j])/delX))) - ((2/3 *
_mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
_velo_u[i-3][j])/(2*delX) +
((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delV))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
_velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
_velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
(_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *
(_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
(_velo_v_ij[i-3][j]))/(2 *  delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
(_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/(2 * delX)) + (((_mew_ij[i-l][j]) *
(_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j]) / delX) *
((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/((_dens_ij[i][j][t]/delX) +
(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
(_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
//k-e Turbulence Model

// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] *·1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
]));
//k-w Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][jJ[t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])/t);
//	}
//}
if (t	== 2) {
//dens_ij[i][j][t-1] - dens[i][j][t-2] =?
//press_ij[i][j][t-1] - press[i][j][t-2]  ?
/*
Courant no= 0.5 ~ 0.95 Conmvergeance criteria
dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
*/

//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
/*
if (leastValue <= 10e-6 * Courant_no) //Converged
{



}
else

bConverged = true; break;

continue;

if (bConverged) break;

else

*/

//for (int t = 0; t < 5000; t++) {

//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];

//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t]) {
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]


//retrieve
//for(t=0;
//II {

//dens[t,j] press[t,j] u_ij[t,j]
//v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

// dens_ij_N
// press_ij_N[t][j]

//Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
//}
//}
}

void MainWidget::diagonalFormAsDKS()
{
    //Computing flow: Viscous, compressible flows - Navier-Stokes solver...diagonal form as a discrete kinetic system system...
    //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
    //time step= 0
    //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

    //Algorithms:
    //for (int t = 0; t < 10000; t++) {
    // for(int j=0; j<noOfPointsin_j_drn; j++) {
    //  for(int i=0; i<noOfPointsin_i_drn; i++) {
    //predictor [t]
    //altitude= 15000 metre
    // _lapseRate = -0.00065 K & Ts= 288.16 Kelvin
    // _temp_ij[i][j][t] =Ts+ (_lapseRate) * altitude
    //density altitude= _dens_ij[i][j][t]
    //Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
    //_temp_ij[i][j][t]
    //_velo_u_ij[il[j][t] = _velo_u_ij[i-l][j][t] - (1/lambda*lambda*epsilon) * (_velo_v_ij[i-l][j][t] - (_press_ij[i-l][j][t] -
    //_press_ij[i-2][j][t])/(_velo_ij[i-l][j][t] - _velo_ij[i-2][j][t]))
    //_velo_v_ij[i][j][t] = _velo_v_ij[i-l][j][t]
    //corrector step
    //_press_ij[i][j][t] = (_press_ij[i - l][j][t] / delY) - _dens_ij[i][j][t]
    //* (_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) + (_velo_u_ij[i][j][t] /

    //delX) - (_velo_v_ij[i - l][j][t] / delY)) - (_velo_u_ij[i][j][t]) * (_veio_v_ij[i][j - l][t] / delX))
    //_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
    //((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
    //(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));	·

    //Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
    //DNS Turbulence model
    //_energy_ij[i][j][t] =  ((_energy_ij[i-l][j][t]) *
    //(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
    //_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
    //(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY)) + ((-1.0) *
    //(_temp_ij[i-l][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] -
    //_temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
    //(_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2][j])/delX))) - ((2/3 *
    //_mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
    /*_velo_u[i-3][j])/(2*delX) +
    ((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delY))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
    2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
    _velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
    ((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
    (_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *
    (_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
    (_velo_v_ij[i-3][j]))/(2 *  delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
    (_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/(2 * delX)) + (((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j]) / delX) *
    ((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/((_dens_ij[i][j][t]/delX) +
    ( dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
    (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
    //k-e Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ] ) ) ;
    //k-w Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo v_ij[i][j][t
    ])/t);
    II	}
            */
    //}
    //if (t	== 2) {

    //dens_ij[i][j][t-1] - dens[i][j][t-2] =?
    //press_ij[i][j][t-1] - press[i][j][t-2] =?

    /*
    Courant no= 0.5 ~ 0.95 Conmvergeance criteria· dens_ij[l] - dens_ij[0] = del_dens;
    press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
    visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
    */

    //SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
    /*
    if (leastValue <= 10e-6 * Courant_no) //Converged
    {
    bConverged = true; break;
    }
    else
    continue; if (bConverged)
    break;
    else
    continue;
    */

    //for (int t = 0; t < 5000; t++) {

    //dd_dens[t] = del_dens[2] - del_dens[l];
    //dd_press[t] = del_press[3] = del_press[2];
    //Least values: dd_dens[2] - dd_dens[l] =?
    //Convergeance factor

    //if (dd_dens[t]) {
    //least value=> C.F <= 10e-6 * courant no => converged
    //get index[t*2] - index[t*2-1]


    //retrieve
    //lfor(t=0;
    //II	{

    //dens[t,j] press[t,j] u_ij[t,j]
    //v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

    // dens_ij_N
    // press_ij_N[t][j]

    //Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
    //}
    //}
    //}
}


void relaxationTechniques()
{
    //Computing flow: Viscous, compressible flows - Navier-Stokes solver...Diagonal form for Relaxation systems...
    //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
    //time step= 0
    //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

    //Algorithms:
    /* for (int t = 0; t < 10000; t++) {
    // for(int j=0; j<noOfPointsln_j_drn; j++) {
    //  for(int i=0; i<noOfPointsin_i_drn; i++) {
    //predictor [t]
    //altitude= 15000 metre
    // _lapseRate = -0.00065 K & Ts= 288.16 Kelvin
    // _temp_ij[i][j][t] =Ts+ (_lapseRate) * altitude
    //density altitude= _dens_ij[i][j][t]
    //Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] *  R *
    _temp_ij[i][j][t]
    //_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
    - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    (_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
    - ((_velo_u_ij[i-l][j][t]/delX)))
    //_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
    - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    (_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j]/delY) + (_velo_ij[i][j][t]/delX) - ((_velo_v_ij[i-l][j][t]/delY))) ·
    //_velo_ij[i][j][t] = sqrt(_velo_u_ij[i][j][t] * _velo_u_ij[i][j][t] +
    _velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
    //corrector step
    //_press_x_ij[i][j][t] = (1/sqrt(gamma * R *
    _temp_ij[i][j][t])/2/lambda/epsilon) * (_velo_v_ij[i][j][t] - (_press_x_ij[i-l][j][t]-_press_x_ij[i-2][j][t])/(_velo_u_ij[i-l][j][t] -
    _velo_u_ij[i-2][j][t]));
    //_press_y_ij[i][j][t] = (1/sqrt(gamma * R *
    _temp_ij[i][j][t])/2/lambda/epsilon) * (_velo_v_ij[i][j][t] - (_press_y_ij[i-l][j][t]-_press_x_ij[i-2][j][t])/(_velo_u_ij[i-l][j][t] -
    _velo_u_ij[i-2][j][t]));
    //_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
    ((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
    (_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) *
    (_velo_v_ij[i][j-l][t]/delX));

    Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
    //DNS Turbulence model
    //_energy_ij[i][j][t] = (( energy_ij[i-l][j][t]) *
    (_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
    _velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
    (_velo_v_ij[i][j][t] * (_pres _ij[i][j][t]/delY)) + ((-1.0) *
    (_temp_ij[i-1][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] -
    _temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
    (_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2][j])/delX))) - ((2/3 *
    _mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
    _velo_u[i-3][j])/(2*delX) +
    ((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delY))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
    2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
    _velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
    ((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
    (_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *
    (_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
    (_velo_v_ij[i-3][j]))/(2 *  delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
    (_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/(2 * delX)) + (((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j]) / delX) *
    ((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/((_dens_ij[i][j][t]/delX) +
    (_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
    (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
    //k-e Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 *	, sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ]));
    //k-w Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    .J)) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 *
    'sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+ velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])/t);
    //	}
    //}
    if (t	== 2) {
    //dens_ij[i][j][t-1] - dens[i][j][t-2] =?
    //press_ij[i][j][t-1] - press[i][j][t-2] =?
    /*
    Courant no= 0.5 ~ 0.95 Conmvergeance criteria

    dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[ ] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
    visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
    */

    //SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
    /*
    if (leastValue <= 10e-6 * Courant_no) //Converged
    {



    }
    else

    bConverged = true; break;

    continue; if (bConverged)
    break;
    else
    continue;
    */

    //for (int t = 0; t < 5000; t++) {
    //dd_dens[t] = del_dens[2] - del_dens[l];
    //dd_press[t] = del_press[3] = del_press[2];
    //Least values: dd_dens[2] - dd_dens[l] =?
    //Convergeance factor

    //if (dd_dens[t]) {
    //least value=> C.F <= 10e-6 * courant no => converged
    //get index[t*2].- index[t*2-1]


    //retrieve
    //for(t=0;
    //{

    //dens[t,j] press[t,j] u_ij[t,j]
    //v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

    // dens_ij_N
    // press_ij_N[t][j]

    //Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
    //}

    //}
    //}


}

void chapmanEnskogTypeCFDSolver()

{
    //Comput1ng flow: Viscous, compressible flows - Navier-Stokes solver	With
    //Chapman-Enskog type Expansion waves for Relaxation system...
    //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
    //time step= 0
    //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

    //Algorithms:
    /* for (int t = 0; t < 10000; t++) {
    // for(int j=0; j<noOfPointsin_j_drn; j++) {
    //  for(int i=0; i<noOfPointsin_i_drn; i++) {
    //predictor [t]
    //altitude= 15000 metre
    // _lapseRate = -0.00065 K & Ts= 288.16 Kelvin
    // _temp_ij[i][j][t] =Ts+ (_lapseRate) * altitude
    //density altitude= _dens_ij[i][j][t]
    //Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
    _temp_ij[i][j][t]
    //_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
    - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    (_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
    - ((_velo_u_ij[i-l][j][t]/delX)))
    //_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
    - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    (_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j]/delY) + (_velo_ij[i][j][t]/delX) - ((_velo_v_ij[i-l][j][t]/delY)))
    //_velo_ij[i][j][t] = sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t] +
    _velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
    //corrector step
    // _qp_ij[i][j][t] = (( velo_ij[i][j][t] - _velo[i-l][j][t])*_lambda *
    _lambda/delX) - ((_press_ij[i][j][t] - _press_ij[i-l][j][t]) * (_press_ij[i][j][t]
    - _press_ij[i-l][j][t]))/((_velo_ij[i][j][t] - _velo[i-l][j][t]));
    //_press_ij[i][j][t] =epsilon* (_qp_ij[i][j][t] -
    _qp_ij[i-l][j][t])/delX;
    //_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
    ((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
    (_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));

    Heat Transfer -- FRom here below we have viscous term for Energy equations ADdd Heat Transfer
    //DNS Turbulence model
    //_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
    (_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *

    _velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
    (_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY)) + ((-1.0) *
    (_temp_ij[i-1][j][t]-_temp_ij[i-2][j][t])/delX) + ((-1.0) * (_temp_ij[i-l][j][t] -
    _temp_ij[i-2][j][t])/delY) (_mew_ij[i-l][j] * (_velo_u_ij[i-l][j] *
    (_velo_u_ij[i-l][j] -2 _velo_u_ij[i-l][j] + _velo_u_ij[i-2][j])/delX))) - {{2/3 *
    _mew_ij[i-l][j] *(((_velo_u_ij[i-l][j] - 2 * _velo_u[i-2][j] - 2 *
    _velo_u[i-3][j])/{2*delX) +
    ((_velo_v_ij[i-l][j])-2*(_velo_v_ij[i-2][])+(_velo_v_ij[i-3][j]))/(delX*delY))))) + ((_mew_ij[i-l][j]/delX) * (_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j])) - (((_mew_ij[i-l][j] *
    2)/(3))*(((_velo_u_ij[i-l][j]-2*_velo_u_ij[i-2][j] +
    _velo_u_ij[i-3][j]/(2*delX))/()) - (((_velo_u_ij[i-l][j] -
    _velo_u_ij[i-2][j])/delX) * ((_velo_v_ij[i-l][j] - _velo_v_ij[i-l][j-1])/delY)))) + ((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]) *
    ((_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j])/delX) *
    (_velo_u_ij[i-l][j]-_velo_u_ij[i-l][j-1])/delY)) + ((_mew_ij[i-l][j]) *
    (_velo_v_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) - (2*(_velo_v_ij[i-2][j])) +
    (_velo_v_ij[i-3][j]))/{2 *  delX)) + ((_mew_ij[i-l][j]) * ((_velo_v_ij[i-l][j]) -
    (_velo_v_ij[i-2][j]) + (_velo_v_ij[i-3][j]))/{2 * delX)) + (((_mew_ij[i-l][j]) * (_velo_v_ij[i-l][j]-_velo_v_ij[i-2][j]) / delX) *
    ((_velo_u_ij[i][j-1]-_velo_u_ij[i][]-2)/delY))))/({_dens_ij[i][j][t]/delX) +
    (_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) *
    (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)));
    //k-e Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ] ) ) ;
    //k-w Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])/t);
    //	}
    //}
    if (t	== 2) {
    //dens_ij[i][j][t-1] - dens[i][j][t-2] =?
    //press_ij[i][j][t-1] - press[i][j][t-2]  ?
    /*
    Courant no= 0.5 ~ 0.95 Conmvergeance criteria
    dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
    visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
    */

    //SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an ar ay
    /*
    if (leastValue <= 10e-6 * Courant_no) //Converged
    {
    bConverged = true; break;
    }
    else
    continue; if (bConverged)
    break;
    else
    continue;
    */

    /* for (int t = 0; t < 5000; t++) {

    //dd_dens[t] = del_dens[2] - del_dens[l];
    //dd_press[t] = del_press[3] = del_press[2];
    //Least values: dd_dens[2] - dd_dens[l] =?
    //Convergeance factor

    //if (dd_dens[t]) {
    //least value=> C.F <= 10e-6 * courant no => converged
    //get index[t*2] - index[t*2-1]


    //retrieve
    for(t=0;
    {

    dens[t,j] press[t,j] u_ij[t,j]
    v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

    dens_ij_N
    press_ij_N[t][j]
    */
    //Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
}

void MainWidget::compressible_euler_solver()
{
    //Computing flow: Inviscid, compressible flows - Euler Solver...
    //Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
    //time step= 0
    //Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

    //Algorithms:
    //for (int t = 0; t<10000; t++) {
    // for(int j=0; j<noOfPointsin_j_drn; j++) {
    //  for(int i=0; i<noOfPointsin_i_drn; i++) {
    //predictor step
    //density altitude= _dens_ij[i][j][t];
    //Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
    //_temp_ij[i][j][t]
    //_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-l][t]/delX)
    //- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    //(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
    //- ((_velo_u_ij[i-l][j][t]/delX)))
    //_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-l][t]/delX)
    //- (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
    //(_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j][t]/delY) + (_velo_ij[i][j][t]/delX)
    //- ((_velo_v_ij[i-l][j][t]/delY)))
    //corrector step
    //_press_ij[i][j][t] = (_press_ij[i - l][j][t] / delY) - _dens_ij[i][j][t] *
    //(_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) + (_velo_u_ij[i][j][t] / delX)
    //- (_velo_v_ij[i - l][j][t] / delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j - l][t] / delX))
    //_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
    //({_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
    //(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));
    //DNS Turbulence model
    //_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
    /* .(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
    _velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
    (_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) +
    (_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)))
    //k-e Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ]) ) ;
    //k-w Turbulence Model
    // _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 *

    sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
    ])/t);
    //	}
    //	}
    if (t	== 2) {
    //dens_ij[i][j][t-1] - dens[i][j][t 2] =?
    //press_ij[i][j][t-1] - press[i][j][t-2] =?

    Courant no= 0.5 ~ 0.95 Conmvergeance criteria
    dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
    visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;


    //SelectionSort(del_dens, del_press,.del_temp, del_visc, del_visc_kinematic); make an array

    if (leastValue <= 10e-6 * Courant_no) //Converged
    {



    }
    else

    bConverged = true; break;

    continue; if (bConverged)
    break;
    else
    continue;


    for (int t = 0; t < 5000; t++) {

    //dd_dens[t] = del_dens[2] - del_dens[l];
    //dd_press[t] = del_press[3] = del_press[2];
    //Least values: dd_dens[2] - dd_dens[l] =?
    //Convergeance factor

    //if (dd_dens[t]) {
    //least value=> C.F <= 10e-6 * courant no => converged
    //get index[t*2] - index[t*2-1]
    //retrieve dens[t,j]
    //	press[t,j]
    //	u_ij[t,j]

    //for(t=0;
    {

    v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

    // dens_ij_N
    // press_ij_N[t][j]

    //Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity : Plotting & GRaph charts.
    }
    }
    } */
}


void MainWidget::MonteCarloSimulation()
{
         //Monte Carlo Simulation - for Kinetic Theory of motions. friction coefficient, deceleration & Number range 0-99
         // Consider this and simulate a Simulation once again...

    /*     double r2 = 16; double rl = 10; double tr= 10; double delT = 1; double delX = 1; double delY = 1;
         //double courant_no = delT / delX = 0.5; double*** f_ij_t=0, **w_ij_t=0;

         for (int t = 1; t < 10000; t++) { for (int j = 0; j < 10; j++) {
         for (inti= 0; i < 10; i++) {

         density altitude= _dens_ij[i][j][t];
         Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R
         * _temp_ij[i][j][t];
         _velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] *
         _velo_u_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
         (_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
         - ((_velo_u_ij[i-l][j][t]/delX)))
         //_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] *
         _velo_v_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
         (_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j][t]/delY) + (_velo_ij[i][j][t]/delX)
         - ((_velo_v_ij[i-l][j][t]/delY)))

*/
         //for (int t = 0; t < 5000; t++) {
         //dd_dens[t] = del_dens[2] - del_dens[l];
         //dd_press[t] = del_press[3] = del_press[2];
         //dd_N[t] = del_N[3] = del_N[2];

         //Least values: dd_N[2] - dd_N[l] =?
         //Convergeance factor

         //if (dd_N[t]) {
         //least value=> C.F <= 10e-6 * courant no => converged
         //get index[t*2] - index[t*2-1]
         //retrieve dens[t,j]
         //	press[t,j]
         //	u_ij[t,j]
         //	v_ij[t,j)
         //	N_ij[t,j]
         //for(t=0; t<10000; t++
         //	{
         // dens_ij_N
         // press_ij_N[t][j]

         //Conclude outputs, End, Pressure, Density, Temperature, N Plotting & GRaph charts.
}

void MainWidget::EulerSolverBoltzmanEquan()
{
//Computing flow: Inviscid, compressible flows - Euler Solver...Boltzman's Equation
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:

for (int t= 0; t < 10000; t++) {

// for (int j=0; j<noOfPointsin_j_drn; j++) {

//for(int i=0; i<noOfPointsin_i_drn; i++) {

//predictor step
//density altitude = _dens_ij[i][j][t];
//Equation of state = _press_ij[i][j][t] = _dens_ij[i][j][t] * R * _temp_ij[i][j][t];

//_velo_u_ij[i][j][t] =((_velo_v_ij[i][j][t] * _velo_u_ij[i][j-1][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t] - (_press_ij[i-1][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)-((_velo_v_ij[i-1][j][t]/delY)));
//_velo_v_ij[i][j][t] =((_velo_u_ij[i][j][t] * _velo_v_ij[i][j-1][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t] - (_press_ij[i-1][j][t]/delY)))/((_velo_ij[i][j][t]/delY) + (_velo_ij[i][j][t]/delX)-((_velo_v_ij[i-1][j][t]/delY)));


//corrector step

//  F = (_dens_ij[i][j][t])/Io * sqrt(beta/pi) * exp(-beta*(chi - _velo_ij[i][j][t]*_velo_ij[i][j][t])+I/Io);
//  f = velocity distribution function
//  f  =  _velo_ij[i][j][t]  =  sqrt(_velo_u_ij[i][j][t]  *
//_velo_u_ij[i][j][t] + _velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
// tR = 1
//_press_ij[i][j][t]r= (F-f/tR);
//_dens_ij[i][j][t]  =  (_p ess_ij[i-l][j][t]/delY -  _press_ij[i][j][t]) /
//((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
//(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));

//DNS Turbulence model

//energy_ij[i][j][t] = _energy_ij[i-1][j][t] *
//(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
//_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
//(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) +
//(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)))

//k-e Turbulence Model

//_energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 *
//sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 *
//sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t])) ;

//k-wnTurbulence Model

// _e ergy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 *
//sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])/t);
//	}
//	}
//if (t == 2) {
//dens_ij[i][j][t-1] -	dens[i][j][t-2] =?

//press_ij[i][j][t-1] - press[i][j][t-2] =?

//Courant no= 0.5 ~ 0.95 Conmvergeance criteria
//dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
//visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;


//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
//if (leastValue <= 10e-6 * Courant_no) //Converged
//{



//}
//else

//bConverged = true; break;

//continue; if (bConverged)
//break;
//else
//continue;


//for (int t = 0; t < 5000; t++)
 //                                                  {
//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];

//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t])
//{
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]


//retrieve
//for(t=0;
//	{

//dens[t,j] press[t,j] u_ij[t,j]
//v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000;  t++

// dens_ij_N
// press_ij_N[t][j]
//Conclude outputs, End, Pressure, Density, Temperature,

//Viscosity, Kinematic Viscosity
   }
}
//}


void MainWidget::diagonalFormRelaxationSystem()
{
//Computing flow: Inviscid, compressible flows - Euler Solver...Diagonal form for Relaxation techniques
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:
/* for (int t = 0; t < 10000; t++) {
// for(int j=0; j<noOfPointsin_j_drn; j++) {
//  for(int i=0; i<noOfPointsin_i_drn; i++) {
//predictor step
//density altitude= _dens_ij[i][j][t];
//Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
_temp_ij[i][j][t]
//_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] *
_velo_u_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
- ((_velo_u_ij[i-l][j][t]/delX)))
//_velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] *
_velo_v_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
(_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j][t]/delY) + (_velo_ij[i][j][t]/delX)
- ((_velo_v_ij[i-l][j][t]/delY)))
//_velo_ij[i][j][t] = sqrt(_velo_u_ij[i][j][t] *

_velo_u_ij[i][j][t] + _velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
//corrector step
//_press_x_ij[i][j][t] = (1/sqrt(gamma * R *
_temp_ij[i][j][t])/2/lambda/epsilon) * (_velo_v_ij[i][j][t] - (_press_x_ij[ i-1][j][t]-_press_x_ij[i-2][j][t])/(_velo_u_ij[ i-1][ j][t] -
_velo_u_ij[i-2][j][t]));
//_press_y_ij[i][j][t] = (1/sqrt(gamma * R *
_temp_ij[i][j][t])/2/lambda/epsilon) * (_velo_v_ij[i][j][t] - (_press_y_ij[i-l][j][t]-_press_x_ij[i-2][j][t])/(_velo_u_ij[i-1][j][t] -
_velo_u_ij[i-2][j][t]));
//_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j-l][t]/delX));
//DNS Turbulence model
//_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) +
(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)))
//k-e Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
] ) ) ;
//k-w Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i](j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])/t);
//	}
//	}
if (t	== 2) {
//dens_ij[i][j][t-1] - dens[i][j][t-2] =?
//press_ij[i][j][t-1] - press[i][j][t-2] =?

/*
Courant no= 0.5 ~ 0.95 Conmvergeance criteria
dens_ij[l] - dens_ij(0] = del_dens; press_ij[l] - press_ij(0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij(0] = del_visc;
visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
*/

//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array

/*
if (leastValue <= 10e-6 * Courant_no) //Converged
{



}
else

bConverged = true; break;

continue; if (bConverged)
break;
else
continue;
*/

//for (int t = 0; t < 5000; t++) {
//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];

//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t]) {
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]

//retrieve dens[t,j]

//  press[t,j]
//	u_ij[t,j]
//	v_ij[t,j]
//	new_ij[t,j]
//	mew_ij[t,j]
//for(t=0; t<10000; t++
//	{
// dens_ij_N
// press_ij_N[t][j]

//Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
//}
//}
}

void MainWidget::ChapmanEnskogTypeEulerSolver()
{
//Computing flow: Inviscid, compressible flows - Euler Solver...With Chapman-Enskog type Expansion waves for Relaxation system...
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:
//* for (int t = 0; t < 10000; t++) {
// for(int j=0; j<noOfPointsin_j_drn; j++) {
//  for(int i=0; i<noOfPointsin_i_drn; i++) {
//predictor step
//density altitude= _dens_ij[i][j][t];
//Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
//_temp_ij[i][j][t]
//_velo_u_ij[i][j][t] = ((_velo_v_ij[i][j][t] *
//_velo_u_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
//(_press_ij[i-l][j][t]/delX)))/((_velo_ij[i][j][t]/delX) + (_velo_ij[i][j][t]/delY)
//- ((_velo_u_ij[i-l][j][t]/delX)))
//I I _velo_v_ij[i][j][t] = ((_velo_u_ij[i][j][t] *
//_velo_v_ij[i][j-l][t]/delX) - (1/_dens_ij[i][j][t])((_press_ij[i][j][t]) -
//(_press_ij[i-l][j][t]/delY)))/((_velo_ij[i][j][t]/delY) + (_velo_ij[i][j][t]/delX)
//- ((_velo_v_ij[i-l][j][t]/delY)))
//_velo_ij[i][j][t] = sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t] +
//_velo_v_ij[i][j][t] * _velo_v_ij[i][j][t]);
//corrector step
// _qp_ij[i][j][t] = ((_velo_ij[i][j][t] - _velo[i-l][j][t])*_lambda *
//_lambda/delX) - ((_press_ij[i][j][t] - _press_ij[i-l][j][t]) * (_press_ij[i][j][t]
//- _press_ij[i-l][j][t]))/((_velo_ij[i][j][t] - _velo[i-l][j][t]));
//_press_ij[i][j][t] =epsilon* (_qp_ij[i][j][t] -
//_qp_ij[i-l][j][t])/delX;
//_dens_ij[i][j][t] = (_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
//((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) - (_velo_v_ij[i-l][j][t]/delY)) - < velo_u_ij[i][j][t]) *
//(_velo_v_ij[i][j-l][t]/delX));
//DNS Turbulence model
//_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
//'(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
//_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
//(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) +
//(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)))
//k-e Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//] ) ) ;
//k-w Turbulence Model

// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
//])/t);
//	}
//	}
//if (t	== 2) {

//dens_ij[i][j][t-1] - dens[i][j][t-2] =?
//press_ij[i][j][t-1] - press[i][j][t-2] =?

/*
//Courant no= 0.5 ~ 0.95 Conmvergeance criteria
//dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
//visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
*/
//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
/*
if (leastValue <= 10e-6 * Courant_no) //Converged
{
bConverged = true; break;
}
else
continue; if (bConverged)
break;

else
continue;
*/

//for (int t = 0; t < 5000; t++) {
//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];
//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t]) {
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]

//retrieve dens[t,j]
//	press[t,j]

//for(t=0;
//	{

//u_ij[t,j]
//v_ij[t,j] new_ij[t,j] mew_ij[t,j] t<10000; t++

// dens_ij_N
// press_ij_N[t][j]

//Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity : Plotting & GRaph charts.
//}
//}
//}
}

void MainWidget::EulerSolverDiscreteKineticSystem()
{
//Computing flow: Inviscid, compressible flows - Euler Solver...Diagonal for as a discrete kinetic system of Relaxations
//Initial conditions on all points= sea level settings: density, velocity, temperature, viscosity, kinematic viscocity.
//time step= 0
//Boundary conditions: sea level settings: density, velocity, temperature, viscosity, kinematic viscosity

//Algorithms:
/* for (int t = 0; t < 10000; t++) {
// for(int j=0; j<noOfPointsin_j_drn; j++) {
//  for(int i=0; i<noOfPointsin_i_drn; i++) {
//predictor step
//density altitude= _dens_ij[i][j][t];
//Equation of state= _press_ij[i][j][t] = _dens_ij[i][j][t] * R *
_temp_ij[i][j][t]
//_velo_u_ij[i][j][t] = _velo_u_ij[i-l][j][t] - (1/lambda*lambda*epsilon) * (_velo_v_ij[i-l][j][t] - (_press_ij[i-l][j][t] -
_press_ij[i-2][j][t])/(_velo_ij[i-l][j][t] - _velo_ij[i-2][j][t]))
//_velo_v_ij[i][j][t] = _velo_v_ij[i-l][j][t]
//corrector step
//_press_ij[i][j][t] = (_press_ij[i - l][j][t] / delY) -
_dens_ij[i][j][t] * (_velo_v_ij[i][j][t] * ((_velo_v_ij[i][j][t] / delY) +
(_velo_u_ij[i][j][t] I delX) - (_velo_v_ij[i - l][j][t] / delY)) -
(_velo_u_ij[i][j][t]) * (_velo_v_ij[i][j - l][t] / delX))
//_dens_ij[i][j][t] =·(_press_ij[i-l][j][t]/delY - _press_ij[i][j][t]) /
((_velo_v_ij[i][j][t]) * ((_velo_v_ij[i][j][t]/delY) + (_velo_u_ij[i][j][t]/delX) -
(_velo_v_ij[i-l][j][t]/delY)) - (_velo_u_ij[i][j][t]) *
(_velo_v_ij[i][j-l][t]/delX));
//DNS Turbulence model
//_energy_ij[i][j][t] = ((_energy_ij[i-l][j][t]) *
(_dens_ij[i][j][t]*_velo_u_ij[i][j][t]/delX + _dens_ij[i][j][t] *
_velo_v[i][j][t]/delY)) - ((_velo_u_ij[i][j][t] * (_press_ij[i][j][t]/delX)) +
(_velo_v_ij[i][j][t] * (_press_ij[i][j][t]/delY))))/((_dens_ij[i][j][t]/delX) +
(_dens_ij[i][j][t]/delY) + ((_velo_u_ij[i][j][t] * _dens_ij[i][j][t]/delX) * (_velo_v_ij[i][j][t] * _dens_ij[i][j][t]/delY)))
//k-e Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * .9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t

] ) ) ;
//k-w Turbulence Model
// _energy_ij[i][j][t] = (0.5) * (_dens_ij[i][j][t] * 1 *
sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])) + (_dens_ij[i][j][t]) * 9.81 * altitude - (0.5) * (_dens_ij[i][j][t] * 1 * sqrt(_velo_u_ij[i][j][t]*_velo_u_ij[i][j][t]+_velo_v_ij[i][j][t]*_velo_v_ij[i][j][t
])/t);
//	}
//	}
if (t	== 2) {

//dens_ij[i][j][t-1] - dens[i][j][t-2] =?
//press_ij[i][j][t-1] - press[i][j][t-2] =?

/*
Courant no= 0.5 ~ 0.95 Conmvergeance criteria
dens_ij[l] - dens_ij[0] = del_dens; press_ij[l] - press_ij[0] = del_press; temp_ij[l] - temp_ij[0] = del_temp; visc_ij[l] - visc_ij[0] = del_visc;
visc_kinematic_ij[l] - visc_kinematic_ij[0] = del_visc_kinematic;
*/

//SelectionSort(del_dens, del_press, del_temp, del_visc, del_visc_kinematic); make an array
/*
if (leastValue <= 10e-6 * Courant_no) //Converged
{
bConverged = true; break;
}
else
continue; if (bConverged)
break;
else
continue;
*/

//for (int t = 0; t < 5000; t++) {
//dd_dens[t] = del_dens[2] - del_dens[l];
//dd_press[t] = del_press[3] = del_press[2];
//Least values: dd_dens[2] - dd_dens[l] =?
//Convergeance factor

//if (dd_dens[t]) {
//least value=> C.F <= 10e-6 * courant no => converged
//get index[t*2] - index[t*2-1]

//retrieve dens[t,j]
//	press[t,j]
//	u_ij[t,j]
//	v_ij[t,j]
//	new_ij[t,j]
//	mew_ij[t,j]
//for(t=0; t<10000; t++
//	{
// dens_ij_N
// press_ij_N[t][j]

//Conclude outputs, End, Pressure, Density, Temperature, Viscosity, Kinematic Viscosity	Plotting & GRaph charts.
//}
//}
//}
}

void MainWidget::AdaptiveGrid()
{
//Adaptive Structured grid...
//11 points on upper surface and 11 points on lower surface....Pu0-Pu10 && Pl0-Pl10
//Make polynomial of Upper surface and Lower surface : Pu0-Pu10 and Pl0-Pl10//////////////////////////////////////////////////////
//y=an.xn+an-1.xn-l+alxl+aoxo+K find the values of an-ao & Kand find the polynomial
//similarly find the polynomial for lower surface...
//Use K = 100 and create the boundary outer surafce points 11 points and n = 10.
//For upper surface
//Use y = axA3 + b.xA2 +ex+ d and dy/dx = 3axA2+2bx+c
//smoothening factor 0-1 use 0.5 or 0.1
//Put dy/dx = 0.5 and find all x values for all yl-y0/xl-x0 find new ym for new xm
/* int m = 0;
for (inti= 0; i < n; i++) {

//find dy/dx = 0.5 for each x[i+l]-x[i]/2.0 = xm
//find ym[i] = for xm
//new List of x and y=> x = x[m] and x[m+l]
m += 2;
}
i = 0;
n = m - 1;
p = 0, q = 0;
do {

//p[i + 1] ~p[i] intersection point=> q[i]; //n points
//make a triangle p[i+l], p[i] & q[i]
//if q[i] satisfies the airfoil polynomial equation then discard it and freeze further marching.
i++;
if (i	== n) {
for (int j = 0; j < n; j++) { p[j] = q[j];
}
n--;
//Solve Euler/Ns equations for n=n and n=n-1;
//if (it is> solution convergeance factor) {
//split all 0-n+l points on upper surface and lower surface

//Get non upper and non lower mid-points total 2*n+l
//Put n=m and P[n+l] points index from 0 ton
n = n * 2;
continue;

} while (n >= 3);
//similarly for lower surface.
//Adaptive Unstructured
//For upper surface
i = 0;
n = 10;
p=0, q=0;
do {

//p[i + 1] ~p[i] intersection point=> q[i]; //10 points
//make a triangle p[i+l], p[i] & q[i]
//if q[i] satisfies the airfoil polynomial equation then discard it and freeze further marching.
i++;
if (i == n) {
for(int j=0; j<n; j++) {
p[j] = q[j];
}
i=0;
n--;
//Solve Euler/NS equations for n=n and n=n-1;
//if(it is > solution convergeance factor) {
//split all 0-11 or 11 points on upper surface and lower surface 11 points by 2.
//Get 10 on upper and 10 on lower mid-points total 21  points realign=>
//Put n = 20 and P[21] points index from 0 to 20

n=20;
continue;
}*/
//}while(n>=3);
}

void MainWidget::drawFuselage()
{
    angles[5] = (0, 45, 90, 135, 180);
    //Drawing fuselage
    ctrlX = new GLfloat[6]; ctrlY = new GLfloat[6]; ctrlZ = new GLfloat[6];
    ctrlX[6] = (0, 30, 100, 0, 30, 0);
    ctrlY[6] = (0, 15, 0,  0, -15, 0);
    ctrlZ[6] = (0, 0,  0,  0, 0, 0);
    fuslgX = new GLfloat*[9];
    fuslgY = new GLfloat*[9];
    fuslgZ = new GLfloat*[9];
    float theta = 0.0;

        for(int j=0; j<5; j++) {
            fuslgX[j] = new float[6];
            fuslgY[j] = new float[6];
            fuslgZ[j] = new float[6];
            for(int i=0; i<6; i++)
            {
               fuslgX[i][j] = ctrlX[i];
               fuslgY[i][j] = ctrlY[i] * sin((pi/180.0)*theta);
               fuslgZ[i][j] = ctrlZ[i] * cos((pi/180.0)*theta);
            }
            theta += 45.0;
        }
}

void MainWidget::drawWing()
{
    float chord = 100;
    float maxThickness = 30;
    float maxThicknessPos = 30;
    float camber  = 7;
    float intervalX = chord/10;
    //Total no of points...
    int N = 11;
    int NbreakLE = 4;
    float cambInterLE = camber/(N-(N-NbreakLE));
    float cambInterTE = camber/(N-NbreakLE);
    float *xCamber = new float[N];
    float *yCamber = new float[N];

    float *xAirfoilu = new float[N];
    float *yAirfoilu = new float[N];

    float *xAirfoill = new float[N];
    float *yAirfoill = new float[N];
    //drawing camber
    for(int i=0; i<(N); i++)
    {
        if(i<(N-(N-NbreakLE)))
        {
            xCamber[i] = intervalX * i;
            yCamber[i] = cambInterLE * i;
            //airfoil points
            if(i==0) {
            xAirfoilu[i] = xCamber[i];
            yAirfoilu[i] = yCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                yAirfoilu[i] = yCamber[i] + (maxThickness/2.0);
            }
        }
        else if(i>=NbreakLE)
        {
            xCamber[i] = intervalX * i;
            yCamber[i] = camber - cambInterTE * (i-NbreakLE);

            if(i==(N-1)) {
            xAirfoilu[i] = xCamber[i];
            yAirfoilu[i] = yCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                yAirfoilu[i] = yCamber[i] + (maxThickness*.5);
            }
        }
    }

    //Lower surface...
    for(int i=0; i<N; i++)
    {
        if(i<(NbreakLE))
        {
            //airfoil points
            if(i==0) {
            xAirfoill[i] = xCamber[i];
            yAirfoill[i] = yCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                yAirfoill[i] = yCamber[i] - (maxThickness/2.0);
            }
        }
        else if(i>=NbreakLE)
        {
            if(i==(N-1)) {
            xAirfoill[i] = xCamber[i];
            yAirfoill[i] = yCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                yAirfoill[i] = yCamber[i] - (maxThickness*.5);
            }
        }
    }

    ofstream out("Airfoildata.txt");

    for(int i=0; i<N; i++)
        out << xAirfoilu[i] << " " << yAirfoilu[i] << " " << yAirfoill[i] << "\n";
}

void MainWidget::drawTail()
{
    int N = 11;
    float chord = 100;
    float maxThickness = 30;
    float camber  = 7;
    float intervalX = chord/(N-1);
    int NbreakLE = 4; //==30% maximum camber & maximum thickness position

    //Total no of points...
    float cambInterLE = camber/(N-(N-NbreakLE));
    float cambInterTE = camber/(N-NbreakLE);
    float *xCamber = new float[N];
    float *yCamber = new float[N];

    float *xAirfoilu = new float[N];
    float *yAirfoilu = new float[N];

    float *xAirfoill = new float[N];
    float *yAirfoill = new float[N];
    //drawing camber
    for(int i=0; i<(N); i++)
    {
        if(i<(N-(N-NbreakLE)))
        {
            xCamber[i] = intervalX * i;
            yCamber[i] = cambInterLE * i;
            //airfoil points
            if(i==0) {
            xAirfoilu[i] = xCamber[i];
            yAirfoilu[i] = yCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                yAirfoilu[i] = yCamber[i] + (maxThickness/2.0);
            }
        }
        else if(i>=NbreakLE)
        {
            xCamber[i] = intervalX * i;
            yCamber[i] = camber - cambInterTE * (i-NbreakLE);

            if(i==(N-1)) {
            xAirfoilu[i] = xCamber[i];
            yAirfoilu[i] = yCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                yAirfoilu[i] = yCamber[i] + (maxThickness*.5);
            }
        }
    }

    //Lower surface...
    for(int i=0; i<N; i++)
    {
        if(i<(NbreakLE))
        {
            //airfoil points
            if(i==0) {
            xAirfoill[i] = xCamber[i];
            yAirfoill[i] = yCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                yAirfoill[i] = yCamber[i] - (maxThickness*.5);
            }
        }
        else if(i>=NbreakLE)
        {
            if(i==(N-1)) {
            xAirfoill[i] = xCamber[i];
            yAirfoill[i] = yCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                yAirfoill[i] = yCamber[i] - (maxThickness*.5);
            }
        }
    }

    ofstream out("Taildata.txt");

    for(int i=0; i<N; i++)
        out << xAirfoilu[i] << " " << yAirfoilu[i] << " " << yAirfoill[i] << "\n";
}


void MainWidget::drawFin()
{
    int N = 11;
    float chord = 100;
    float maxThickness = 30;
    float camber  = 7;
    float intervalX = chord/(N-1);
    int NbreakLE = 4; //==30% maximum camber & maximum thickness position

    //Total no of points...
    float cambInterLE = camber/(N-(N-NbreakLE));
    float cambInterTE = camber/(N-NbreakLE);
    float *xCamber = new float[N];
    float *zCamber = new float[N];

    float *xAirfoilu = new float[N];
    float *zAirfoilu = new float[N];

    float *xAirfoill = new float[N];
    float *zAirfoill = new float[N];
    //drawing camber
    for(int i=0; i<(N); i++)
    {
        if(i<(N-(N-NbreakLE)))
        {
            xCamber[i] = intervalX * i;
            zCamber[i] = cambInterLE * i;
            //airfoil points
            if(i==0) {
            xAirfoilu[i] = xCamber[i];
            zAirfoilu[i] = zCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                zAirfoilu[i] = zCamber[i] + (maxThickness/2.0);
            }
        }
        else if(i>=NbreakLE)
        {
            xCamber[i] = intervalX * i;
            zCamber[i] = camber - cambInterTE * (i-NbreakLE);

            if(i==(N-1)) {
            xAirfoilu[i] = xCamber[i];
            zAirfoilu[i] = zCamber[i];
            }
            else
            {
                xAirfoilu[i] = xCamber[i];
                zAirfoilu[i] = zCamber[i] + (maxThickness*.5);
            }
        }
    }

    //Lower surface...
    for(int i=0; i<N; i++)
    {
        if(i<(NbreakLE))
        {
            //airfoil points
            if(i==0) {
            xAirfoill[i] = xCamber[i];
            zAirfoill[i] = zCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                zAirfoill[i] = zCamber[i] - (maxThickness*.5);
            }
        }
        else if(i>=NbreakLE)
        {
            if(i==(N-1)) {
            xAirfoill[i] = xCamber[i];
            zAirfoill[i] = zCamber[i];
            }
            else
            {
                xAirfoill[i] = xCamber[i];
                zAirfoill[i] = zCamber[i] - (maxThickness*.5);
            }
        }
    }

    ofstream out("Findata.txt");

    for(int i=0; i<N; i++)
        out << xAirfoilu[i] << " " << zAirfoilu[i] << " " << zAirfoill[i] << "\n";
}


void MainWidget::makeCube()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
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
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]
//!
}

void MainWidget::makeCheckImage(void)
{
    /*
    int i, j, c;

    for(i=0; i<checkImageHeight; i++) {
        for(j=0; j<checkImageWidth; j++)
        {
            c = ((((i&0x8==0)^((j&0x8)))==0))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
    */
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    drawWing();
}

void MainWidget::idleFunc()
{
    QVector2D diff = QVector2D(5,-3);
    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;

    update();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
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
    angularSpeed += acc;
}

void MainWidget::timerEvent(QTimerEvent *)
{

//    Decrease angular speed (friction)
      //angularSpeed *= 0.99;
      angularSpeed = 3.99;
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
      rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
      // Request an update
      update();

 /*   m_rot[0] =  15.50f;
    m_rot[1] = -15.50f;
    //m_rot[0] += .009;
    //m_rot[1] -= .009;
    #define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
    clamp(m_rot[0]);
    clamp(m_rot[1]);
    update(); */

//! [1]
}

/* Draw a torus */
void MainWidget::torus(int numc, int numt)
{
   int i, j, k;
   double s, t, x, y, z, twopi;

   twopi = 2 * (double)(4.0*atan(1.0));
   for (i = 0; i < numc; i++) {
      //glBegin(GL_QUAD_STRIP);
      for (j = 0; j <= numt; j++) {
         for (k = 1; k >= 0; k--) {
            s = (i + k) % numc + 0.5;
            t = j % numt;

            x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
            y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
            z = .1 * sin(s * twopi / numc);
            //glVertex3f(x, y, z);
         }
      }
      //glEnd();
   }
}

void MainWidget::LoadGLTextures( const char * name )
{
    QImage img;

    if(!img.load("cube.png")){
        //std::cerr << "ERROR in loading image" << std::endl;
    }
/*
    QImage t = QGLWidget::convertToGLFormat(img);
    int wInter = t.width();
    int hInter = t.height();
    int n = 0;
    glGenTextures(6, texName);

    for(int j=0; j<2; j++)
    {
        for(int i=0; i<3; i++)
        {
            n = i*j+i+j;
            glBindTexture(GL_TEXTURE_2D, texName[n]);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, wInter, hInter, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glBindTexture( GL_TEXTURE_2D, 0 );

       }
        n++;
    }

    */
}


void MainWidget::initializeGL()
{
    //initializeOpenGLFunctions();
    // Generate 2 VBOs
    //arrayBuf.create();
    //indexBuf.create();

    //drawCube();

    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glShadeModel(GL_FLAT);


    //initShaders();
    //initTextures();

//! [2]
    // Enable depth buffer
    //glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);


    //LoadGLTextures("cube.png");

/*  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
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
void MainWidget::initShaders()
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
void MainWidget::initTextures()
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
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    /* qreal aspect = qreal(w) / qreal(h ? h : 1);
    glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    //glOrtho(-5.0, 5.0, 5.0, -5.0, -5.0, 5.0);
    gluPerspective(27.0, aspect, 1.0, 100.0);
    gluLookAt(0, 0, -10.0, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); */

    // Calculate aspect ratio
    //qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    //const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    //projection.setToIdentity();

    // Set perspective projection
    //projection.perspective(fov, aspect, zNear, zFar);

    //timerEvent(nullptr);
    //update();
}

//! [5]
void MainWidget::drawCube()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    vertices =new GLfloat[72] {
        // Vertex data for face 0
        (-3.0f, -3.0f,  3.0f),  // v0
        ( 3.0f, -3.0f,  3.0f), // v1
        (-3.0f,  3.0f,  3.0f),  // v2
       ( 3.0f,  3.0f,  3.0f), // v3

        // Vertex data for face 1
       ( 3.0f, -3.0f,  3.0f), // v4
      ( 3.0f, -3.0f, -3.0f), // v5
     ( 3.0f,  3.0f,  3.0f), // v6
    ( 3.0f,  3.0f, -3.0f), // v7

        // Vertex data for face 2
   ( 3.0f, -3.0f, -3.0f), // v8
  (-3.0f, -3.0f, -3.0f), // v9
  ( 3.0f,  3.0f, -3.0f), // v10
   (-3.0f,  3.0f, -3.0f), // v11

        // Vertex data for face 3
(-3.0f, -3.0f, -3.0f), // v12
     (-3.0f, -3.0f,  3.0f), // v13
      (-3.0f,  3.0f, -3.0f), // v14
   (-3.0f,  3.0f,  3.0f), // v15

        // Vertex data for face 4
  (-3.0f, -3.0f, -3.0f), // v16
    ( 3.0f, -3.0f, -3.0f), // v17
   (-3.0f, -3.0f,  3.0f), // v18
    ( 3.0f, -3.0f,  3.0f), // v19

        // Vertex data for face 5
  (-3.0f,  3.0f,  3.0f), // v20
   ( 3.0f,  3.0f,  3.0f), // v21
     (-3.0f,  3.0f, -3.0f), // v22
  ( 3.0f,  3.0f, -3.0f), // v23
    };

   indices = new GLint[24] {

        0,1,2,
        2,0,3,
        3,4,2,
        2,5,4,
        4,7,5,
        5,6,7,
        7,0,1,
        1,6,7

    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
   colors = new GLfloat [72] {
         0.0, 1.0, 0.0 ,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         0.0, 1.0, 0.0, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         0.0, 1.0, 0.0, // 0Face 2 - triangle strip ( v8,  v9, v10, v11)
        0.0, 1.0, 0.0, // Face 3 - triangle strip (v12, v13, v14, v15)
        0.0, 1.0, 0.0, // Face 4 - triangle strip (v16, v17, v18, v19)
        0.0, 1.0, 0.0 ,     // Face 5 - triangle strip (v20, v21, v22, v23)
           0.0, 1.0, 0.0 ,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
           0.0, 1.0, 0.0, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
           0.0, 1.0, 0.0, // 0Face 2 - triangle strip ( v8,  v9, v10, v11)
          0.0, 1.0, 0.0, // Face 3 - triangle strip (v12, v13, v14, v15)
          0.0, 1.0, 0.0, // Face 4 - triangle strip (v16, v17, v18, v19)
          0.0, 1.0, 0.0   ,   // Face 5 - triangle strip (v20, v21, v22, v23)
           0.0, 1.0, 0.0 ,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
           0.0, 1.0, 0.0, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
           0.0, 1.0, 0.0, // 0Face 2 - triangle strip ( v8,  v9, v10, v11)
          0.0, 1.0, 0.0, // Face 3 - triangle strip (v12, v13, v14, v15)
          0.0, 1.0, 0.0, // Face 4 - triangle strip (v16, v17, v18, v19)
          0.0, 1.0, 0.0 ,     // Face 5 - triangle strip (v20, v21, v22, v23)
             0.0, 1.0, 0.0 ,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
             0.0, 1.0, 0.0, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
             0.0, 1.0, 0.0, // 0Face 2 - triangle strip ( v8,  v9, v10, v11)
            0.0, 1.0, 0.0, // Face 3 - triangle strip (v12, v13, v14, v15)
            0.0, 1.0, 0.0, // Face 4 - triangle strip (v16, v17, v18, v19)
            0.0, 1.0, 0.0   ,   // Face 5 - triangle strip (v20, v21, v22, v23)
    };
   //theTorus = glGenLists (1);
   //glNewList(theTorus, GL_COMPILE);
   //torus(8, 25);
   //glEndList();

   //glEnableClientState(GL_VERTEX_ARRAY);
   //glEnableClientState(GL_NORMAL_ARRAY);
   //glEnableClientState(GL_COLOR_ARRAY);
   //glVertexPointer(3,GL_FLOAT,72*sizeof(GLfloat),vertices);
   //glColorPointer (3,GL_FLOAT,72*sizeof(GLfloat),colors);
   //glDrawArrays(GL_TRIANGLES, 0, *indices);
   //glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, 0);
   //glDisableClientState(GL_COLOR_ARRAY);
   //glDisableClientState(GL_NORMAL_ARRAY);
   //glDisableClientState(GL_VERTEX_ARRAY);
}

//Drawing default cube...1st GL Window...
void MainWidget::paintGL()
{
    
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_TEXTURE_2D);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //glBindTexture(GL_TEXTURE_2D, *texName);

    //texture->bind();


    // Calculate model view transformation
    //QMatrix4x4 matrix;
    //matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(m_rot[0], 1.0, 0.0, 0.0);
    //matrix.rotate(m_rot[0], 0.0, 1.0, 0.0);
    //matrix.rotate(rotation);

    // Set modelview-projection matrix
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
    //glRotatef(m_rot[0], 1.0, 0.0, 0.0);
    //glRotatef(m_rot[1], 0.0, 1.0, 0.0);
    //object = glGenLists(1);
    //glNewList(object, GL_COMPILE);

    //front view
    //glBindTexture(GL_TEXTURE_2D, texName[0]);
   /* glBegin(GL_QUADS);
    //glColor3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.0f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(3.0, 3.0, -3.0);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-3.0, 3.0, -3.0);
    glEnd();

    //right view
    //glBindTexture(GL_TEXTURE_2D, texName[1]);
    glBegin(GL_QUADS);
    //glColor3f(0.0, 0.5, 0.0);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.33f, 1.0f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0, 3.0, -3.0);
    glEnd();


    //rear view
    //glBindTexture(GL_TEXTURE_2D, texName[2]);
    glBegin(GL_QUADS);
    //glColor3f(0.5, 0.0, 0.0);
    glTexCoord2f(0.33f, 0.0f); glVertex3f(-3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.0f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(0.33f, 0.5f); glVertex3f(-3.0, 3.0, 3.0);
    glEnd();


    //left view
    //glBindTexture(GL_TEXTURE_2D, texName[3]);
    glBegin(GL_QUADS);
    //glColor3f(0.0, 0.0, 1.0);
    glTexCoord2f(.33, .5); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(.33, 1.0f); glVertex3f(-3.0, -3.0, 3.0);
    glTexCoord2f(.66f, 1.0f); glVertex3f(-3.0, 3.0, 3.0);
    glTexCoord2f(.66f, .5f); glVertex3f(-3.0, 3.0, -3.0);
    glEnd();

    //bottom view
    //glBindTexture(GL_TEXTURE_2D, texName[4]);
    glBegin(GL_QUADS);
    //glColor3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.66f, 0.0f); glVertex3f(-3.0, -3.0, -3.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0, -3.0, -3.0);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(3.0, -3.0, 3.0);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(-3.0, -3.0, 3.0);
    glEnd();

    //top view
    //glBindTexture(GL_TEXTURE_2D, texName[5]);
    glBegin(GL_QUADS);
    //glColor3f(0.0, 0.0, 0.5);
    glTexCoord2f(0.66f, 0.5f); glVertex3f(-3.0, 3.0, -3.0);
    glTexCoord2f(0.66f, 1.0f); glVertex3f(3.0, 3.0, -3.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0, 3.0, 3.0);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-3.0, 3.0, 3.0);
    glEnd();


    //glEndList();
    //glCallList(object);
    glFlush();
    glDisable(GL_TEXTURE_2D);
    */
    // Tell OpenGL which VBOs to use
    //arrayBuf.bind();
    //indexBuf.bind();

    // Offset for position
    //quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    //int vertexLocation = program.attributeLocation("a_position");
    //program.enableAttributeArray(vertexLocation);
    //program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    //offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    //int texcoordLocation = program.attributeLocation("a_texcoord");
    //program.enableAttributeArray(texcoordLocation);
    //program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    //glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    //glFlush();
    
    //rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    //update();
}
