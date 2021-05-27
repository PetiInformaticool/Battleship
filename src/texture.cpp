#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"
   
int windowHeight, windowWidth;

    GLuint tex;
    
void writeText(const char* string) {
	glColor3f(1.0f, 1.0f, 1.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	 //printf("display\n");
	 	 //glColor3f(1.0f, 1.f, 0.f);

	 glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(100, 100);
    glTexCoord2i(0, 1); glVertex2i(100, 500);
    glTexCoord2i(1, 1); glVertex2i(500, 500);
    glTexCoord2i(1, 0); glVertex2i(500, 100);
		 glEnd();
		glDisable(GL_TEXTURE_2D);
	 //drawRect(100.0, 100.0, 500.0, 500.0);
	 glColor3f(0.0f, 1.0f, 1.0f);
	  glRasterPos2f(200.0, 200.0);
	  writeText("mori");
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
    int width, height, nr;
    unsigned char* texDat = stbi_load("download.jpg", &width, &height, &nr, 0);
    assert(texDat != NULL);

    //upload to GPU texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
    glBindTexture(GL_TEXTURE_2D, 0);

    //match projection to window resolution (could be in reshape callback)
		//glutReshapeFunc(reshape);
    glutIdleFunc(nextmove);
		glutDisplayFunc(display);
		glMatrixMode(GL_PROJECTION);
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);
		
		glClear(GL_COLOR_BUFFER_BIT);
    
		 glFlush();
		 //getchar();
		//return 0;

    //clear and draw quad with texture (could be in display callback)
		glutMainLoop();

    return 0;
}
