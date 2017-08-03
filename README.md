# SFML-Mandelbrot-fract-viewer
Mandelbrot fractal viewer based on SFML

I've done this in Windows using QtCreator as IDE (Plain C++ Application, without Qt lib itself but using qmake build system)

Here is example of .pro file

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mandelbrotcalculator.cpp \
    sfml_visualizer.cpp

HEADERS += \
    mandelbrotcalculator.h \
    sfml_visualizer.h \
    view.h

LIBS += -L/path/to/sfml/binaries

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += /path/to/sfml/includes
DEPENDPATH += /path/to/sfml/includes

