#pragma once

#include "KinectStream.h"
#include "pclviewer.h"

class Controller
{
public:
	Controller(PCLViewer *pclv);
	~Controller();
	void run();

private:
	KinectStream m_kinectStream;
	PCLViewer* m_pclviewer;
};

