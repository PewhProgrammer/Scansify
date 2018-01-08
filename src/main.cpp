#include "main.h"
#include "glut.h"

#include <cmath>
#include <cstdio>

#include <Windows.h>
#include <Ole2.h>

#include "MeshTriangulation.h"
#include "PCLRenderer.h"
#include "KinectWrapper.h"
#include <Kinect.h>

#include "iostream"

// We'll be using buffer objects to store the kinect point cloud
GLuint vboId;
GLuint cboId;

PCLRenderer rend;
KinectWrapper kinectWrapper;


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
	*fdest++ = 0;
	*fdest++ = 0;
	*fdest++ = 1.5f;
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	dest = (GLubyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	fdest = (float*)dest;
	*fdest++ = 140;
	*fdest++ = 0;
	*fdest++ = 255;
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void drawPCLData() {
	PCLRenderer::PCLOutput* out = new PCLRenderer::PCLOutput ;
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
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void drawData() {
	getKinectData();
	//bufferAxis();
	//rotateCamera();
	//drawPCLData();

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


int main(int argc, char* argv[]) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	

	printf("Log Console.");
	std::cout.put('\n');

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
	gluLookAt(0,0,0,0,0,1,0,1,0);

    // Main loop
    execute();
    return 0;
}
