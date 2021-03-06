#-------------------------------------------------
#
# Project created by QtCreator 2017-04-02T13:15:01
#
#-------------------------------------------------

QT       += core gui
QT       += svg
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlocksterIDE
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
    programcontainer.cpp \
    blockstersession.cpp \
    terminalwidget.cpp \
    programscene.cpp \
    block.cpp \
    inport.cpp \
    outport.cpp \
    libbrowser.cpp \
    signalline.cpp \
    signalsegment.cpp \
    libscene.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    programcontainer.h \
    blockstersession.h \
    terminalwidget.h \
    programscene.h \
    block.h \
    inport.h \
    outport.h \
    libbrowser.h \
    signalline.h \
    signalsegment.h \
    libscene.h \
    config.h

FORMS    += mainwindow.ui \
    programcontainer.ui \
    terminalwidget.ui \
    libbrowser.ui

DISTFILES += \
    vectors/blockster.svg

RESOURCES += \
    vectors/vector_graphics.qrc

INCLUDEPATH += ..\Blockster\src\blster
