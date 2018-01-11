#pragma once
#include <Windows.h>

#ifndef NOMINMAX
# define NOMINMAX
#endif
#undef max
#undef min
#include <pcl\io\vtk_lib_io.h>

#ifndef NOMINMAX
# define NOMINMAX
#endif
#undef max
#undef min

const int width = 512;
const int height = 424;
const int colorwidth = 1920;
const int colorheight = 1080;

void drawData();
pcl::PointCloud<pcl::PointXYZ>::Ptr scanData();
void triangulateMesh();
