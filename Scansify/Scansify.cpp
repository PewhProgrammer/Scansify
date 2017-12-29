#include "Scansify.h"
#include "Kinect.h"
#include "ICP.h"
#include "FrameSource.h";

#include "iostream";
#include "Qpalette.h";


Scansify::Scansify(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	
	Init();
}

Scansify::~Scansify()
{
	delete &ui;
}

void Scansify::run() {

	while (true) {
		FS.Tick(2);
	}
	//ICP icp;
	//icp.executeICP();

	return;
}

bool Scansify::Init()
{
	bool result = true;
	result = result && FS.initKinect();


	return result;
}
