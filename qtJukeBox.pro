#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T16:58:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtJukeBox
TEMPLATE = app


win32-msvc* {
    contains(QT_ARCH, i386) {
        message("msvc 32-bit")
        INCLUDEPATH += "C:/dev/3rdParty/taglib-msvc32/include"
        DEPENDPATH += "C:/dev/3rdParty/taglib-msvc32/include"
        LIBS += "C:/dev/3rdParty/taglib-msvc32/lib/tag.lib"
    } else {
        message("msvc 64-bit")
        INCLUDEPATH += "C:/dev/3rdParty/taglib-msvc64/include"
        DEPENDPATH += "C:/dev/3rdParty/taglib-msvc64/include"
        LIBS += "C:/dev/3rdParty/taglib-msvc64/lib/tag.lib"
    }
}

win32-g++ {
    message("mingw")
    INCLUDEPATH += "C:/dev/3rdParty/taglib/include"
    DEPENDPATH += "C:/dev/3rdParty/taglib/include"
    LIBS += "C:/dev/3rdParty/taglib/lib/libtag.dll.a"
}

unix {
    LIBS	+= -ltag
}

SOURCES += main.cpp\
        cmainwindow.cpp \
    cmediainfo.cpp \
    ctag.cpp \
    cdatabase.cpp \
    cstring.cpp \
    common.cpp \
    cpixmap.cpp \
    cmusicviewitemdelegate.cpp \
    calbum.cpp \
    ctrack.cpp

HEADERS  += cmainwindow.h \
    cmediainfo.h \
    ctag.h \
    cdatabase.h \
    cstring.h \
    common.h \
    cpixmap.h \
    cmusicviewitemdelegate.h \
    calbum.h \
    ctrack.h \
    common.h

FORMS    += cmainwindow.ui
