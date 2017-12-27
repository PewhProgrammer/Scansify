#include "Scansify.h"
#include <QtWidgets/QApplication>

#include <thread>
#include <string>
#include <iostream>



int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	Scansify w;
	w.show();

	

	/*std::thread t1(&Scansify::run, std::ref(w));
	t1.detach();*/

	// gui runs forever and closes application with its exit code
	return a.exec();

}


