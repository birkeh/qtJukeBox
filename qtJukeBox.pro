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
    common.cpp \
    cmusicviewitemdelegate.cpp \
    calbum.cpp \
    cid3field.cpp \
    cmediainfo.cpp \
    cpicture.cpp \
    libid3tag/compat.c \
    libid3tag/crc.c \
    libid3tag/debug.c \
    libid3tag/field.c \
    libid3tag/file.c \
    libid3tag/frame.c \
    libid3tag/frametype.c \
    libid3tag/genre.c \
    libid3tag/latin1.c \
    libid3tag/parse.c \
    libid3tag/render.c \
    libid3tag/tag.c \
    libid3tag/ucs4.c \
    libid3tag/utf16.c \
    libid3tag/utf8.c \
    libid3tag/util.c \
    libid3tag/version.c \
    libmpg123/dxhead.c \
    libmpg123/getbits.c \
    libmpg123/layer1.c \
    libmpg123/layer2.c \
    libmpg123/layer3.c \
    libmpg123/mpg123.c

HEADERS  += cmainwindow.h \
    common.h \
    cmusicviewitemdelegate.h \
    calbum.h \
    libid3tag/compat.h \
    libid3tag/crc.h \
    libid3tag/debug.h \
    libid3tag/field.h \
    libid3tag/file.h \
    libid3tag/frame.h \
    libid3tag/frametype.h \
    libid3tag/genre.h \
    libid3tag/global.h \
    libid3tag/id3tag.h \
    libid3tag/latin1.h \
    libid3tag/parse.h \
    libid3tag/render.h \
    libid3tag/tag.h \
    libid3tag/ucs4.h \
    libid3tag/utf16.h \
    libid3tag/utf8.h \
    libid3tag/util.h \
    libid3tag/version.h \
    libmpg123/dxhead.h \
    libmpg123/getbits.h \
    libmpg123/huffman.h \
    libmpg123/l2tables.h \
    libmpg123/mpg123.h \
    calbum.h \
    cid3field.h \
    cmediainfo.h \
    cpicture.h \
    fields.h

FORMS    += cmainwindow.ui

LIBS += -lz

DISTFILES += \
    libid3tag/genre.dat
