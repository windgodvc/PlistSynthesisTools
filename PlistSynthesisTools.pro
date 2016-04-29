#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T20:25:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlistSynthesisTools
TEMPLATE = app

INCLUDEPATH += /usr/local/Cellar/boost/1.56.0/include

SOURCES += main.cpp\
        mainwindow.cpp \
    FileUtils/Plist.cpp \
    FileUtils/PlistDate.cpp \
    FileUtils/pugixml.cpp

HEADERS  += mainwindow.h \
    FileUtils/base64.hpp \
    FileUtils/Plist.hpp \
    FileUtils/PlistDate.hpp \
    FileUtils/pugiconfig.hpp \
    FileUtils/pugixml.hpp

FORMS    += mainwindow.ui

CONFIG += C++11

RESOURCES += \
    res.qrc
