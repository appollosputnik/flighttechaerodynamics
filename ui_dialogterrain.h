/********************************************************************************
** Form generated from reading UI file 'dialogterrain.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGTERRAIN_H
#define UI_DIALOGTERRAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogTerrain
{
public:
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea_CONTROL_WIDGET;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_CHORD_LENGTH;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_CAMBER;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_MAX_THICKNESS;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QLineEdit *lineEdit_MAX_THICKNESS_POS;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *lineEdit_ORDER;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *lineEdit_NO_KNOTS;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_READ_ANCHORS;
    QPushButton *pushButton_APPLY_READ_ANCHORS;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *pushButton_ANCHOR_SHOW_HIDE;
    QPushButton *pushButton_BEZIER_SHOW_HIDE;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_APPLY_FULL;
    QPushButton *pushButton_CANCEL;
    QPushButton *pushButton_CLOSE;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_7;
    QLineEdit *lineEdit_nHGRID;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_DRAW_H_GRID;
    QPushButton *pushButton_SHOW_HIDE_GRID;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *pushButton_IMPORT_STL;
    QPushButton *pushButton_IMPORT_OBJ;
    QPushButton *pushButton_IMPORT_IGES;
    QPushButton *pushButton_IMPORT_STEP;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton_EXPORT_STL;
    QPushButton *pushButton_EXPORT_OBJ;
    QPushButton *pushButton_EXPORT_IGES_2;
    QPushButton *pushButton_EXPORT_STEP;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_8;
    QLineEdit *lineEdit_NUMBER_OF_SLICES;
    QPushButton *pushButton_DRAW_FUSELAGE;
    QSpacerItem *horizontalSpacer_12;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_9;
    QLineEdit *lineEdit_ENTER_TAPER_RATIO;
    QPushButton *pushButton_DRAW_STARBOARD_WING;
    QPushButton *pushButton_DRAW_PORT_WING;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_SCREEN;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea_GRAPHICS_WINDOW;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *textEdit_CONSOLE_WINDOW;

    void setupUi(QDialog *DialogTerrain)
    {
        if (DialogTerrain->objectName().isEmpty())
            DialogTerrain->setObjectName(QStringLiteral("DialogTerrain"));
        DialogTerrain->resize(987, 640);
        gridLayout_3 = new QGridLayout(DialogTerrain);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, 5, -1);
        scrollArea_CONTROL_WIDGET = new QScrollArea(DialogTerrain);
        scrollArea_CONTROL_WIDGET->setObjectName(QStringLiteral("scrollArea_CONTROL_WIDGET"));
        scrollArea_CONTROL_WIDGET->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 527, 618));
        gridLayout = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 5, -1, 5);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, -1, 5, -1);
        label = new QLabel(scrollAreaWidgetContents_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit_CHORD_LENGTH = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_CHORD_LENGTH->setObjectName(QStringLiteral("lineEdit_CHORD_LENGTH"));

        horizontalLayout_2->addWidget(lineEdit_CHORD_LENGTH);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, -1, 5, -1);
        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_CAMBER = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_CAMBER->setObjectName(QStringLiteral("lineEdit_CAMBER"));

        horizontalLayout_3->addWidget(lineEdit_CAMBER);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(5, -1, 5, -1);
        label_3 = new QLabel(scrollAreaWidgetContents_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_MAX_THICKNESS = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_MAX_THICKNESS->setObjectName(QStringLiteral("lineEdit_MAX_THICKNESS"));

        horizontalLayout_4->addWidget(lineEdit_MAX_THICKNESS);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_6 = new QLabel(scrollAreaWidgetContents_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_9->addWidget(label_6);

        lineEdit_MAX_THICKNESS_POS = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_MAX_THICKNESS_POS->setObjectName(QStringLiteral("lineEdit_MAX_THICKNESS_POS"));

        horizontalLayout_9->addWidget(lineEdit_MAX_THICKNESS_POS);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(5, -1, 5, -1);
        label_4 = new QLabel(scrollAreaWidgetContents_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        lineEdit_ORDER = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_ORDER->setObjectName(QStringLiteral("lineEdit_ORDER"));

        horizontalLayout_5->addWidget(lineEdit_ORDER);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(5, -1, 5, -1);
        label_5 = new QLabel(scrollAreaWidgetContents_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        lineEdit_NO_KNOTS = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_NO_KNOTS->setObjectName(QStringLiteral("lineEdit_NO_KNOTS"));

        horizontalLayout_6->addWidget(lineEdit_NO_KNOTS);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        pushButton_READ_ANCHORS = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_READ_ANCHORS->setObjectName(QStringLiteral("pushButton_READ_ANCHORS"));

        horizontalLayout_8->addWidget(pushButton_READ_ANCHORS);

        pushButton_APPLY_READ_ANCHORS = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_APPLY_READ_ANCHORS->setObjectName(QStringLiteral("pushButton_APPLY_READ_ANCHORS"));

        horizontalLayout_8->addWidget(pushButton_APPLY_READ_ANCHORS);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(5, -1, 5, -1);
        pushButton_ANCHOR_SHOW_HIDE = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_ANCHOR_SHOW_HIDE->setObjectName(QStringLiteral("pushButton_ANCHOR_SHOW_HIDE"));

        horizontalLayout_13->addWidget(pushButton_ANCHOR_SHOW_HIDE);

        pushButton_BEZIER_SHOW_HIDE = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_BEZIER_SHOW_HIDE->setObjectName(QStringLiteral("pushButton_BEZIER_SHOW_HIDE"));

        horizontalLayout_13->addWidget(pushButton_BEZIER_SHOW_HIDE);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        pushButton_APPLY_FULL = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_APPLY_FULL->setObjectName(QStringLiteral("pushButton_APPLY_FULL"));

        horizontalLayout_7->addWidget(pushButton_APPLY_FULL);

        pushButton_CANCEL = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_CANCEL->setObjectName(QStringLiteral("pushButton_CANCEL"));

        horizontalLayout_7->addWidget(pushButton_CANCEL);

        pushButton_CLOSE = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_CLOSE->setObjectName(QStringLiteral("pushButton_CLOSE"));

        horizontalLayout_7->addWidget(pushButton_CLOSE);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_7 = new QLabel(scrollAreaWidgetContents_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_14->addWidget(label_7);

        lineEdit_nHGRID = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_nHGRID->setObjectName(QStringLiteral("lineEdit_nHGRID"));

        horizontalLayout_14->addWidget(lineEdit_nHGRID);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(5, -1, 5, -1);
        pushButton_DRAW_H_GRID = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_DRAW_H_GRID->setObjectName(QStringLiteral("pushButton_DRAW_H_GRID"));

        horizontalLayout_10->addWidget(pushButton_DRAW_H_GRID);

        pushButton_SHOW_HIDE_GRID = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_SHOW_HIDE_GRID->setObjectName(QStringLiteral("pushButton_SHOW_HIDE_GRID"));

        horizontalLayout_10->addWidget(pushButton_SHOW_HIDE_GRID);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_9);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(5, -1, 5, -1);
        pushButton_IMPORT_STL = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_IMPORT_STL->setObjectName(QStringLiteral("pushButton_IMPORT_STL"));

        horizontalLayout_11->addWidget(pushButton_IMPORT_STL);

        pushButton_IMPORT_OBJ = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_IMPORT_OBJ->setObjectName(QStringLiteral("pushButton_IMPORT_OBJ"));

        horizontalLayout_11->addWidget(pushButton_IMPORT_OBJ);

        pushButton_IMPORT_IGES = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_IMPORT_IGES->setObjectName(QStringLiteral("pushButton_IMPORT_IGES"));

        horizontalLayout_11->addWidget(pushButton_IMPORT_IGES);

        pushButton_IMPORT_STEP = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_IMPORT_STEP->setObjectName(QStringLiteral("pushButton_IMPORT_STEP"));

        horizontalLayout_11->addWidget(pushButton_IMPORT_STEP);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(5, -1, 5, -1);
        pushButton_EXPORT_STL = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_EXPORT_STL->setObjectName(QStringLiteral("pushButton_EXPORT_STL"));

        horizontalLayout_12->addWidget(pushButton_EXPORT_STL);

        pushButton_EXPORT_OBJ = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_EXPORT_OBJ->setObjectName(QStringLiteral("pushButton_EXPORT_OBJ"));

        horizontalLayout_12->addWidget(pushButton_EXPORT_OBJ);

        pushButton_EXPORT_IGES_2 = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_EXPORT_IGES_2->setObjectName(QStringLiteral("pushButton_EXPORT_IGES_2"));

        horizontalLayout_12->addWidget(pushButton_EXPORT_IGES_2);

        pushButton_EXPORT_STEP = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_EXPORT_STEP->setObjectName(QStringLiteral("pushButton_EXPORT_STEP"));

        horizontalLayout_12->addWidget(pushButton_EXPORT_STEP);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(5, -1, 5, -1);
        label_8 = new QLabel(scrollAreaWidgetContents_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_15->addWidget(label_8);

        lineEdit_NUMBER_OF_SLICES = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_NUMBER_OF_SLICES->setObjectName(QStringLiteral("lineEdit_NUMBER_OF_SLICES"));

        horizontalLayout_15->addWidget(lineEdit_NUMBER_OF_SLICES);

        pushButton_DRAW_FUSELAGE = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_DRAW_FUSELAGE->setObjectName(QStringLiteral("pushButton_DRAW_FUSELAGE"));

        horizontalLayout_15->addWidget(pushButton_DRAW_FUSELAGE);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_12);


        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalLayout_16->setContentsMargins(5, -1, 5, -1);
        label_9 = new QLabel(scrollAreaWidgetContents_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_16->addWidget(label_9);

        lineEdit_ENTER_TAPER_RATIO = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_ENTER_TAPER_RATIO->setObjectName(QStringLiteral("lineEdit_ENTER_TAPER_RATIO"));

        horizontalLayout_16->addWidget(lineEdit_ENTER_TAPER_RATIO);

        pushButton_DRAW_STARBOARD_WING = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_DRAW_STARBOARD_WING->setObjectName(QStringLiteral("pushButton_DRAW_STARBOARD_WING"));

        horizontalLayout_16->addWidget(pushButton_DRAW_STARBOARD_WING);

        pushButton_DRAW_PORT_WING = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_DRAW_PORT_WING->setObjectName(QStringLiteral("pushButton_DRAW_PORT_WING"));

        horizontalLayout_16->addWidget(pushButton_DRAW_PORT_WING);


        verticalLayout_2->addLayout(horizontalLayout_16);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 0, 1, 1);

        scrollArea_CONTROL_WIDGET->setWidget(scrollAreaWidgetContents_2);

        horizontalLayout->addWidget(scrollArea_CONTROL_WIDGET);

        widget_SCREEN = new QWidget(DialogTerrain);
        widget_SCREEN->setObjectName(QStringLiteral("widget_SCREEN"));
        gridLayout_2 = new QGridLayout(widget_SCREEN);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 5, -1, 5);
        scrollArea_GRAPHICS_WINDOW = new QScrollArea(widget_SCREEN);
        scrollArea_GRAPHICS_WINDOW->setObjectName(QStringLiteral("scrollArea_GRAPHICS_WINDOW"));
        scrollArea_GRAPHICS_WINDOW->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 402, 291));
        scrollArea_GRAPHICS_WINDOW->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea_GRAPHICS_WINDOW);

        textEdit_CONSOLE_WINDOW = new QTextEdit(widget_SCREEN);
        textEdit_CONSOLE_WINDOW->setObjectName(QStringLiteral("textEdit_CONSOLE_WINDOW"));

        verticalLayout->addWidget(textEdit_CONSOLE_WINDOW);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout->addWidget(widget_SCREEN);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(DialogTerrain);

        QMetaObject::connectSlotsByName(DialogTerrain);
    } // setupUi

    void retranslateUi(QDialog *DialogTerrain)
    {
        DialogTerrain->setWindowTitle(QApplication::translate("DialogTerrain", "Dialog", nullptr));
        label->setText(QApplication::translate("DialogTerrain", "Chord Length :", nullptr));
        lineEdit_CHORD_LENGTH->setText(QApplication::translate("DialogTerrain", "100", nullptr));
        label_2->setText(QApplication::translate("DialogTerrain", "Camber (%c):", nullptr));
        lineEdit_CAMBER->setText(QApplication::translate("DialogTerrain", "7", nullptr));
        label_3->setText(QApplication::translate("DialogTerrain", "Max Thickness (%c):", nullptr));
        lineEdit_MAX_THICKNESS->setText(QApplication::translate("DialogTerrain", "30", nullptr));
        label_6->setText(QApplication::translate("DialogTerrain", "Max Thickness position:", nullptr));
        lineEdit_MAX_THICKNESS_POS->setText(QApplication::translate("DialogTerrain", "30", nullptr));
        label_4->setText(QApplication::translate("DialogTerrain", "Order:", nullptr));
        lineEdit_ORDER->setText(QApplication::translate("DialogTerrain", "2", nullptr));
        label_5->setText(QApplication::translate("DialogTerrain", "No. Knots:", nullptr));
        lineEdit_NO_KNOTS->setText(QApplication::translate("DialogTerrain", "11", nullptr));
        pushButton_READ_ANCHORS->setText(QApplication::translate("DialogTerrain", "Read Anchors", nullptr));
        pushButton_APPLY_READ_ANCHORS->setText(QApplication::translate("DialogTerrain", "Apply", nullptr));
        pushButton_ANCHOR_SHOW_HIDE->setText(QApplication::translate("DialogTerrain", "Show/Hide Anchors", nullptr));
        pushButton_BEZIER_SHOW_HIDE->setText(QApplication::translate("DialogTerrain", "Show/Hide Airfoil", nullptr));
        pushButton_APPLY_FULL->setText(QApplication::translate("DialogTerrain", "Apply", nullptr));
        pushButton_CANCEL->setText(QApplication::translate("DialogTerrain", "Cancel", nullptr));
        pushButton_CLOSE->setText(QApplication::translate("DialogTerrain", "Close", nullptr));
        label_7->setText(QApplication::translate("DialogTerrain", "Number of grid...:", nullptr));
        lineEdit_nHGRID->setText(QApplication::translate("DialogTerrain", "11", nullptr));
        pushButton_DRAW_H_GRID->setText(QApplication::translate("DialogTerrain", "Draw Hyperbolic Grid...", nullptr));
        pushButton_SHOW_HIDE_GRID->setText(QApplication::translate("DialogTerrain", "Show/Hide Grid", nullptr));
        pushButton_IMPORT_STL->setText(QApplication::translate("DialogTerrain", "Import STL...", nullptr));
        pushButton_IMPORT_OBJ->setText(QApplication::translate("DialogTerrain", "Import OBJ...", nullptr));
        pushButton_IMPORT_IGES->setText(QApplication::translate("DialogTerrain", "Import IGES...", nullptr));
        pushButton_IMPORT_STEP->setText(QApplication::translate("DialogTerrain", "Import STEP...", nullptr));
        pushButton_EXPORT_STL->setText(QApplication::translate("DialogTerrain", "Export STL...", nullptr));
        pushButton_EXPORT_OBJ->setText(QApplication::translate("DialogTerrain", "Export OBJ...", nullptr));
        pushButton_EXPORT_IGES_2->setText(QApplication::translate("DialogTerrain", "Export IGES...", nullptr));
        pushButton_EXPORT_STEP->setText(QApplication::translate("DialogTerrain", "Export STEP...", nullptr));
        label_8->setText(QApplication::translate("DialogTerrain", "Enter No of slices :", nullptr));
        lineEdit_NUMBER_OF_SLICES->setText(QApplication::translate("DialogTerrain", "2", nullptr));
        pushButton_DRAW_FUSELAGE->setText(QApplication::translate("DialogTerrain", "Draw Fuselage", nullptr));
        label_9->setText(QApplication::translate("DialogTerrain", "Enter Taper Ratio :", nullptr));
        lineEdit_ENTER_TAPER_RATIO->setText(QApplication::translate("DialogTerrain", "0.3", nullptr));
        pushButton_DRAW_STARBOARD_WING->setText(QApplication::translate("DialogTerrain", "Draw Starboard Wing", nullptr));
        pushButton_DRAW_PORT_WING->setText(QApplication::translate("DialogTerrain", "Draw Port Wing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogTerrain: public Ui_DialogTerrain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGTERRAIN_H
