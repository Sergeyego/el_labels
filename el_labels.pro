#-------------------------------------------------
#
# Project created by QtCreator 2013-09-17T16:30:38
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = el_labels
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    pgdialog.cpp \
    modelpart.cpp \
    lblengine.cpp \
    glabelslbl.cpp

HEADERS  += mainwidget.h \
    pgdialog.h \
    modelpart.h \
    lblengine.h \
    glabelslbl.h

FORMS    += mainwidget.ui \
    pgdialog.ui

RESOURCES += \
    res.qrc
