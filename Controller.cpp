#include "Controller.h"
#include "KinectStream.h"
#include "pclviewer.h"


Controller::Controller(PCLViewer *pclv)
{
	m_pclviewer = pclv;
	m_kinectStream.initKinect();
}


Controller::~Controller()
{
}

void Controller::run()
{

	while (true) {
		m_kinectStream.Tick(2);
	}

	m_kinectStream.Shutdown();
	//ICP icp;
	//icp.executeICP();

	return;
}
