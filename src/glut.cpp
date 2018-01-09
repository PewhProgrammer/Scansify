#include "glut.h"
#include "main.h"

#include "gl\glew.h"
#include <iostream>

void draw() {
   drawData();
   glutSwapBuffers();
}

void execute() {
    glutMainLoop();
}

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//store the x,y value where the click happened
		std::cout << "mouse click on: (" << x << ", " << y << ") " << std::endl;
	}
}

bool init(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width,height);
    glutCreateWindow("Scansify");
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
	glutMouseFunc(OnMouseClick);
	glewInit();
    return true;
}
