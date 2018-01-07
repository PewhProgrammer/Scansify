#include "glut.h"
#include "main.h"

#include "gl\glew.h"

void draw() {
   drawData();
   glutSwapBuffers();
}

void execute() {
    glutMainLoop();
}

bool init(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width,height);
    glutCreateWindow("Scansify");
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
	glewInit();
    return true;
}
