#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"
   
int windowHeight, windowWidth;

    GLuint tex;
    
 const float PI = 4 * atan(1);
    
void writeText(const char* string) {
	glColor3f(1.0f, 1.0f, 1.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void drawRegPoly(float x, float y, float r, int edges) {
	glScalef((GLfloat)1.0/9.0, (GLfloat)1.0/9.0, 1.0);
	glColor4f(0.0, 0.5, 0.0, 0.0);
	glLineWidth(1.0f);
  glBegin(GL_POLYGON);
  for (int i = 0; i < edges; i++) {
	  float phi = i * 2.0f * PI / edges;
	  glVertex2f(x + r* cos(phi), y + r * sin(phi));
	}
	glEnd();
  for (int i = 0; i < edges; i += 50) {
		glBegin(GL_POLYGON);
			float phi;
			phi = (i + 16) * 2.0f * PI / edges;
			glVertex2f(x + r * cos(phi), y + r * sin(phi));
			phi = (i + 34) * 2.0f * PI / edges;
			glVertex2f(x + r * cos(phi), y + r * sin(phi));
			phi = (i + 25) * 2.0f * PI / edges;
			glVertex2f(x + (r + 2  * r / 3) * cos(phi), y + (r + 2 * r / 3) * sin(phi));
		glEnd();
		
	}
}

void drawRect (float x1, float y1, float x2, float y2) {
  glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
  glEnd();
}
   
void reshape(int w, int h) {
  printf("onWindowResize %d %d\n\n\n\n\n", w, h);
  windowWidth = w;
  windowHeight = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
      2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
  else
    glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
      2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	drawRegPoly(0.0, 0.0, 1.0, 400);
	glFlush();
  glutSwapBuffers();
}


void nextmove() {display();}



int main(int argc, char** argv)
{
    //create GL context
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("windowname");

    //create test checker image
    /*int width, height, nr;
    unsigned char* texDat = stbi_load("lmao.jpg", &width, &height, &nr, 0);
    assert(texDat != NULL);

    //upload to GPU texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
    glBindTexture(GL_TEXTURE_2D, 0);*/

    //match projection to window resolution (could be in reshape callback)
		//glutReshapeFunc(reshape);
    glutReshapeFunc(reshape);
		glutIdleFunc(nextmove);
		glutDisplayFunc(display);
    		 //getchar();
		//return 0;

    //clear and draw quad with texture (could be in display callback)
		glutMainLoop();

    return 0;
}
