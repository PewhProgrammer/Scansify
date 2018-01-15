#include "ChangeCameraCommand.h"
#include "glut.h"


ChangeCameraCommand::ChangeCameraCommand(float eyex, float eyey, float eyez, float posx, float posy, float posz, float upx, float upy, float upz)
{
	eye[0] = eyex;
	eye[1] = eyey;
	eye[2] = eyez;

	pos[0] = posx;
	pos[1] = posy;
	pos[2] = posz;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;
}


ChangeCameraCommand::~ChangeCameraCommand()
{
}

void ChangeCameraCommand::execute() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], pos[0], pos[1], pos[2], up[0], up[1], up[2]);
}

bool ChangeCameraCommand::verify() {
	return true;
}