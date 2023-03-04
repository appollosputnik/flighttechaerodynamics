#ifndef _CDIALOGCONTROLLERWINGS_
#pragma once
#include "C:\OpenCASCADE-7.7.0-vc14-64\qt5.11.2-vc14-64\include\QtWidgets\qdialog.h"
#include "ui_dialogcontroller.h"
#include "ClassDlgConsole.h"
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include "../src/View.h"
class CDialogControllerWings :
    public QDialog
{
public:
    CDialogControllerWings();
	~CDialogControllerWings();
    Ui_DialogController* ui;
	View* myView;
	void setView(View* view) { myView = view;  }
	void setConsole(ClassDlgConsole *cnsl) { _console = cnsl;  }
private slots:
    void _apply_full_wings();

    ////////////////////////////////////////////////////////////////////////////////////////////
	float  fact(int n);
private:
	ClassDlgConsole * _console;
	int nKnots;
	static int i;
	static int N;
	float* xu, * yu, * zu, * xl, * yl, * zl, * xu_, * yu_, * zu_, * xl_, * yl_, * zl_;
	float* xAnchorU, * yAnchorU, * zAnchorU, * xAnchorL, * yAnchorL, * zAnchorL,
		* xAnchorU_, * yAnchorU_, * zAnchorU_, * xAnchorL_, * yAnchorL_, * zAnchorL_;

	View* cairfoil_designer;
	float xMin, xMax, yMin, yMax, xMinKnots, xMaxKnots, yMinKnots, yMaxKnots;
private slots:
	void read3DDataNPlot();
	void make3D();
	void drawStarBoardWing();
	void drawPortWing();
	void drawFuselage();
	void fuselage_show_hide();
	void bezier_show_hide();
	void anchor_show_hide();
	void hyperbolic_grid_show_hide();
	//////////////////////////////////////////////////////////////////////////
	/// \brief drawHGrid
	void _import_stl();
	void _export_stl();
	void _import_obj();
	void _export_obj();
	void _import_iges();
	void _export_iges();
	void _import_step();
	void _export_step();

	void drawHGrid();


	void set_anchors();
	void set_bezier_knots();
	void push_apply_anchors();
	void push_read_anchors();
};

#endif

