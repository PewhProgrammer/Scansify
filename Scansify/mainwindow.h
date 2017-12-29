#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FrameSource.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
	FrameSource FS;
	void run();

private slots:
    void onAddNew();

};

#endif
