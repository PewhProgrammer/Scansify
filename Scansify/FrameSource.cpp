#include "FrameSource.h"
#include "Kinect.h"

#include "Windows.h"
#include "Ole2.h"

#include "iostream"
#include "string"



using namespace std;

FrameSource::FrameSource()
{
}


FrameSource::~FrameSource()
{
}


bool FrameSource::initKinect() {
	HRESULT hr;
	hr = GetDefaultKinectSensor(&_sensor);
	if (FAILED(hr)) {
		return false;
	}

	_sensor->Open();
	_sensor->get_CoordinateMapper(&_mapper);

	IDepthFrameSource* depthFrameSource;
	hr = _sensor->get_DepthFrameSource(&depthFrameSource);

	if (FAILED(hr)) {
		return false;
	}

	hr = depthFrameSource->OpenReader(&_depthFrameReader);
	if (FAILED(hr)) {
		return false;
	}

	SafeRelease(depthFrameSource);

	_depthBuffer = new uint16[512 * 424];

	return true;
}

void FrameSource::Tick(float deltaTime) {
	// update and draw

	HRESULT hr;

	IDepthFrame* depthFrame;
	hr = _depthFrameReader->AcquireLatestFrame(&depthFrame);


	if (FAILED(hr)) return;

	std::cout << "Copying Data ";
	hr = depthFrame->CopyFrameDataToArray(DEPTHHEIGHT * DEPTHWIDTH, _depthBuffer);
	if (checkHR(hr, "Couldn't copy frame data to array"));

	CameraSpacePoint* csp = new CameraSpacePoint[DEPTHHEIGHT*DEPTHWIDTH];

	// Get data from frame
	/*unsigned int sz;
	unsigned short* buf;
	depthFrame->AccessUnderlyingBuffer(&sz, &buf);*/
	hr = _mapper->MapDepthFrameToCameraSpace(DEPTHHEIGHT * DEPTHWIDTH, _depthBuffer, DEPTHHEIGHT * DEPTHWIDTH, csp);
	if (checkHR(hr, "Couldn't map from depth to camera space"));


	SafeRelease(depthFrame);

	// print depth data to log
	for (int i = 0; i < DEPTHHEIGHT * DEPTHWIDTH; i++) {
		uint16 d = _depthBuffer[i];
		CameraSpacePoint p = csp[i];
		cout << p.X << "," << p.Y << ", " << p.Z << "\n";
	}
}

bool FrameSource::checkHR(HRESULT hr, std::string msg) {
	if (FAILED(hr)) {
		std::cout << msg << "\n";
		return false;
	}

	return true;
}

void FrameSource::Shutdown() {

	delete[] _depthBuffer;
	SafeRelease(_depthFrameReader);
	SafeRelease(_sensor);
}


