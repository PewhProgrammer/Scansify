#undef max
#include "PCLRenderer.h"

#include <pcl/point_types.h>
#include <pcl\io\pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl\io\vtk_lib_io.h>

#include "MeshTriangulation.h"


bool run = true;
PCLRenderer::PCLRenderer()
{
}


PCLRenderer::~PCLRenderer()
{
}

void PCLRenderer::drawPCLData(GLubyte * dest, PCLOutput* out)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	getPCLData(cloud);

	size_t cloudSize = cloud->size();
	out->size = (int)cloudSize;
	out->points = cloud;

	float* fdest = (float*)dest;
	for (int i = 0; i < cloudSize; i++) {
		pcl::PointXYZ p = cloud->operator[](i);
		*fdest++ = p.x;
		*fdest++ = p.y;
		*fdest++ = p.z;
	}

	if (run) {
		MeshTriangulation meshT;
		pcl::PolygonMesh* mesh = new pcl::PolygonMesh();
		meshT.reconstruct(mesh, cloud);

		pcl::io::savePolygonFileSTL("subject.stl", *mesh, false);
		run = false;
	}
	
}

void PCLRenderer::getPCLData(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
	// Load input file into a PointCloud<T> with an appropriate type
	pcl::PCLPointCloud2 cloud_blob;
	pcl::io::loadPCDFile("Resources/bun0.pcd", cloud_blob);
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);
	//* the data should be available in cloud
}
