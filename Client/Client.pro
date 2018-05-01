#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T19:00:49
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    signdialog.cpp \
    findpassddialog.cpp \
    state.cpp \
    linkserverdialog.cpp \
    handler.cpp \
    itemtable.cpp \
    fileutil.cpp \
    timeutil.cpp \
    progressdialog.cpp \
    log.cpp \
    friendlistview.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    signdialog.h \
    findpassddialog.h \
    state.h \
    linkserverdialog.h \
    handler.h \
    itemtable.h \
    fileutil.h \
    timeutil.h \
    progressdialog.h \
    log.h \
    friendlistview.h \
    filesr.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    signdialog.ui \
    findpassddialog.ui \
    linkserverdialog.ui \
    progressdialog.ui

RESOURCES += \
    icon.qrc

DISTFILES += \
    style/login.qss
RC_FILE=icon.rc
 