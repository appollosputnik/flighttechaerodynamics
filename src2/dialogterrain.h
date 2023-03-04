#ifndef DIALOGTERRAIN_H
#define DIALOGTERRAIN_H

#include <QDialog>
#include "CAirfoilDesignerWindow.h"
namespace Ui {
class DialogTerrain;
}

class DialogTerrain : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTerrain(QWidget *parent = nullptr);
    ~DialogTerrain();
    float  fact(int n);
private:
    int nKnots;
    static int i;
    static int N;
    float *xu, *yu, *zu, *xl, *yl, *zl, *xu_, *yu_, *zu_, *xl_, *yl_, *zl_;
    float *xAnchorU, *yAnchorU, *zAnchorU, *xAnchorL, *yAnchorL, *zAnchorL,
    *xAnchorU_, *yAnchorU_, *zAnchorU_, *xAnchorL_, *yAnchorL_, *zAnchorL_;
    Ui::DialogTerrain *ui;
    CAirfoilDesigner *cairfoil_designer;
    float xMin, xMax, yMin, yMax, xMinKnots, xMaxKnots, yMinKnots, yMaxKnots;
private slots:
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
    void push_apply_full();
    void push_apply_anchors();
    void push_read_anchors();
};

#endif // DIALOGAIRFOILDESIGNER_H
