#include "main_window.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a{argc, argv};
	main_window w;
	w.show();

	return a.exec();
}
