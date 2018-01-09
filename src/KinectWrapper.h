#pragma once
#ifndef NOMINMAX
# define NOMINMAX
#endif
#undef max

#include "glut.h"
#include "Kinect.h"
#include <pcl\io\pcd_io.h>
#include "bbox.h"

class KinectWrapper
{
public:
	KinectWrapper();
	~KinectWrapper();

	bool initKinect();
	void getRGBData(IMultiSourceFrame* frame, GLubyte* dest);
	void getDepthData(IMultiSourceFrame* frame, GLubyte* dest);

	// should be able to process any pointcloud data
	void convertDepthDataToPCL(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
	// compute bbox around arms for filtering
	void computeRightArmBox(IMultiSourceFrame* frame, rt::BBox* box);
	void computeLeftArmBox(IMultiSourceFrame* frame, rt::BBox* box);

	HRESULT aquireLatestFrame(IMultiSourceFrame** frame);
private:

	// get left/right arm boundaries

};

