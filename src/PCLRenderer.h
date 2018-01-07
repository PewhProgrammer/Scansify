#ifndef NOMINMAX
# define NOMINMAX
#endif
#pragma once

#undef max

#include "glut.h"
#include <pcl/point_types.h>
#include <pcl\io\pcd_io.h>

class PCLRenderer
{
public:
	struct PCLOutput {
		int size;

	};

	PCLRenderer();
	~PCLRenderer();

	void drawPCLData(GLubyte * dest, PCLOutput* output);

	// gets PointCloud in PCL and stores it into buffer
private: 
	void getPCLData(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
};

