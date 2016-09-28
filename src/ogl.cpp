#include "ogl.hpp"

void display() {
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS);
  glColor3f(1.0f,1.0f,1.0f);
  glVertex2f(-0.5f,-0.5f);
  glVertex2f(0.5f,-0.5f);
  glVertex2f(0.5f,0.5f);
  glVertex2f(-0.5f,0.5f);
  glEnd();

  glFlush();
}

int openGLMode(int c, char **v) {

  glutInit(&c,v);                 // Initialize GLUT
  glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
  glutInitWindowSize(320, 320);   // Set the window's initial width & height
  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
  glutDisplayFunc(display); // Register display callback handler for window re-paint
  glutMainLoop();           // Enter the infinitely event-processing loop

  return EXIT_SUCCESS;

}
