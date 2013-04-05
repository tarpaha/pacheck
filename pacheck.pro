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
    svnutils.cpp

HEADERS  += mainwidget.h \
    process.h \
    package.h \
    svnutils.h

FORMS    += mainwidget.ui
