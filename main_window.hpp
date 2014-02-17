#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP


#include "server_manager.hpp"
#include "ui_main_window.h"

#include <QMainWindow>



namespace Ui
{class main_window;}

class main_window : public QMainWindow
{
	Q_OBJECT

	Ui::main_window* m_ui;

	server_manager m_servermgr;

public:
	main_window(QWidget* parent = nullptr) :
		QMainWindow{parent},
		m_ui{new Ui::main_window}
	{m_ui->setupUi(this);}

	~main_window()
	{delete m_ui;}

private:

};



#endif // MAIN_WINDOW_HPP
