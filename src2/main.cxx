#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwindow.h"
#endif

#include <fstream>
#include <iostream>
#include "math.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("Best Practices GL Qt - V1.0");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWindow window;
    window.showMaximized();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
return app.exec();
}


