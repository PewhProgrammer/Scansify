#include "KinectStream.h"

#include "Kinect.h"

#include "Windows.h"
#include "Ole2.h"

#include "iostream"
#include "string"

using namespace std;


KinectStream::KinectStream()
{
}


KinectStream::~KinectStream()
{
}

bool KinectStream::initKinect()
{
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

Output KinectStream::Tick(float deltaTime)
{
	// update and draw

	HRESULT hr;

	IDepthFrame* depthFrame;
	hr = _depthFrameReader->AcquireLatestFrame(&depthFrame);

	Output failed;
	failed.count = 0;
	if (FAILED(hr))
		return failed ;


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
		//cout << p.X << "," << p.Y << ", " << p.Z << "\n";
	}

	Output result;
	result.csp = csp;
	result.count = DEPTHHEIGHT * DEPTHWIDTH;

	return result;
}

void KinectStream::Shutdown()
{
	delete[] _depthBuffer;
	SafeRelease(_depthFrameReader);
	SafeRelease(_sensor);
}

bool KinectStream::checkHR(HRESULT hr, std::string msg)
{
	if (FAILED(hr)) {
		std::cout << msg << "\n";
		return false;
	}

	return true;
}
