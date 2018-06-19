QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console
CONFIG += c++17

SOURCES += main.cpp \
    MainController/MainController.cpp \
    Analyser/Analyser.cpp \
    Analyser/LinkData.cpp \
    Reader/Reader.cpp \
    Drawer/Drawer.cpp \
    Map/Map.cpp

HEADERS += \
    MainController/MainController.hpp \
    Analyser/Analyser.hpp \
    Reader/Reader.hpp \
    Drawer/Drawer.hpp \
    Map/Map.hpp \
    Protocols/DrawingPackage.hpp \
    Analyser/LinkData.hpp \
    Protocols/Aliases.hpp

INCLUDEPATH += \
    ../includes

LIBS += \
    -pthread \
