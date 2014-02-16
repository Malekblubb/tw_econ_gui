#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>

namespace Ui {
	class main_window;
}

class main_window : public QMainWindow
{
	Q_OBJECT

public:
	explicit main_window(QWidget *parent = 0);
	~main_window();

private:
	Ui::main_window *ui;
};

#endif // MAIN_WINDOW_HPP
