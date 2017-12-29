#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Scansify.h"
#include "FrameSource.h"

class Scansify : public QMainWindow
{

public:
	Scansify(QWidget *parent = Q_NULLPTR);
	~Scansify();
	void run();

private:
	Ui::MainWindow ui;
	FrameSource FS;

	bool Init();
};
