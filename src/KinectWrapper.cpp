#include "KinectWrapper.h"
#include <iostream>
#include "core\point.h"
#include "OneEuroFilter.h"

#define width 424
#define height 512
#define colorwidth 1920
#define colorheight 1080

// Intermediate Buffers
unsigned char rgbimage[colorwidth*colorheight * 4];    // Stores RGB color image
ColorSpacePoint depth2rgb[width*height];             // Maps depth pixels to rgb pixels
CameraSpacePoint depth2xyz[width*height];			 // Maps depth pixels to 3d coordinates
CameraSpacePoint depthfiltered[width*height];			 // stores filtered pixels

// Kinect Variables
IKinectSensor* sensor;             // Kinect sensor
IMultiSourceFrameReader* reader;   // Kinect data source
ICoordinateMapper* mapper;         // Converts between depth, color, and 3d coordinates

// Body tracking variables
BOOLEAN tracked;                            // Whether we see a body
Joint joints[JointType_Count];              // List of joints in the tracked body

// Filtering Objects
std::vector<OneEuroFilter> m_filters;

// custom var
unsigned int pointcloudSize = 0; 
bool trackedJoints = false;

rt::BBox rightArmBox = rt::BBox::empty();
rt::BBox leftArmBox = rt::BBox::empty();

enum Features {

	tipRight,
	thumbRight,
	handRight,
	wristRight,

	tipLeft,
	thumbLeft,
	handLeft,
	wristLeft,
	ElbowLeft
};

	
KinectWrapper::KinectWrapper()
{
	m_filters.reserve(Features::ElbowLeft + 1);
	for (int i = 0; i <= Features::ElbowLeft; i++) {
		OneEuroFilter e(20); // Hz in params
		m_filters.push_back(e);
	}
}


KinectWrapper::~KinectWrapper()
{
}

bool KinectWrapper::initKinect()
{
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->get_CoordinateMapper(&mapper);

		sensor->Open();
		sensor->OpenMultiSourceFrameReader(
			FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color| FrameSourceTypes::FrameSourceTypes_Body ,
			&reader);
		return reader;
	}
	else {
		return false;
	}
	return true;
}

void KinectWrapper::getRGBData(IMultiSourceFrame* frame, GLubyte* dest)
{
	IColorFrame* colorframe;
	IColorFrameReference* frameref = NULL;
	frame->get_ColorFrameReference(&frameref);
	frameref->AcquireFrame(&colorframe);
	if (frameref) frameref->Release();

	if (!colorframe) return;
	

	// Get data from frame
	colorframe->CopyConvertedFrameDataToArray(colorwidth*colorheight * 4, rgbimage, ColorImageFormat_Rgba);

	// Write color array for vertices
	float* fdest = (float*)dest;
	for (int i = 0; i < width*height; i++) {
		
		ColorSpacePoint p = depth2rgb[i];
		// Check if color pixel coordinates are in bounds
		if ((p.X < 0 || p.Y < 0 || p.X > colorwidth || p.Y > colorheight)
			|| !checkBBox(depth2xyz[i].X, depth2xyz[i].Y, depth2xyz[i].Z, &leftArmBox)
			  /*&& !checkBBox(depth2xyz[i].X, depth2xyz[i].Y, depth2xyz[i].Z, &rightArmBox) */) {
			*fdest++ = 0;
			*fdest++ = 0;
			*fdest++ = 0;
		}
		else {
			int idx = (int)p.X + colorwidth * (int)p.Y;
			/*
			*fdest++ = rgbimage[4 * idx + 0] / 255.;
			*fdest++ = rgbimage[4 * idx + 1] / 255.;
			*fdest++ = rgbimage[4 * idx + 2] / 255.;
			*/
			
			*fdest++ = 0.84f;
			*fdest++ = 0.13f;
			*fdest++ = 0.61f;
			
		}
	
		// Don't copy alpha channel
	}

	if (colorframe) colorframe->Release();
}

void KinectWrapper::getDepthData(IMultiSourceFrame* frame, GLubyte* dest)
{
	
	IDepthFrame* depthframe;
	IDepthFrameReference* frameref = NULL;
	frame->get_DepthFrameReference(&frameref);
	frameref->AcquireFrame(&depthframe);
	if (frameref) frameref->Release();

	if (!depthframe) return;

	// Get data from frame
	unsigned int sz;
	unsigned short* buf;
	depthframe->AccessUnderlyingBuffer(&sz, &buf);
	
	// create box around arms
	rightArmBox = rt::BBox::empty();
	leftArmBox = rt::BBox::empty();
	computeRightArmBox(frame, &rightArmBox);
	computeLeftArmBox(frame, &leftArmBox);

	// Write vertex coordinates
	mapper->MapDepthFrameToCameraSpace(width*height, buf, width*height, depth2xyz);
	float* fdest = (float*)dest;
	pointcloudSize = 0;
	for (int i = 0; i < sz ; i++) {
		float x = depth2xyz[i].X;
		float y = depth2xyz[i].Y;
		float z = depth2xyz[i].Z;

		*fdest++ = x;
		*fdest++ = y;
		*fdest++ = z;

		if (checkBBox(x, y, z, &leftArmBox) /* ||
			checkBBox(x, y, z, &rightArmBox) */) {
			depthfiltered[pointcloudSize] = depth2xyz[i];
			pointcloudSize++;

			//std::cout << i << ": " << *(buf+i) << "\n";
		}
	}

	// Fill in depth2rgb map
	mapper->MapDepthFrameToColorSpace(width*height, buf, width*height, depth2rgb);
	if (depthframe) depthframe->Release();
}



void KinectWrapper::convertDepthDataToPCL(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
	// wont do anything if pcS < 0
	for (int i = 0; i < pointcloudSize; i++) {
		pcl::PointXYZ p(depthfiltered[i].X, depthfiltered[i].Y, depthfiltered[i].Z);
		cloud->push_back(p);
	}
}

void KinectWrapper::computeRightArmBox(IMultiSourceFrame * frame, rt::BBox* box)
{
	IBodyFrame* bodyframe;
	IBodyFrameReference* frameref = NULL;
	frame->get_BodyFrameReference(&frameref);
	frameref->AcquireFrame(&bodyframe);
	if (frameref) frameref->Release();

	if (!bodyframe) return;

	// ------ NEW CODE ------
	IBody *body[BODY_COUNT] = { 0 };
	HRESULT hr = bodyframe->GetAndRefreshBodyData(_countof(body), body);
	for (int i = 0; i < BODY_COUNT; i++) {
		body[i]->get_IsTracked(&tracked);
		if (tracked) {
			body[i]->GetJoints(JointType_Count, joints);

			CameraSpacePoint handTip = joints[JointType_HandTipRight].Position;
			CameraSpacePoint handThumb = joints[JointType_ThumbRight].Position;
			CameraSpacePoint hand = joints[JointType_HandRight].Position;
			CameraSpacePoint handWrist = joints[JointType_WristRight].Position;

			box->extend(m_filters[Features::tipRight].filter(handTip));
			box->extend(m_filters[Features::thumbRight].filter(handThumb));
			box->extend(m_filters[Features::handRight].filter(hand));
			box->extend(m_filters[Features::wristRight].filter(handWrist));

			//rt::Point p = m_filters[Features::tipRight].filter(handTip);
			//std::cout << "(" << p.x << ", " << p.z << " ," << p.z << ")" << std::endl;
			
			break;
		}
	}
	// ------ END NEW CODE ------

	if (bodyframe) bodyframe->Release();
}

void KinectWrapper::computeLeftArmBox(IMultiSourceFrame * frame, rt::BBox* box)
{
	IBodyFrame* bodyframe;
	IBodyFrameReference* frameref = NULL;
	frame->get_BodyFrameReference(&frameref);
	frameref->AcquireFrame(&bodyframe);
	if (frameref) frameref->Release();

	if (!bodyframe) return;

	// ------ NEW CODE ------
	IBody *body[BODY_COUNT] = { 0 };
	HRESULT hr = bodyframe->GetAndRefreshBodyData(_countof(body), body);
	for (int i = 0; i < BODY_COUNT; i++) {
		body[i]->get_IsTracked(&tracked);
		if (tracked) {
			body[i]->GetJoints(JointType_Count, joints);

			CameraSpacePoint handTip = joints[JointType_HandTipLeft].Position;
			CameraSpacePoint handThumb = joints[JointType_ThumbLeft].Position;
			CameraSpacePoint hand = joints[JointType_HandLeft].Position;
			CameraSpacePoint handWrist = joints[JointType_WristLeft].Position;
			CameraSpacePoint handElbow = joints[JointType_ElbowLeft].Position;

			
			box->extend(rt::Point(handTip.X, handTip.Y, handTip.Z));
			box->extend(rt::Point(handThumb.X, handThumb.Y, handThumb.Z));
			box->extend(rt::Point(hand.X, hand.Y, hand.Z));
			box->extend(rt::Point(handWrist.X, handWrist.Y, handWrist.Z));
			box->extend(rt::Point(handElbow.X, handElbow.Y, handElbow.Z));
			
			/*
			rt::Point handtip_filtered = m_filters[Features::tipLeft].filter(handTip); 
			rt::Point handthumb_filtered = m_filters[Features::thumbLeft].filter(handThumb);
			rt::Point hand_filtered = m_filters[Features::handLeft].filter(hand);
			rt::Point handWrist_filtered = m_filters[Features::wristLeft].filter(handWrist);
			rt::Point handElbow_filtered = m_filters[Features::ElbowLeft].filter(handElbow);

			box->extend(handtip_filtered);
			box->extend(handthumb_filtered);
			box->extend(hand_filtered);
			box->extend(handWrist_filtered);
			box->extend(handElbow_filtered);
			*/

			//printf("Before: (%6.3f,%6.3f,%6.3f)		After: (%6.3f,%6.3f,%6.3f)  \n", handTip.X, handTip.Y, handTip.Z , p_filtered.x, p_filtered.y, p_filtered.z);

			break;
		}
	}
	// ------ END NEW CODE ------

	if (bodyframe) bodyframe->Release();
}

bool KinectWrapper::checkBBox(float x, float y, float z, rt::BBox * box)
{
	return 
		(z > box->min.z - 0.8f) && (z < box->max.z + 0.08f ) &&
		(y > box->min.y - 0.073f) && (y < box->max.y + 0.07f)  &&
		(x > box->min.x - 0.065f) && (x < box->max.x + 0.035f)
		;
	/* for finger only
	return (z > box->min.z - 0.8f) && (z < box->max.z + 0.08f) &&
		(y > box->min.y - 0.033f) && (y < box->max.y + 0.07f) &&
		(x > box->min.x - 0.025f) && (x < box->max.x + 0.015f)
		;
	*/
}

HRESULT KinectWrapper::aquireLatestFrame(IMultiSourceFrame** frame)
{
	HRESULT k = reader->AcquireLatestFrame(frame);
	return k;
}

void KinectWrapper::changeFilterValues(float mincutoff, float beta)
{
	for (int i = 0; i < m_filters.size(); i++) {
		m_filters[i].changeFilterValues(mincutoff, beta);
	}
}


