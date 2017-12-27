#pragma once

#include "Kinect.h"
#include "iostream"

// size of screen
#define DEPTHWIDTH 512
#define DEPTHHEIGHT 424

typedef unsigned short uint16;

template<typename T>
void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

class FrameSource
{
public:
	FrameSource();
	~FrameSource();

	bool initKinect();
	void Tick(float deltaTime);
	void Shutdown();

private:
	// Kinect variables
	IKinectSensor* _sensor = nullptr;
	IDepthFrameReader* _depthFrameReader = nullptr;
	ICoordinateMapper* _mapper = nullptr;

	uint16 *_depthBuffer= nullptr;

	bool FrameSource::checkHR(HRESULT hr, std::string msg);
};

