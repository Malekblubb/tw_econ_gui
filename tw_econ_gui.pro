#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T19:38:27
#
#-------------------------------------------------

!win32{
QMAKE_CXX = clang++
}
QMAKE_CXXFLAGS	 += -std=c++0x
QT		 += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET	    = tw_econ_gui
TEMPLATE    = app


win32{
LIBS += -lws2_32
}

INCLUDEPATH +=	./include \
		./external/mlk/include
SOURCES	+=  main.cpp
HEADERS +=  main_window.hpp \
	    include/twec/data_parser.hpp \
	    include/twec/player_info.hpp \
	    include/twec/server_manager.hpp \
	    include/twec/telnet_client.hpp \
	    include/twec/twecon_client.hpp
FORMS	+=  main_window.ui
