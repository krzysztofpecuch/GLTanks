TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    game.cpp \
    server.cpp \
    client.cpp \
    resources.cpp \
    tank.cpp


INCLUDEPATH += C:\SFML-2.4.2\include
DEPENDPATH += C:\SFML-2.4.2\include

LIBS += -LC:\Projects\SFML_Build\Libs\

CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-system-d -lsfml-network-d -lsfml-main-d -lsfml-window-d -lsfml-graphics-d
CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-system -lsfml-network -lsfml-main -lsfml-window -lsfml-graphics

HEADERS += \
    game.h \
    server.h \
    client.h \
    resources.h \
    tank.h
