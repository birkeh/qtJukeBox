#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T16:58:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtJukeBox
TEMPLATE = app


SOURCES += main.cpp\
        cmainwindow.cpp \
    cmediainfo.cpp \
    ctag.cpp \
    cdatabase.cpp \
    cstring.cpp \
    cimage.cpp

HEADERS  += cmainwindow.h \
    cmediainfo.h \
    ctag.h \
    cdatabase.h \
    cstring.h \
    cimage.h

FORMS    += cmainwindow.ui

LIBS	+= -ltaglib
