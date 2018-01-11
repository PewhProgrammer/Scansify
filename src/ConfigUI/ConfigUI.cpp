#include "ConfigUI.h"
#include "../main.h"
#include <iostream>
#include <string>
#include <windows.h>

ConfigUI::ConfigUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, [this] {
		std::cout << "Triangulating to a mesh..." << std::endl;
		triangulateMesh();
/*
		TCHAR buff[MAX_PATH];
		GetModuleFileName(NULL, buff, _countof(buff));
		std::cout << buff << std::endl;

		char a[] = "Scansify\\";

		char *output = NULL;
		output = strstr(buff, a);
		long long f = output - buff;
		char*k = buff + 39;
		if (output) {
			printf("String Found");
		}
*/

		std::cout << "Triangulation successfully completed. Stored in \"Scansify/models/subject.stl\""  << std::endl;

	});
	connect(ui.pushButton_2, &QPushButton::clicked, [this] {
		std::cout << "reset" << std::endl;
	});
	connect(ui.pushButton_4, &QPushButton::clicked, [this] {
		scanData();
	});

	connect(ui.horizontalSlider, &QSlider::valueChanged, [this] {
		std::cout << ui.horizontalSlider->value() << std::endl;
	});
	connect(ui.horizontalSlider_2, &QSlider::valueChanged, [this] {
		std::cout << ui.horizontalSlider_2->value() << std::endl;
	});
	
}

