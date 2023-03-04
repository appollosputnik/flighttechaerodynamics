#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QScrollArea>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);

    _stack_widget = new QStackedWidget;
    setCentralWidget(_stack_widget);
    _central_default = new MainWidget;
    _central_window_cube_1 = new MainWidgetSupplment_1;
    _mainwidget_supplement_2 = new MainWidgetSupplment_2;
    _mainwidget_supplement_3 = new MainWidgetSupplment_3;
    _airfoil_designer = new DialogAirfoilDesigner;
    _stack_widget->addWidget(_central_default);
    _stack_widget->addWidget(_central_window_cube_1);
    _stack_widget->addWidget(_mainwidget_supplement_2);
    _stack_widget->addWidget(_mainwidget_supplement_3);
    _stack_widget->addWidget(_airfoil_designer);

    //QActions
    QAction *_default_action, *_action_1, *_action_2, *_action_3, *_action_4
            , *_action_5, *_action_6, *_action_7;

    _default_action = new QAction("defaultCube");
    connect(_default_action, SIGNAL(triggered()), this, SLOT(_default_slot()));

    _action_1 = new QAction("Texture");
    connect(_action_1, SIGNAL(triggered()), this, SLOT(_slot_1()));

    _action_2 = new QAction("checkerBoard");
    connect(_action_2, SIGNAL(triggered()), this, SLOT(_slot_2_checkerboard()));

    _action_3 = new QAction("Torus");
    connect(_action_3, SIGNAL(triggered()), this, SLOT(_slot_3_Torus()));

    _action_4 = new QAction("Generate Airfoil...");
    connect(_action_4, SIGNAL(triggered()), this, SLOT(_slot_4_DrawAirfoil()));


    _action_5 = new QAction("Real time CL~Alpha curve...");
    connect(_action_5, SIGNAL(triggered()), this, SLOT(_slot_5_realtime()));

    _action_6 = new QAction("Aerodynamic curves...");
    connect(_action_6, SIGNAL(triggered()), this, SLOT(_slot_6_realtime()));

    _action_7 = new QAction("Aerodynamic post processing...");
    connect(_action_7, SIGNAL(triggered()), this, SLOT(_slot_7_realtime()));

    ui->toolBar->addAction(_default_action);
    ui->toolBar->addAction(_action_1);
    ui->toolBar->addAction(_action_2);
    ui->toolBar->addAction(_action_3);
    ui->toolBar->addAction(_action_4);
    ui->toolBar->addAction(_action_5);
    ui->toolBar->addAction(_action_6);
    ui->toolBar->addAction(_action_7);
    _stack_widget->setCurrentIndex(4);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::_default_slot()
{
    _stack_widget->setCurrentIndex(0);
}

void MainWindow::_slot_1()
{
    _stack_widget->setCurrentIndex(1);
}

void MainWindow::_slot_2_checkerboard()
{
    _stack_widget->setCurrentIndex(2);
}

void MainWindow::_slot_3_Torus()
{
    _stack_widget->setCurrentIndex(3);
}

void MainWindow::_slot_4_DrawAirfoil()
{
    _stack_widget->setCurrentIndex(4);
}

void MainWindow::_slot_5_realtime()
{
    _stack_widget->setCurrentIndex(5);
}

void MainWindow::_slot_6_realtime()
{
    _stack_widget->setCurrentIndex(6);
}

void MainWindow::_slot_7_realtime()
{
    _stack_widget->setCurrentIndex(7);
}
