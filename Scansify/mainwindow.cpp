#include "window.h"
#include "Kinect.h"
#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <thread>
#include <iostream>

using namespace std;

void MainWindow::run()
{
	while (true) {
		FS.Tick(2);
	}
	//ICP icp;
	//icp.executeICP();

	return;
}

MainWindow::MainWindow()
{
	// open console for log output
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);


	// Initializing processes
	QMenuBar *menuBar = new QMenuBar;
	QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
	QAction *addNew = new QAction(menuWindow);
	addNew->setText(tr("Add new"));
	menuWindow->addAction(addNew);
	connect(addNew, &QAction::triggered, this, &MainWindow::onAddNew);
	setMenuBar(menuBar);

	onAddNew();

	// for kinect
	if (FS.initKinect()) cout << "Kinect is not properly connected!";
	else cout << "Kinect is ready to stream";

	// open up thread for kinect stream
	std::thread t1(&MainWindow::run,this);
	t1.detach();
}

void MainWindow::onAddNew()
{
	if (!centralWidget())
		setCentralWidget(new Window(this));
	else
		QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
}

