/********************************************************************************
** Form generated from reading UI file 'dialog_console.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_CONSOLE_H
#define UI_DIALOG_CONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Dialog_CONSOLE
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit_CONSOLE_WINDOW;

    void setupUi(QDialog *Dialog_CONSOLE)
    {
        if (Dialog_CONSOLE->objectName().isEmpty())
            Dialog_CONSOLE->setObjectName(QStringLiteral("Dialog_CONSOLE"));
        Dialog_CONSOLE->resize(400, 300);
        gridLayout = new QGridLayout(Dialog_CONSOLE);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textEdit_CONSOLE_WINDOW = new QTextEdit(Dialog_CONSOLE);
        textEdit_CONSOLE_WINDOW->setObjectName(QStringLiteral("textEdit_CONSOLE_WINDOW"));

        gridLayout->addWidget(textEdit_CONSOLE_WINDOW, 0, 0, 1, 1);


        retranslateUi(Dialog_CONSOLE);

        QMetaObject::connectSlotsByName(Dialog_CONSOLE);
    } // setupUi

    void retranslateUi(QDialog *Dialog_CONSOLE)
    {
        Dialog_CONSOLE->setWindowTitle(QApplication::translate("Dialog_CONSOLE", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_CONSOLE: public Ui_Dialog_CONSOLE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_CONSOLE_H
