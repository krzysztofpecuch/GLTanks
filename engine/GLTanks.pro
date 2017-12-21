TEMPLATE = app
CONFIG -= console c++11
CONFIG -= app_bundle
CONFIG += qt

SOURCES += main.cpp \
    game.cpp \
    tank.cpp

HEADERS += \
    game.h \
    tank.h

INCLUDEPATH += C:\Projects\SFML-2.4.2\include
DEPENDPATH  += C:\Projects\SFML-2.4.2\include

LIBS += -LC:\Projects\SFML_Build\Libs\

CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-system-d -lsfml-network-d -lsfml-main-d -lsfml-window-d -lsfml-graphics-d
CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-system -lsfml-network -lsfml-main -lsfml-window -lsfml-graphics
