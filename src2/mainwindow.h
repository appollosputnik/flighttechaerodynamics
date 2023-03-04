#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "mainwidget.h"
#include "mainwidget_supplement_1.h"
#include "mainwidget_supplement_2.h"
#include "mainwidget_supplement_3.h"
#include "dialogairfoildesigner.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *_stack_widget;
    MainWidget *_central_default;
    MainWidgetSupplment_1 *_central_window_cube_1;
    MainWidgetSupplment_2 *_mainwidget_supplement_2;
    MainWidgetSupplment_3 *_mainwidget_supplement_3;
    Ui::MainWindow *ui;
    DialogAirfoilDesigner *_airfoil_designer;
private slots:
    void _default_slot();
    void _slot_1();
    void _slot_2_checkerboard();
    void _slot_3_Torus();
    void _slot_4_DrawAirfoil();
    void _slot_5_realtime();
    void _slot_6_realtime();
    void _slot_7_realtime();
};

#endif // MAINWINDOW_H
