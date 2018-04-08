QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console
CONFIG += c++17

SOURCES += main.cpp \
    MainController/MainController.cpp \
    Analyser/Analyser.cpp \
    Reader/Reader.cpp \
    Drawer/Drawer.cpp \
    Map/Map.cpp \
    Analyser/IPLocation.cpp

HEADERS += \
    MainController/MainController.hpp \
    Analyser/Analyser.hpp \
    Reader/Reader.hpp \
    Drawer/Drawer.hpp \
    Map/Map.hpp \
    Analyser/IPLocation.hpp

INCLUDEPATH += \
    /home/marcin/workspace/includes

LIBS += \
    -pthread \
