#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T19:38:27
#
#-------------------------------------------------

!win32{
QMAKE_CXX = clang++
}
QMAKE_CXXFLAGS += -std=c++0x

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tw_econ_gui
TEMPLATE = app

INCLUDEPATH +=	./include \
		./external/mlk/include

SOURCES	+=  main.cpp
HEADERS +=  *.hpp \
	    include/twec/*.hpp
FORMS	+=  main_window.ui
