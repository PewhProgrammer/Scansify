#pragma once

#include "glut.h"
#include "Kinect.h"

class KinectWrapper
{
public:
	KinectWrapper();
	~KinectWrapper();

	bool initKinect();
	void getRGBData(IMultiSourceFrame* frame, GLubyte* dest);
	void getDepthData(IMultiSourceFrame* frame, GLubyte* dest);
	HRESULT aquireLatestFrame(IMultiSourceFrame** frame);
};

