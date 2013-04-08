#-------------------------------------------------
#
# Project created by QtCreator 2013-03-28T18:51:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pacheck
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    process.cpp \
    package.cpp \
    svnutils.cpp \
    settings.cpp \
    state.cpp \
    state_widget.cpp \
    state_svncheck.cpp \
    state_getpackagesfolder.cpp

HEADERS  += mainwidget.h \
    process.h \
    package.h \
    svnutils.h \
    settings.h \
    state.h \
    state_widget.h \
    state_svncheck.h \
    state_getpackagesfolder.h

FORMS    += mainwidget.ui
