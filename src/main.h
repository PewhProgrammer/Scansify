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
void triangulateMesh();
void mousePicking(float screenX, float screenY);
void changeCameraProperties(float eyex, float eyey, float eyez, float posx, float posy, float posz, float upx, float upy, float upz);
void changeFilterValues(double mincutoff, double beta);

pcl::PointCloud<pcl::PointXYZ>::Ptr scanData();
