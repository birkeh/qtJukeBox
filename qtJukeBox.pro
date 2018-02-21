#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T16:58:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtJukeBox
TEMPLATE = app


INCLUDEPATH += "C:/dev/3rdParty/taglib/include"
DEPENDPATH += "C:/dev/3rdParty/taglib/include"
LIBS += "C:/dev/3rdParty/taglib/lib/libtag.dll.a"

SOURCES += main.cpp\
        cmainwindow.cpp \
    cmediainfo.cpp \
    ctag.cpp \
    cdatabase.cpp \
    cstring.cpp \
    common.cpp \
    cpixmap.cpp

HEADERS  += cmainwindow.h \
    cmediainfo.h \
    ctag.h \
    cdatabase.h \
    cstring.h \
    common.h \
    cpixmap.h

FORMS    += cmainwindow.ui

#LIBS	+= -ltaglib
