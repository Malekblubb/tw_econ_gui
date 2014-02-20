//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#include "main_window.hpp"

#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a{argc, argv};
	main_window w;
	w.show();

	return a.exec();
}
