#include "dialogterrain.h"
#include "ui_dialogterrain.h"
#include <QFileDialog>
#include <QTextStream>
#include <math.h>
#include <QSplitter>
#include <QMessageBox>
int DialogTerrain::i = 0;
int DialogTerrain::N = 0;
DialogTerrain::DialogTerrain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTerrain),
    xu(NULL),
    yu(NULL),
    zu(NULL),
    xl(NULL),
    yl(NULL),
    zl(NULL),
    xu_(NULL),
    yu_(NULL),
    zu_(NULL),
    xl_(NULL),
    yl_(NULL),
    zl_(NULL),
    xAnchorU(NULL),
    yAnchorU(NULL),
    zAnchorU(NULL),
    xAnchorL(NULL),
    yAnchorL(NULL),
    zAnchorL(NULL),
    xAnchorU_(NULL),
    yAnchorU_(NULL),
    zAnchorU_(NULL),
    xAnchorL_(NULL),
    yAnchorL_(NULL),
    zAnchorL_(NULL)
{


    ui->setupUi(this);
    connect(ui->pushButton_READ_ANCHORS, SIGNAL(clicked()), this, SLOT(push_read_anchors()));
    connect(ui->pushButton_APPLY_READ_ANCHORS, SIGNAL(clicked()), this, SLOT(push_apply_anchors()));
    connect(ui->pushButton_APPLY_FULL, SIGNAL(clicked()), this, SLOT(push_apply_full()));
    connect(ui->pushButton_IMPORT_STL, SIGNAL(clicked()), this, SLOT(_import_stl()));
    connect(ui->pushButton_EXPORT_STL, SIGNAL(clicked()), this, SLOT(_export_stl()));
    connect(ui->pushButton_IMPORT_OBJ, SIGNAL(clicked()), this, SLOT(_import_obj()));
    connect(ui->pushButton_EXPORT_OBJ, SIGNAL(clicked()), this, SLOT(_export_obj()));
    connect(ui->pushButton_IMPORT_IGES, SIGNAL(clicked()), this, SLOT(_import_iges()));
    connect(ui->pushButton_EXPORT_IGES_2, SIGNAL(clicked()), this, SLOT(_export_iges()));
    connect(ui->pushButton_IMPORT_STEP, SIGNAL(clicked()), this, SLOT(_import_step()));
    connect(ui->pushButton_EXPORT_STEP, SIGNAL(clicked()), this, SLOT(_export_step()));

    connect(ui->pushButton_SHOW_HIDE_GRID, SIGNAL(clicked()), this, SLOT(hyperbolic_grid_show_hide()));
    connect(ui->pushButton_ANCHOR_SHOW_HIDE, SIGNAL(clicked()), this, SLOT(anchor_show_hide()));
    connect(ui->pushButton_BEZIER_SHOW_HIDE, SIGNAL(clicked()), this, SLOT(bezier_show_hide()));

    connect(ui->pushButton_DRAW_H_GRID, SIGNAL(clicked()), this, SLOT(drawHGrid()));
    connect(ui->pushButton_DRAW_FUSELAGE, SIGNAL(clicked()), this, SLOT(drawFuselage()));

    //TCS, Wipro, Cognizant, Genpact, Capgemini, Tech Mahindra...



    cairfoil_designer = new CAirfoilDesigner;
    ui->scrollArea_GRAPHICS_WINDOW->setWidget(cairfoil_designer);
    QSplitter *splitter1 = new QSplitter;
    splitter1->addWidget(ui->scrollArea_CONTROL_WIDGET);
    splitter1->addWidget(ui->widget_SCREEN);
    /////////////////////////////////////////////////////////////////////////////////////

    QSplitter *splitter2 = new QSplitter;
    splitter2->setOrientation(Qt::Vertical);
    splitter2->addWidget(ui->scrollArea_GRAPHICS_WINDOW);
    splitter2->addWidget(ui->textEdit_CONSOLE_WINDOW);
    ui->verticalLayout->addWidget(splitter2);
    ui->horizontalLayout->addWidget(splitter1);
    N = -1;
}

DialogTerrain::~DialogTerrain()
{
    delete ui;
}

void DialogTerrain::drawStarBoardWing()
{
    //nKnots, xu_, yu_, zu, xl_, yl_, zl

    int taperRatio = ui->lineEdit_ENTER_TAPER_RATIO->text().toFloat();
    float *xu_tip = new float[nKnots], *yu_tip = new float[nKnots], *zu_tip = new float[nKnots];
    float *xl_tip = new float[nKnots], *yl_tip = new float[nKnots], *zl_tip = new float[nKnots];

    for(int i=0; i<nKnots; i++)
    {
        xu_tip[i] = taperRatio * xu_[i];
        yu_tip[i] = taperRatio * yu_[i];
        zu_tip[i] = taperRatio * zu[i] * (10.0);
        xl_tip[i] = taperRatio * xl_[i];
        yl_tip[i] = taperRatio * yl_[i];
        zl_tip[i] = taperRatio * zl[i] * (10.0);
    }

    cairfoil_designer->draw_star_wing(nKnots, xu_tip, yu_tip, zu_tip, xl_tip, yl_tip, zl_tip);
}

void DialogTerrain::drawPortWing()
{
    int taperRatio = ui->lineEdit_ENTER_TAPER_RATIO->text().toFloat();
    float *xu_tip = new float[nKnots], *yu_tip = new float[nKnots], *zu_tip = new float[nKnots];
    float *xl_tip = new float[nKnots], *yl_tip = new float[nKnots], *zl_tip = new float[nKnots];

    for(int i=0; i<nKnots; i++)
    {
        xu_tip[i] = taperRatio * xu_[i];
        yu_tip[i] = taperRatio * yu_[i];
        zu_tip[i] = taperRatio * zu[i] * (-10.0);
        xl_tip[i] = taperRatio * xl_[i];
        yl_tip[i] = taperRatio * yl_[i];
        zl_tip[i] = taperRatio * zl[i] * (-10.0);
    }


    cairfoil_designer->draw_port_wing(nKnots, xu_tip, yu_tip, zu_tip, xl_tip, yl_tip, zl_tip);
}

void DialogTerrain::drawFuselage()
{
    int n = ui->lineEdit_NUMBER_OF_SLICES->text().toInt();
    float **xright = new float*[n+1], **yright = new float*[n+1], **zright = new float*[n+1];
    float **xleft = new float*[n+1], **yleft = new float*[n+1], **zleft = new float*[n+1];
    float theta = 0.0;
    float pi = 4.0 * atan(1.0);
    //right surfaces
    for(int i=0; i<n+1; i++)
    {
        xright[i] = new float[nKnots];
        yright[i] = new float[nKnots];
        zright[i] = new float[nKnots];

        for(int j=0; j<nKnots; j++) {
            xright[i][j] = xu_[j];
            yright[i][j] = yu_[j] * cos(theta);
            zright[i][j] = zu[j]  * sin(theta);
        }

        theta += pi/n;
    }

    //left surfaces
    theta = 360;
    for(int i=0; i<n+1; i++)
    {
        xleft[i] = new float[nKnots];
        yleft[i] = new float[nKnots];
        zleft[i] = new float[nKnots];

        for(int j=0; j<nKnots; j++) {
        xleft[i][j] = xl_[i];
        yleft[i][j] = yl_[i] * cos(theta);
        zleft[i][j] = zl[i] * sin(theta);
        }
        theta -= pi/n;
    }

    cairfoil_designer->drawFuselage(nKnots, n, xright, yright, zright, xleft, yleft, zleft);
}

void DialogTerrain::fuselage_show_hide()
{
    cairfoil_designer->set_ifDrawFuselage();
}

void DialogTerrain::bezier_show_hide()
{
    cairfoil_designer->set_ifDrawKnots();
}

void DialogTerrain::anchor_show_hide()
{
    cairfoil_designer->set_ifDrawAnchors();
}

void DialogTerrain::hyperbolic_grid_show_hide()
{
    cairfoil_designer->set_ifDrawHGrid();
}

void DialogTerrain::_import_stl()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Import STL"),
                                                     ".",
                                                     ("DataSTL (*.stl)"));
}
void DialogTerrain::_export_stl()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Export STL"),
                                                     ".",
                                                     ("DataSTL (*.stl)"));
}
void DialogTerrain::_import_obj()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Import OBJ"),
                                                     ".",
                                                     ("DataOBJ (*.obj)"));
}
void DialogTerrain::_export_obj()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Export OBJ"),
                                                     ".",
                                                     ("DataOBJ (*.obj)"));
}
void DialogTerrain::_import_iges()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Import IGES"),
                                                     ".",
                                                     ("DataIGES (*.iges)"));
}
void DialogTerrain::_export_iges()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Export IGES"),
                                                     ".",
                                                     ("DataIGES (*.iges)"));
}
void DialogTerrain::_import_step()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Import STEP"),
                                                     ".",
                                                     ("DataSTEP (*.step)"));
}
void DialogTerrain::_export_step()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Export STEP"),
                                                     ".",
                                                     ("DataSTEP (*.step)"));
}


void DialogTerrain::drawHGrid()
{

    cairfoil_designer->generateGridOverAirfoil(ui->lineEdit_nHGRID->text().toFloat());
}

void DialogTerrain::push_apply_full()
{
    float chord = ui->lineEdit_CHORD_LENGTH->text().toFloat();
    float camber = ui->lineEdit_CAMBER->text().toFloat();
    float maxThickness = ui->lineEdit_MAX_THICKNESS->text().toFloat();
    int order = ui->lineEdit_ORDER->text().toInt();
    nKnots = ui->lineEdit_NO_KNOTS->text().toInt();
    float max_thickness_pos = ui->lineEdit_MAX_THICKNESS_POS->text().toFloat();

    if(N==-1) {
    N = 3;
    if(xAnchorU) {delete [] xAnchorU; xAnchorU=nullptr;}
    if(yAnchorU) {delete [] yAnchorU; yAnchorU=nullptr;}
    if(zAnchorU) {delete [] zAnchorU; zAnchorU=nullptr;}
    if(xAnchorL) {delete [] xAnchorL; xAnchorL=nullptr;}
    if(yAnchorL) {delete [] yAnchorL; yAnchorL=nullptr;}
    if(zAnchorL) {delete [] zAnchorL; zAnchorL=nullptr;}

    ui->textEdit_CONSOLE_WINDOW->append(QString::number(N) + "\n");
    xAnchorU = new float[N];
    yAnchorU = new float[N];
    zAnchorU = new float[N];

    xAnchorL = new float[N];
    yAnchorL = new float[N];
    zAnchorL = new float[N];


    xAnchorU_ = new float[N];
    yAnchorU_ = new float[N];
    zAnchorU_ = new float[N];

    xAnchorL_ = new float[N];
    yAnchorL_ = new float[N];
    zAnchorL_ = new float[N];
    xMin = 0.0;
    xMax = chord;
    yMax =  maxThickness/2.0 + camber;
    yMin = -(maxThickness/2.0) + camber;
                xAnchorU[0] = 0.0;
                yAnchorU[0] = 0.0;
                zAnchorU[0] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorU[0]) + " " + QString::number(yAnchorU[0]) + " " + QString::number(zAnchorU[0]) + "\n");

                xAnchorU[1] = max_thickness_pos;
                yAnchorU[1] = (camber + (maxThickness/2.0));
                zAnchorU[1] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorU[1]) + " " + QString::number(yAnchorU[1]) + " " + QString::number(zAnchorU[1]) + "\n");

                xAnchorU[2] = chord;
                yAnchorU[2] = 0.0;
                zAnchorU[2] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorU[2]) + " " + QString::number(yAnchorU[2]) + " " + QString::number(zAnchorU[2]) + "\n");


                xAnchorL[0] = 0.0;
                yAnchorL[0] = 0.0;
                zAnchorL[0] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorL[0]) + " " + QString::number(yAnchorL[0]) + " " + QString::number(zAnchorL[0]) + "\n");

                xAnchorL[1] = max_thickness_pos;
                yAnchorL[1] = (camber - (maxThickness/2.0));
                zAnchorL[1] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorL[1]) + " " + QString::number(yAnchorL[1]) + " " + QString::number(zAnchorL[1]) + "\n");

                xAnchorL[2] = chord;
                yAnchorL[2] = 0.0;
                zAnchorL[2] = 0.0;
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorL[2]) + " " + QString::number(yAnchorL[2]) + " " + QString::number(zAnchorL[2]) + "\n");

                for(i=0; i<N; i++)
                {

                    xAnchorU_[i] = xAnchorU[N-1-i];
                    yAnchorU_[i] = yAnchorU[N-1-i];
                    zAnchorU_[i] = zAnchorU[N-1-i];

                    xAnchorL_[i] = xAnchorL[N-1-i];
                    yAnchorL_[i] = yAnchorL[N-1-i];
                    zAnchorL_[i] = zAnchorL[N-1-i];
                }

                for(i=0; i<N; i++)
                {
                    yAnchorU[i] *= -1.0;
                    yAnchorL[i] *= -1.0;
                }



                /////////////////////////////////////////////////////////////////////////////
                //Enter nKnots, order
                float *t = new float[nKnots];
                float inter_t = 1.0/((float)nKnots-1);
                for(i=0; i<nKnots; i++)
                {
                    if(i==0)
                        t[i] = 0;
                    else
                        t[i] = t[i-1] + inter_t;
                }

                int *k = new int[N]; //3 values
                for(int m=0; m<N; m++)
                    k[m] = m+1;
                float *nCk = new float[N]; //3 values

                for(i=0; i<N; i++)
                {
                    float n1 = fact(N);
                    float n2 = fact(N-k[i]);
                    float n3 = fact(k[i]);
                    nCk[i] = n1/(n2*n3);
                }
///////////////////////////////////////////////////////////////////////////

    if(xu) {delete [] xu; xu = nullptr;}  if(xu_) {delete [] xu_; xu_ = nullptr;}
    if(yu) {delete [] yu; yu = nullptr;}  if(yu_) {delete [] yu_; yu_ = nullptr;}
    if(zu) {delete [] zu; zu = nullptr;}  if(zu_) {delete [] yu_; zu_ = nullptr;}


////////////////////////////////////////////////////////////////////////////////


    //Upper surfaces
                ///////////////////////////////////////////////////////////////////////////////////////////
                    xu = new  float[nKnots], xu_ = new  float[nKnots];
                    yu = new  float[nKnots], yu_ = new  float[nKnots];
                    zu = new  float[nKnots], zu_ = new  float[nKnots];
                    float basex = 0; float basey = 0; float basez=0;

                    ui->textEdit_CONSOLE_WINDOW->append("Airfoil points-Upper:\n");

                    for(i=0; i<nKnots; i++)
                    {
                        basex = 0, basey=0, basez=0;
                        xu[i] = 0.0, yu[i] = 0.0, zu[i] = 0.0;
                        for(int j=0; j<N; j++)
                        {
                            basex += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * xAnchorU[j];
                            basey += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * yAnchorU[j];
                            //basez += 0;

                            xu[i] +=  basex;
                            yu[i] +=  basey;
                            zu[i] +=  0.0; //basez * pow(1-t[i],(N-1)-k[i]) * pow(t[i],k[i]);
                        }
                        ui->textEdit_CONSOLE_WINDOW->append(QString::number(xu[i]) + " " + QString::number(yu[i]) + " " + QString::number(zu[i]) + "\n");
                    }

                    for(i=0; i<nKnots; i++)
                    {
                        xu_[i] = (xu[i] + (xu[nKnots-1] - xu[nKnots-1-i]))/2.0;
                        yu_[i] = yu[nKnots-1-i];
                        zu_[i] = zu[nKnots-1-i];
                    }



    //Lower surfaces
                ////////////////////////////////////////////////////////////////////////////////////////
                    xl = new  float[nKnots], xl_ = new  float[nKnots];
                    yl = new  float[nKnots], yl_ = new  float[nKnots];
                    zl = new  float[nKnots], zl_ = new  float[nKnots];

                    ui->textEdit_CONSOLE_WINDOW->append("Airfoil points-Lower:\n");

                    for(i=0; i<nKnots; i++)
                    {
                        basex = 0, basey=0, basez=0;
                        xl[i] = 0.0, yl[i] = 0.0, zl[i] = 0.0;
                        for(int j=0; j<N; j++)
                        {
                            basex += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * xAnchorL[j];
                            basey += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * yAnchorL[j];
                            //basez += 0;

                            xl[i] +=  basex;
                            yl[i] +=  basey;
                            zl[i] +=  0.0; //basez * pow(1-t[i],(N-1)-k[i]) * pow(t[i],k[i]);
                        }
                        ui->textEdit_CONSOLE_WINDOW->append(QString::number(xl[i]) + " " + QString::number(yl[i]) + " " + QString::number(zl[i]) + "\n");
                    }

                    for(i=0; i<nKnots; i++)
                    {
                        xl_[i] = (xl[i] + (xl[nKnots-1] - xl[nKnots-1-i]))/2.0;
                        yl_[i] = yl[nKnots-1-i];
                        zl_[i] = zl[nKnots-1-i];
                    }

                    //Draw Airfoil...
                    set_anchors();
                    set_bezier_knots();

                    N=-1;
 }

    else {
            /////////////////////////////////////////////////////////////////////////////
            //Enter nKnots, order
            float *t = new float[nKnots];
            float inter_t = 1.0/((float)nKnots-1);
            for(i=0; i<nKnots; i++)
            {
                if(i==0)
                    t[i] = 0;
                else
                    t[i] = t[i-1] + inter_t;
            }

            int *k = new int[N]; //3 values
            for(int m=0; m<N; m++)
                k[m] = m+1;
            float *nCk = new float[N]; //3 values

            for(i=0; i<N; i++)
            {
                float n1 = fact(N);
                float n2 = fact(N-k[i]);
                float n3 = fact(k[i]);
                nCk[i] = n1/(n2*n3);
            }
            if(xu) {
                delete [] xu;
                xu = nullptr;
            }  if(xu_) {delete [] xu_; xu_ = nullptr;}
            if(yu) {delete [] yu; yu = nullptr;}  if(yu_) {delete [] yu_; yu_ = nullptr;}
            if(zu) {delete [] zu; zu = nullptr;}  if(zu_) {delete [] yu_; zu_ = nullptr;}
        //Upper surfaces
        ///////////////////////////////////////////////////////////////////////////////////////////
            xu = new  float[nKnots], xu_ = new  float[nKnots];
            yu = new  float[nKnots], yu_ = new  float[nKnots];
            zu = new  float[nKnots], zu_ = new  float[nKnots];
            float basex = 0; float basey = 0; float basez=0;

            ui->textEdit_CONSOLE_WINDOW->append("Airfoil points-Upper:\n");

            for(i=0; i<nKnots; i++)
            {
                basex = 0, basey=0, basez=0;
                xu[i] = 0.0, yu[i] = 0.0, zu[i] = 0.0;
                for(int j=0; j<N; j++)
                {
                    basex += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * xAnchorU[j];
                    basey += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * yAnchorU[j];
                    //basez += 0;

                    xu[i] +=  basex;
                    yu[i] +=  basey;
                    zu[i] +=  0.0; //basez * pow(1-t[i],(N-1)-k[i]) * pow(t[i],k[i]);
                }
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xu[i]) + " " + QString::number(yu[i]) + " " + QString::number(zu[i]) + "\n");
            }

            for(i=0; i<nKnots; i++)
            {
                xu_[i] = (xu[i] + (xu[nKnots-1] - xu[nKnots-1-i]))/2.0;
                yu_[i] = yu[nKnots-1-i];
                zu_[i] = zu[nKnots-1-i];
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xu_[i]) + " " + QString::number(yu_[i]) + " " + QString::number(zu_[i]) + "\n");
            }
        //Lower surfaces
        ////////////////////////////////////////////////////////////////////////////////////////
            xl = new  float[nKnots], xl_ = new  float[nKnots];
            yl = new  float[nKnots], yl_ = new  float[nKnots];
            zl = new  float[nKnots], zl_ = new  float[nKnots];

            ui->textEdit_CONSOLE_WINDOW->append("Airfoil points-Lower:\n");

            for(i=0; i<nKnots; i++)
            {
                basex = 0, basey=0, basez=0;
                xl[i] = 0.0, yl[i] = 0.0, zl[i] = 0.0;
                for(int j=0; j<N; j++)
                {
                    basex += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * xAnchorL[j];
                    basey += nCk[j] * pow(1-t[i],(N)-k[j]) * pow(t[i],k[j]) * yAnchorL[j];
                    //basez += 0;

                    xl[i] +=  basex;
                    yl[i] +=  basey;
                    zl[i] +=  0.0; //basez * pow(1-t[i],(N-1)-k[i]) * pow(t[i],k[i]);
                }
                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xl[i]) + " " + QString::number(yl[i]) + " " + QString::number(zl[i]) + "\n");
            }

            for(i=0; i<nKnots; i++)
            {
                xl_[i] = (xl[i] + (xl[nKnots-1] - xl[nKnots-1-i]))/2.0;
                yl_[i] = yl[nKnots-1-i];
                zl_[i] = zl[nKnots-1-i];

                ui->textEdit_CONSOLE_WINDOW->append(QString::number(xl_[i]) + " " + QString::number(yl_[i]) + " " + QString::number(zl_[i]) + "\n");

            }
            //Draw Airfoil...
            set_anchors();
            set_bezier_knots();
            }
}
void DialogTerrain::push_apply_anchors()
{

}
void DialogTerrain::push_read_anchors()
{
    xAnchorU=nullptr, yAnchorU=nullptr, zAnchorU=nullptr;
    xAnchorL=nullptr, yAnchorL=nullptr, zAnchorL=nullptr;
    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
                                                     ".",
                                                     ("DataAnchors (*.dat)"));
    QFile file(fileName);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return;
       QTextStream in(&file);
       in >> N;

       ui->textEdit_CONSOLE_WINDOW->append(QString::number(N) + "\n");
       xAnchorU = new float[N];
       yAnchorU = new float[N];
       zAnchorU = new float[N];

       xAnchorL = new float[N];
       yAnchorL = new float[N];
       zAnchorL = new float[N];


       i=0;
       for(i=0; i<N/2; i++)
       {
            in >> xAnchorU[i] >> yAnchorU[i] >> zAnchorU[i];
            ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorU[i]) + " " + QString::number(yAnchorU[i]) + " " + QString::number(zAnchorU[i]) + "\n");
       }

       for(i=0; i<N/2; i++)
       {
            in >> xAnchorL[i] >> yAnchorL[i] >> zAnchorL[i];
            ui->textEdit_CONSOLE_WINDOW->append(QString::number(xAnchorU[i]) + " " + QString::number(yAnchorU[i]) + " " + QString::number(zAnchorU[i]) + "\n");
       }

       file.close();
}

float DialogTerrain::fact(int n)
{
        int i;
        unsigned long long fact = 1;
        // shows error if the user enters a negative integer
        if (n < 0)
            printf("Error! Factorial of a negative number doesn't exist.");
        else {
            for (i = 1; i <= n; ++i) {
                fact *= i;
            }

            return fact;
        }      
}

void DialogTerrain::set_anchors()
{
    cairfoil_designer->set_anchors(N, xAnchorU, yAnchorU, zAnchorU, xAnchorL, yAnchorL, zAnchorL );
}
void DialogTerrain::set_bezier_knots()
{
    cairfoil_designer->set_bezier_knots(nKnots, xu_, yu_, zu, xl_, yl_, zl );
}
