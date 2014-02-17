#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T19:38:27
#
#-------------------------------------------------

QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++0x

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tw_econ_gui
TEMPLATE = app


SOURCES	+=  main.cpp
HEADERS +=  $$system("find ./ -name '*.hpp'")
FORMS	+=  main_window.ui
