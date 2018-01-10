#include "ConfigUI.h"
#include "../main.h"
#include <iostream>

ConfigUI::ConfigUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, [this] {
		scanData();
		std::cout << "Scan successfully completed" << std::endl;
	});
	connect(ui.pushButton_2, &QPushButton::clicked, [this] {
		std::cout << "reset" << std::endl;
	});
	connect(ui.pushButton_4, &QPushButton::clicked, [this] {
		std::cout << "mesh triangulation" << std::endl;
	});

	connect(ui.horizontalSlider, &QSlider::valueChanged, [this] {
		std::cout << ui.horizontalSlider->value() << std::endl;
	});
	connect(ui.horizontalSlider_2, &QSlider::valueChanged, [this] {
		std::cout << ui.horizontalSlider_2->value() << std::endl;
	});
}
