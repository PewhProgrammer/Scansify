#include "Scansify.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Scansify w;
	w.show();
	return a.exec();
}
