#-------------------------------------------------
#
# Project created by QtCreator 2016-06-19T09:29:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HostsFileEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ipaddrdelegate.cpp \
    domaindelegate.cpp

HEADERS  += mainwindow.h \
    ipaddrdelegate.h \
    domaindelegate.h

FORMS    += mainwindow.ui

RESOURCES += \
    app.qrc

RC_FILE += icon.rc\

DISTFILES += \
    icon.rc \
    uac.manifest \
    docs/screenshots/20160621165811.png \
    app.ico
