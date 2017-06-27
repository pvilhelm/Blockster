#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T11:05:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlocksterIDE
TEMPLATE = app
CONFIG += c++14

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
    program_container.cpp \
    program_scene.cpp \
    ../Blockster/src/blster/b_block.cpp \
    ../Blockster/src/blster/b_node.cpp \
    ../Blockster/src/blster/b_program_tree.cpp \
    ../Blockster/src/blster/b_task.cpp \
    ../Blockster/src/blster/b_xml.cpp \
    ../libs/pugixml/src/pugixml.cpp \
    block_item.cpp \
    port_item.cpp \
    lib_scene.cpp \
    ../Blockster/src/blster/b_lib_tree.cpp \
    lib_container.cpp

HEADERS  += mainwindow.h \
    program_container.h \
    program_scene.h \
    block_item.h \
    port_item.h \
    lib_scene.h \
    lib_container.h

INCLUDEPATH += ../Blockster/src/blster/ \
    ../libs/pugixml/src/ \
    C:/Repos/boost/boost_1_64_0

LIBS += "-LC:/Repos/boost/boost_1_64_0/mscv64-14.1/lib"
