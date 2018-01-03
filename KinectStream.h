#pragma once

#include "Kinect.h"
#include "iostream"

// size of screen
#define DEPTHWIDTH 512
#define DEPTHHEIGHT 424

typedef unsigned short uint16;

template<typename T>
void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

struct Output {
	CameraSpacePoint* csp;
	int count;
};


class KinectStream
{
public:
	KinectStream();
	~KinectStream();

	bool initKinect();
	Output Tick(float deltaTime);
	void Shutdown();

private:
	// Kinect variables
	IKinectSensor * _sensor = nullptr;
	IDepthFrameReader* _depthFrameReader = nullptr;
	ICoordinateMapper* _mapper = nullptr;

	uint16 *_depthBuffer = nullptr;

	bool checkHR(HRESULT hr, std::string msg);

};

