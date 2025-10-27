#define FREEGLUT_STATIC // Define a static library for calling functions
#include <GL/freeglut.h> // Include the header file

void define_to_OpenGL();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutCreateWindow("GP");

	glutDisplayFunc(define_to_OpenGL);
	glutMainLoop();

}

void define_to_OpenGL()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// set window size and dimension
	glutInitWindowSize(600, 400);
	glutInitWindowPosition(50, 50);

	int L = -100;
	int R = 500;
	int B = -200;
	int T = 200;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(L, R, B, T);

	// draw the axes
	glLineWidth(10.0);
	// RGB
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2f(0.0f, -150.0f);
		glVertex2f(0.0f, 150.0f);
	glEnd();

	// draw a dot
	glPointSize(10.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(200.0, 0.0);
	glEnd();



	glFlush();
}