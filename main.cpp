#include <GL/freeglut.h>

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f); glVertex2f(-0.6f, -0.4f);
		glColor3f(0.f, 1.f, 0.f); glVertex2f( 0.6f, -0.4f);
		glColor3f(0.f, 0.f, 1.f); glVertex2f( 0.0f,  0.6f);
	glEnd();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int, int) {
	if (key == 27) { // ESC
		glutLeaveMainLoop();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("freeglut minimal");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glutMainLoop();
	return 0;
}


