#include <GL/freeglut.h> // Include the header file

void renderScene(void) // Function for geometric creation
{
 // Clear the buffer to the predefined color and depth value
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //Move origin of current user coordinate system to the screen center: similar to a reset operation
 glLoadIdentity();
 glBegin(GL_TRIANGLES); // Start draw TRIANGLE function
 glVertex3f(-0.5, -0.5, 0.0); // Set coordinates of first vertex
 glVertex3f(0.5, 0.0, 0.0); // Set coordinates of second vertex
 glVertex3f(0.0, 0.5, 0.0); // Set coordinates of last vertex
 glEnd(); // End draw TRIANGLE function
 glutSwapBuffers(); // Refresh the screen to display the graphics
}

int main(int argc, char* argv[]) // Standard main function
{
 glutInit(&argc, (char**)argv); // Initialization
 // Define display mode: depth buffer, double buffer and RGBA color
 glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowPosition(100, 100); // Define the location of the window
 glutInitWindowSize(320, 320); // Define the size of the window
 glutCreateWindow("Hello OpenGL"); // Create a window called “Hello OpenGL”
 glutDisplayFunc(renderScene); // Set the first function
 glutMainLoop(); // Enter the GLUT event processing loop
 return 0; // Return an integer value
} 
