QT       += core gui widgets

TARGET = BestPracticesGLQt
TEMPLATE = app

SOURCES += *.cxx

HEADERS += \
           *.h \
           *.hxx

RESOURCES += *.qrc

OTHERS += *.txt

INSTALLS += target

DISTFILES +=

FORMS += *.ui

LIBS += -lOPENGL32 -lglu32
