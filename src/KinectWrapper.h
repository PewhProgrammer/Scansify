#pragma once

#define width 424
#define height 512
#define colorwidth 1920
#define colorheight 1080

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
};

