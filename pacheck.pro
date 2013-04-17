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
    state_getpackagesfolder.cpp \
    state_getpackageslist.cpp \
    state_getversions.cpp \
    utils.cpp \
    state_applychanges.cpp \
    commentswidget.cpp

HEADERS  += mainwidget.h \
    process.h \
    package.h \
    svnutils.h \
    settings.h \
    state.h \
    state_widget.h \
    state_svncheck.h \
    state_getpackagesfolder.h \
    state_getpackageslist.h \
    state_getversions.h \
    utils.h \
    state_applychanges.h \
    commentswidget.h

FORMS    += mainwidget.ui \
    commentswidget.ui

RC_FILE = pacheck.rc

OTHER_FILES += \
    pacheck.rc
