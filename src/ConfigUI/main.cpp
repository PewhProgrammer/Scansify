#include "ConfigUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ConfigUI w;
	w.show();
	return a.exec();
}
