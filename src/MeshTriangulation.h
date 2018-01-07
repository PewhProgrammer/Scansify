#ifndef NOMINMAX
# define NOMINMAX
#endif

#undef max

#include <pcl\PolygonMesh.h>
#include <pcl\io\pcd_io.h>

#pragma once
class MeshTriangulation
{
public:
	MeshTriangulation();
	~MeshTriangulation();

	void reconstruct(pcl::PolygonMesh* triangles, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
	void reconstruct(pcl::PolygonMesh* triangles, std::string path);
};

