#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T07:59:51
#
#-------------------------------------------------

QT       += core gui
QMAKE_LFLAGS += -no-pie
CONFIG   += console debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qsudoku
TEMPLATE = app
target.path = /usr/local/bin

SOURCES += main.cpp\
        sudokuhelper.cpp \
        sudokutile.cpp

HEADERS  += sudokuhelper.h \
    sudokutile.h

FORMS    += sudokuhelper.ui

INSTALLS += target
