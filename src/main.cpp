#undef max
#undef min

#include "main.h"
#include "glut.h"

#include <cmath>
#include <cstdio>

#include <boost\thread\thread.hpp>
#include <thread>

#include <Ole2.h>

#include <Kinect.h>
#include "KinectWrapper.h"

#include "MeshTriangulation.h"
#include "PCLRenderer.h"
#include <pcl/filters/voxel_grid.h>

#include "rt\bvh.h"
#include "rt\intersection.h"
#include "ray.h"
#include "rt\cameras\perspective.h"

#include "ChangeCameraCommand.h"
#include "ICommand.h"
#include "iostream"

// We'll be using buffer objects to store the kinect point cloud
GLuint vboId;
GLuint cboId;

PCLRenderer rend;
KinectWrapper kinectWrapper;
MeshTriangulation meshT;

rt::Point cameraPos(0,0,0.5f);
rt::Vector cameraUp(0, 1.f, 0);
rt::Vector cameraFocal(0, 0,-1.f);
rt::PerspectiveCamera* cam;

rt::BVH *scene = new rt::BVH(); 

bool buildScene = true;
bool processing = true;
float fovy = 0;
GLdouble ratio = 0;

// Command Queue
std::queue<ICommand *> commands;

/*
The different color codes are

0   BLACK
1   BLUE
2   GREEN
3   CYAN
4   RED
5   MAGENTA
6   BROWN
7   LIGHTGRAY
8   DARKGRAY
9   LIGHTBLUE
10  LIGHTGREEN
11  LIGHTCYAN
12  LIGHTRED
13  LIGHTMAGENTA
14  YELLOW
15  WHITE
*/

void processingConsoleOutput(int color, std::string text)
{
	//system("CLS");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int i = 0;
	processing = true;
	while (processing) {
		if(i % 2 == 0) SetConsoleTextAttribute(handle, color);
		else SetConsoleTextAttribute(handle, 0);
		cout << text << '\r';

		SetConsoleTextAttribute(handle, 7);
		i++;

		boost::this_thread::sleep(boost::posix_time::seconds(1.75f));
	}
}

// looks like [512, 424]
void mousePicking(float screenX, float screenY) {
	//std::cout << "mouse click on: (" << screenX << ", " << screenY << ") " << std::endl;

	// base formula for range interpolation: Result := ((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow;
	float outputLow = 1;
	float outputHigh = -1;
 	float x = ((screenX - 0) / (512.f - 0)) * (outputHigh - outputLow) + outputLow;
	float y = ((screenY - 0) / (424.f - 0)) * (outputHigh - outputLow) + outputLow;
	float z = cameraPos.z;

	const rt::Point o(x ,y, z );
	rt::Ray ray = cam->getPrimaryRay(x, y);

	
	
	std::cout << "shooting ray: Pos(" << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << ")    Dir(" << ray.d.x << ", " << ray.d.y << ", " << ray.d.z << ")" ;
	rt::Intersection hit = scene->intersect(ray);
	if (hit)	 std::cout << "        ===>       Hit!" ;
	std::cout << std::endl;
}

pcl::PointCloud<pcl::PointXYZ>::Ptr scanData()
{
	std::thread t1(processingConsoleOutput, 14, "Scanning the object ...");
	t1.detach();

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	kinectWrapper.convertDepthDataToPCL(cloud);
	processing = false;


	size_t k = cloud->size();
	if (k > 0) {
		std::cout << "Scan completed.                               " << std::endl;
		std::cerr << "PointCloud before filtering: " << cloud->width * cloud->height
			<< " data points (" << pcl::getFieldsList(*cloud) << ")." << std::endl;
	}
	else
		std::cout << "Scan was insuccessful!" << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

	// Create the filtering object
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud(cloud);
	sor.setLeafSize(0.01f, 0.01f, 0.01f);
	sor.filter(*cloud_filtered);

	std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height
		<< " data points (" << pcl::getFieldsList(*cloud_filtered) << ")." << std::endl;
	
	return cloud_filtered;
}

void triangulateMesh() {


	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = scanData();

	std::thread t1(processingConsoleOutput, 14, "Triangulating to mesh ...");
	t1.detach();

	/*
	// Load input file into a PointCloud<T> with an appropriate type
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PCLPointCloud2 cloud_blob;
	pcl::io::loadPCDFile("Resources/bun0.pcd", cloud_blob);
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);
	//* the data should be available in cloud
	

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

	// Create the filtering object
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud(cloud);
	sor.setLeafSize(0.015f, 0.015f, 0.015f);
	sor.filter(*cloud_filtered);
	
	
	rt::BBox scanBox;
	for (int i = 0; i < cloud_filtered->size(); i++) {
		pcl::PointXYZ p = (*cloud_filtered)[i];
		scanBox.extend(rt::Point(
			p.x,p.y,p.z));
	}
	*/

	pcl::PolygonMesh* mesh = new pcl::PolygonMesh();
	meshT.reconstruct(mesh, cloud);
	
	
	if (cloud->size() == 0) return;
	processing = false;
	//scanBox = rt::BBox::full();

	// clean up poisson surfaces
	/*
	for (std::vector<pcl::Vertices>::iterator it = mesh->polygons.begin(); it != mesh->polygons.end();) {
		pcl::Vertices p = *it;

		if (!scanBox.contains(rt::Point(p.vertices[0], p.vertices[1], p.vertices[2]))) {
			it = mesh->polygons.erase(it);
		}
		else ++it;

	}
	*/

	std::string modelPath = "/models/subject_poisson.stl";
	pcl::io::savePolygonFileSTL(".." + modelPath, *mesh, false);
	std::cout << "Triangulation successfully completed. Stored in \"Scansify"+ modelPath + "\"" << std::endl;
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
		rt::Point max = scene->getBounds().max;
		rt::Point min = scene->getBounds().min;
		std::cout << "build bvh out of (max,min) " << max[0] << ", " << max[1] << ", " << max[2] << " | " << min[0] << ", " << min[1] << ", " << min[0] << std::endl;
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
	angle += 0.02;
}

void bufferAxis() {
	GLubyte* dest;
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	dest = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	float* fdest = (float*)dest;
	*fdest++ = 0.5f;
	*fdest++ = 0.41f;
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

void changeFilterValues(double mincutoff, double beta) {
	kinectWrapper.changeFilterValues(mincutoff, beta);
}

void changeCameraProperties(float eyex, float eyey, float eyez, float posx, float posy, float posz, float upx, float upy, float upz) {
	cameraFocal.x = eyex;	cameraFocal.y = eyey;	cameraFocal.z = eyez;
	cameraPos.x = posx;		cameraPos.y = posy;		cameraPos.z = posz;
	cameraUp.x = upx;		cameraUp.y = upy;		cameraUp.z = upz;


	// Init cam - dummy values for openingAngle
	cam = new rt::PerspectiveCamera(cameraPos, cameraFocal, cameraUp, fovy * 2, fovy * 2 * ratio);

	commands.push(new ChangeCameraCommand(eyex, eyey, eyez, posx, posy, posz, upx,upy, upz));
	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, posx, posy, posz, upx, upy, upz);
	*/
}

float* getCameraProperties() {
	float *p = (float*)malloc(sizeof(float) * 9);

	*p++ = cameraPos[0];
	*p++ = cameraPos[1];
	*p++ = cameraPos[2];
	*p++ = cameraFocal[0];
	*p++ = cameraFocal[1];
	*p++ = cameraFocal[2];
	*p++ = cameraUp[0];
	*p++ = cameraUp[1];
	*p++ = cameraUp[2];

	return p - 9;
}


void drawData() {

	while (!commands.empty()) {
		ICommand* c = commands.front();
		if (c->verify()) {
			c->execute();
		}
		commands.pop();
	}

	//rotateCamera();
	getKinectData();
	//drawPCLData();
	//bufferAxis();

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
		printf("openGL error: "+err);
	}
	
}

#include <QtWidgets\qapplication.h>
#include "ConfigUI\ConfigUI.h"

#include "DarkStyle.h"

void initQT(int argc, char* argv[]) {
	//QApplication::setStyle("Fusion");
	QApplication app(argc, argv);

	app.setStyle(new DarkStyle());
	//FramelessWindow framelessWindow;
	ConfigUI window;
	//framelessWindow.setContent(window);
	window.show();

	app.exec();
	return;
}



int main(int argc, char* argv[]) {

	//HWND consoleWindow = GetConsoleWindow();
	//SetWindowPos(consoleWindow, 0, 1000, 1500, 50, 50, SWP_NOSIZE | SWP_NOZORDER);


	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	RECT desktop, r;
	const HWND hDesktop = GetDesktopWindow();
	const HWND console = GetConsoleWindow();

	GetWindowRect(console, &r); //stores the console's current dimensions
	GetWindowRect(hDesktop, &desktop);

	MoveWindow(console, desktop.right * 0.3, desktop.bottom - 200, 880, 150, TRUE); // 800 width, 100 height

	printf("Log Console.");
	std::cout.put('\n');

	std::thread t1(initQT, argc, argv);
	t1.detach();

	if (!init(argc, argv)) return 1;
	//if (!initKinect()) return 1;
	if (!kinectWrapper.initKinect()) return 1;

	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	// Set up array buffers
	const int dataSize = width * height * 3 * 4;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &cboId);
	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);

	float openAngle = 45;
	ratio = width / (GLdouble)height;

	// Camera setup
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(openAngle, ratio, 0.1, 1000);
	//glOrtho(-1.0, 1.0, -1.5f, 1.5f, -0.5f, 3.5f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*
	gluLookAt(
		cameraFocal.x, cameraFocal.y, cameraFocal.z,
		cameraPos.x, cameraPos.y, cameraPos.z,
		cameraUp.x, cameraUp.y, cameraUp.z
	);
	*/
	gluLookAt(
		0, 0, 0,
		0, 0, 1,
		0, 1, 0
	);
	

	// Init cam
	fovy = 0.785398163397 * 2; // in rad
	cam = new rt::PerspectiveCamera(cameraPos, cameraFocal, cameraUp,fovy , fovy  * ratio);

    // Main loop
    execute();
    return 0;
}
