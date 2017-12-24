#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Scansify.h"

class Scansify : public QMainWindow
{
	Q_OBJECT

public:
	Scansify(QWidget *parent = Q_NULLPTR);

private:
	Ui::ScansifyClass ui;
};
