#include "ConfigUI.h"
#include "../main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <qapplication.h>
#include <qdesktopwidget.h>

ConfigUI::ConfigUI(QWidget *parent)
	: QMainWindow(parent)

	
{
	ui.setupUi(this);

	QRect rec = QApplication::desktop()->screenGeometry();
	float height = rec.height();
	float width = rec.width();

	move(width - 380, height / 2 - 300);

	connect(ui.pushButton, &QPushButton::clicked, [this] {
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

	});
	connect(ui.pushButton_2, &QPushButton::clicked, [this] {
		std::cout << "reset" << std::endl;
	});
	connect(ui.pushButton_4, &QPushButton::clicked, [this] {
		scanData();
	});

	connect(ui.horizontalSlider, &QSlider::valueChanged, [this] {
		float minc = ui.horizontalSlider->value() / 100.f;
		float beta = ui.horizontalSlider_2->value() / 100.f;
		ui.doubleSpinBox->setValue(minc);
		changeFilterValues(minc, beta);
		std::cout << minc << std::endl;
	});
	connect(ui.horizontalSlider_2, &QSlider::valueChanged, [this] {
		float minc = ui.horizontalSlider->value() / 100.f;
		float beta = ui.horizontalSlider_2->value() / 100.f;
		ui.doubleSpinBox_2->setValue(beta);
		changeFilterValues(minc, beta);
		std::cout << beta << std::endl;
	});

	QList<QLineEdit *> edits;
	edits.push_back(ui.cameraPosXEdit);
	edits.push_back(ui.cameraPosYEdit);
	edits.push_back(ui.cameraPosZEdit);
	edits.push_back(ui.cameraEyeXEdit);
	edits.push_back(ui.cameraEyeXEdit);
	edits.push_back(ui.cameraEyeXEdit);
	edits.push_back(ui.cameraUpXEdit);
	edits.push_back(ui.cameraUpXEdit);
	edits.push_back(ui.cameraUpXEdit);

	for (int i = 0; i < edits.size(); i++) {
		connect(edits[i], &QLineEdit::textChanged, [this] {
			changeCameraProperties(
				ui.cameraEyeXEdit->text().toDouble(), ui.cameraEyeYEdit->text().toDouble(), ui.cameraEyeZEdit->text().toDouble(),
				ui.cameraPosXEdit->text().toDouble(), ui.cameraPosYEdit->text().toDouble(), ui.cameraPosZEdit->text().toDouble(),
				ui.cameraUpXEdit->text().toDouble(), ui.cameraUpYEdit->text().toDouble(), ui.cameraUpZEdit->text().toDouble());
		});
	}
	
}

