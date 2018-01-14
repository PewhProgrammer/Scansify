#undef max
#undef min

#include "main.h"
#include "glut.h"

#include <cmath>
#include <cstdio>

#include <Ole2.h>
#include "iostream"

#include <Kinect.h>
#include "KinectWrapper.h"

#include "MeshTriangulation.h"
#include "PCLRenderer.h"

#include "rt\bvh.h"
#include "rt\intersection.h"
#include "ray.h"

// We'll be using buffer objects to store the kinect point cloud
GLuint vboId;
GLuint cboId;

PCLRenderer rend;
KinectWrapper kinectWrapper;
MeshTriangulation meshT;

rt::Point cameraPos(0,0,0.5f);
rt::Vector cameraUp(0, 1.f, 0);
rt::Vector cameraFocal(0, 0,-1.f);

rt::BVH *scene = new rt::BVH(); 
bool buildScene = true;

// looks like [512, 424]
void mousePicking(float screenX, float screenY) {
	std::cout << "mouse click on: (" << screenX << ", " << screenY << ") " << std::endl;

	// base formula for range interpolation: Result := ((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow;



	float x = ((screenX - 0) / (512.f - 0)) * (0.5f + 0.5f) - 0.5f;
	float y = ((screenY - 0) / (424.f - 0)) * (0.5f + 0.5f) - 0.5f;
	float z = 1;



	std::cout << "shooting ray: Pos(" << x << ", " << y << ", " << z << ")    Dir(" << cameraFocal.x << ", " << cameraFocal.y << ", " << cameraFocal.z << ")" << std::endl;


	const rt::Point o(x,y,z);

	rt::Ray ray(o,
		cameraFocal.normalize());
	

	rt::Intersection hit = scene->intersect(ray);
	if (hit)
		std::cout << "Hit!";


}

pcl::PointCloud<pcl::PointXYZ>::Ptr scanData()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	kinectWrapper.convertDepthDataToPCL(cloud);

	size_t k = cloud->size();
	if (k > 0) std::cout << "Scan completed: " << k << " vertices " << std::endl;
	else std::cout << "Scan was insuccessful!" << std::endl;
	return cloud;
}

void triangulateMesh() {
	//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = scanData();

	// Load input file into a PointCloud<T> with an appropriate type
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PCLPointCloud2 cloud_blob;
	pcl::io::loadPCDFile("Resources/bun0.pcd", cloud_blob);
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);
	//* the data should be available in cloud

	pcl::PolygonMesh* mesh = new pcl::PolygonMesh();
	meshT.reconstruct(mesh, cloud);
	if (cloud->size() == 0) return;

	pcl::io::savePolygonFileSTL("../models/subject.stl", *mesh, false);
}


void getKinectData() {
	IMultiSourceFrame* frame = NULL;
	if (SUCCEEDED(kinectWrapper.aquireLatestFrame(&frame))) {
		GLubyte* ptr;
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		ptr = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (ptr) {
			kinectWrapper.getDepthData(frame,ptr);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, cboId);
		ptr = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (ptr) {
			kinectWrapper.getRGBData(frame, ptr);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	if (frame) frame->Release();
}

void drawPCLData() {
	PCLRenderer::PCLOutput* out = new PCLRenderer::PCLOutput;
	GLubyte* ptr;
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	ptr = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	rend.drawPCLData(ptr, out);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	//draw everything white
	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	ptr = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	float* fdest = (float*)ptr;
	for (int i = 0; i < out->size; i++) {
		*fdest++ = 255;
		*fdest++ = 0;
		*fdest++ = 123;

		if (buildScene) {
			//build bvh
			pcl::PointXYZ p = (*out->points)[i];
			scene->add(rt::Point(p.x, p.y, p.z));
		}
	}
	if (buildScene) {
		buildScene = false;
		scene->rebuildIndex();
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void rotateCamera() {
	static double angle = 0.;
	static double radius = 3.;
	double x = radius*sin(angle);
	double z = radius*(1-cos(angle)) - radius/2;
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(x,0,z,0,0,radius/2,0,1,0);
	angle += 0.002;
}

void bufferAxis() {
	GLubyte* dest;
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	dest = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	float* fdest = (float*)dest;
	*fdest++ = 0.5f;
	*fdest++ = 0;
	*fdest++ = 0;
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	dest = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	fdest = (float*)dest;
	*fdest++ = 140;
	*fdest++ = 0;
	*fdest++ = 255;
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void drawData() {

	//rotateCamera();
	//getKinectData();
	drawPCLData();
	bufferAxis();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	glColorPointer(3, GL_FLOAT, 0, NULL);

	glPointSize(1.f);
	glDrawArrays(GL_POINTS, 0, width*height);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf(""+err);
	}
	
}


#include <thread>
#include <QtWidgets\qapplication.h>
#include "ConfigUI\ConfigUI.h"

void initQT(int argc, char* argv[]) {
	QApplication app(argc, argv);

	ConfigUI window;
	window.show();

	app.exec();
	return;
}


int main(int argc, char* argv[]) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	printf("Log Console.");
	std::cout.put('\n');

	std::thread t1(initQT, argc, argv);
	t1.detach();

    if (!init(argc, argv)) return 1;
    //if (!initKinect()) return 1;
	if (!kinectWrapper.initKinect()) return 1;

    // OpenGL setup
    glClearColor(0,0,0,0);
    glClearDepth(1.0f);

	// Set up array buffers
	const int dataSize = width*height * 3 * 4;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &cboId);
	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);

    // Camera setup
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45, width /(GLdouble) height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.00001f,0.0000001f,-1,0,0, 0.5f,0,1,0);

    // Main loop
    execute();
    return 0;
}
