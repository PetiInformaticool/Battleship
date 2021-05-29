#include "viewer.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"


using namespace std;

const float PI = 4 * atan(1);

int windowHeight, windowWidth;
  int width, height, nr;
  unsigned char* texDat;
 GLuint tex;


void writeText(const char* string) {
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void drawRegPoly(float x, float y, float r, int edges) {
	glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < edges; i++) {
	  float phi = i * 2.0f * PI / edges;
	  glVertex2f(x + r* cos(phi), y + r * sin(phi));
	}
	glEnd();
}
void drawLine (float x1, float y1, float x2, float y2) {
  glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
  glEnd();
}

void drawRect (float x1, float y1, float x2, float y2) {
  glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
  glEnd();
}

void drawCoordinatesSystem() {
  glLoadIdentity();
  glPointSize(8.0f);
  glEnable(GL_POINT_SMOOTH);
  glBegin(GL_POINTS);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = -10; i <= +10; i++) {
    for (int j = -10; j <= +10; j++) {
      glVertex2i(i, j);
    }
  }
  glEnd();
  glDisable(GL_POINT_SMOOTH);
  glLineWidth(1.0f);
  //drawLine(-10, 0, 10, 0);
  //drawLine(0, -10, 0, 10);
}

void drawBoat() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(0.2f, 0.2f, 0.2f);
  glBegin(GL_POLYGON); 
    glVertex2f(0.f, 0.10);
    glVertex2f(0.10f, 0.0f);
    glVertex2f(0.30, 0.0f);
    glVertex2f(0.40f, 0.10f);
  glEnd();
  glColor3f(0.6f, 0.6f, 0.6f);
  //drawLine(0.07f, 0.10f, 0.09f, 0.13);
  glBegin(GL_POLYGON);
    glVertex2f(0.07f, 0.10);
		glVertex2f(0.09f, 0.13);
		glVertex2f(0.12f, 0.13);
		glVertex2f(0.14f, 0.10);
  glEnd();
  glLineWidth(1.f); 
  drawLine(0.09f, 0.13f, 0.06f, 0.15f);
  drawRect(0.25, 0.10, 0.32, 0.17);
  glBegin(GL_TRIANGLES);
    glVertex2f(0.32f, 0.10f);
    glVertex2f(0.32, 0.145);
    glVertex2f(0.34, 0.10);
  glEnd();
}
void drawBoat2() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(0.2f, 0.2f, 0.2f);
  glBegin(GL_POLYGON); 
    glVertex2f(0.10f, 0.0);
    glVertex2f(0.0f, 0.10f);
    glVertex2f(0.0, 0.30f);
    glVertex2f(0.10f, 0.40f);
  glEnd();
  glColor3f(0.6f, 0.6f, 0.6f);
  glBegin(GL_POLYGON);
    glVertex2f(0.10f, 0.07);
		glVertex2f(0.13f, 0.09);
		glVertex2f(0.13f, 0.12);
		glVertex2f(0.10f, 0.14);
  glEnd();
  glLineWidth(1.f); 
  drawLine(0.13f, 0.09f, 0.15f, 0.07f);
  drawRect(0.10, 0.25, 0.17, 0.32);
  glBegin(GL_TRIANGLES);
    glVertex2f(0.10f, 0.32f);
    glVertex2f(0.145, 0.32);
    glVertex2f(0.10, 0.34);
  glEnd();
}

void findBoat(board b, char l, int &x1, int &y1, int &x2, int &y2) {
  x1 = -1;
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++) {
			if (b.board[i][j]==l) {
				if (x1 == -1) {
					x1 = i, y1 = j;
				}
				else
				  x2 = i, y2 = j;
			}
		}
}

void drawBoard (board b[], int cnt[]) {
	glClear(GL_COLOR_BUFFER_BIT);
	
  glLoadIdentity();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
  texDat = stbi_load("water-texture-breeze\ (1).jpg", &width, &height, &nr, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
  glEnable(GL_TEXTURE_2D);
 glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-2.50f, 0.75f);
    glTexCoord2f(0, 1); glVertex2f(-2.50f, -1.75f);
    glTexCoord2f(1, 1); glVertex2f(2.50f, -1.75f);
    glTexCoord2f(1, 0); glVertex2f(2.50f, 0.75f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//drawRect(-2.50f, 0.75f, 2.50f, -1.75f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			glColor3f(0.0, 69.0/255.0, 94.0/255.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0f);
			glBegin(GL_POLYGON);
				glVertex2f(2.0f * i / 10, 2.0f * j / 10);
				glVertex2f(2.0f * (i + 1.0f) / 10, 2.0f * j / 10);
				glVertex2f(2.0f * (i + 1.0f) / 10, 2.0f * (j + 1.0f) / 10);
				glVertex2f(2.0f * i / 10, 2.0f * (j + 1.0f) / 10);
			glEnd();
		}
	for (int i = 0; i < 10; i++)
	  for (int j = 0; j < 10; j++) {
			int x = 10 - j - 1;
			int y = i;
			if (b[0].board[x][y] == FREE);
			else if (b[0].board[x][y] == 'X') {
				glColor3f(0.7, 0.0, 0.0);
				glLineWidth(4.0f);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * (j + 1.0f) / 10-0.02, 2.f * i / 10+0.02, 2.f * j / 10+0.02);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * j / 10+0.02, 2.f * i / 10+0.02, 2.f * (j + 1.0f) / 10-0.02);
			}
			else if (b[0].board[x][y] == '*') {
				glColor3f(0.2f, 0.2f, 0.2f);
				glLineWidth(4.0f);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * (j + 1.0f) / 10-0.02, 2.f * i / 10+0.02, 2.f * j / 10+0.02);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * j / 10+0.02, 2.f * i / 10+0.02, 2.f * (j + 1.0f) / 10-0.02);
			}
 		}
 	for (char l = '0'; l < '7'; l++) {
		int x1, y1, x2, y2;
		findBoat(b[0], l, x1, y1, x2, y2);
		if (x1 == x2 && x1 != -1) {
			int i = y1;
			int j = 10 - x1 - 1;
			glTranslatef(2.0f * i / 10, 2.0f * j / 10, 0.0f); 
			glScalef((y2 - y1 + 1) / 2.0f , 1.0f, 1.0f);
			drawBoat();
			glLoadIdentity();
			glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
			//glScalef(1.0f, 1.0f, 1.0f);
		}
		if (y1 == y2 && x1 != -1) {
			int i = y2;
			int j = 10 - x2 - 1;
			glTranslatef(2.0f * i / 10, 2.0f * j / 10, 0.0f); 
			glScalef(1.0f, (x2 - x1 + 1) / 2.0f, 1.0f);
			drawBoat2();
			glLoadIdentity();
			glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
		}
	}
	glLoadIdentity();
	glTranslatef(0.5f-0.25f, -1.5f, 0.0);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			glColor3f(0.0, 69.0/255.0, 94.0/255.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0f);
			glBegin(GL_POLYGON);
				glVertex2f(2.0f * i / 10, 2.0f * j / 10);
				glVertex2f(2.0f * (i + 1.0f) / 10, 2.0f * j / 10);
				glVertex2f(2.0f * (i + 1.0f) / 10, 2.0f * (j + 1.0f) / 10);
				glVertex2f(2.0f * i / 10, 2.0f * (j + 1.0f) / 10);
			glEnd();
		}
	for (int i = 0; i < 10; i++)
	  for (int j = 0; j < 10; j++) {
			int x = 10 - j - 1;
			int y = i;
			if (b[1].board[x][y] == FREE);
			else if (b[1].board[x][y] == 'X') {
				glColor3f(0.7, 0.0, 0.0);
				glLineWidth(4.0f);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * (j + 1.0f) / 10-0.02, 2.f * i / 10+0.02, 2.f * j / 10+0.02);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * j / 10+0.02, 2.f * i / 10+0.02, 2.f * (j + 1.0f) / 10-0.02);
			}
			else if (b[1].board[x][y] == '*') {
				glColor3f(0.2f, 0.2f, 0.2f);
				glLineWidth(4.0f);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * (j + 1.0f) / 10-0.02, 2.f * i / 10+0.02, 2.f * j / 10+0.02);
				drawLine(2.f * (i + 1.0f) / 10-0.02, 2.f * j / 10+0.02, 2.f * i / 10+0.02, 2.f * (j + 1.0f) / 10-0.02);
			}
 		}
 	for (char l = '0'; l < '7'; l++) {
		int x1, y1, x2, y2;
		findBoat(b[1], l, x1, y1, x2, y2);
		if (x1 == x2 && x1 != -1) {
			int i = y1;
			int j = 10 - x1 - 1;
			glTranslatef(2.0f * i / 10, 2.0f * j / 10, 0.0f); 
						glScalef((y2 - y1 + 1) / 2.0f , 1.0f, 1.0f);

			//printf("%c &d\n", l, y2 - y1  + 1);
			drawBoat();
			glLoadIdentity();
			glTranslatef(0.5f-0.25f, -1.5f, 0.0);
			//glScalef(1.0f, 1.0f, 1.0f);
		}
		if (y1 == y2 && x1 != -1) {
			int i = y2;
			int j = 10 - x2 - 1;
			glTranslatef(2.0f * i / 10, 2.0f * j / 10, 0.0f); 
			glScalef(1.0f, (x2 - x1 + 1) / 2.0f, 1.0f);
			drawBoat2();
			glLoadIdentity();
			glTranslatef(0.5f-0.25f, -1.5f, 0.0);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	texDat = stbi_load("poza.bmp", &width, &height, &nr, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDat);
	glLoadIdentity();
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.f, 1.f, 1.f);
  glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-1.0f-1.75f+0.25f, 1.75+0.50f-1.5f);
    glTexCoord2f(0, 1); glVertex2f(-1.0f-1.75f+0.25f, 2.50+0.25-1.5f);
    glTexCoord2f(1, 1); glVertex2f(1.0f + 1.25f+0.25f, 2.50+0.25-1.5f);
    glTexCoord2f(1, 0); glVertex2f(1.0f + 1.25f+0.25f, 1.75f+0.50f-1.5f);
	  //drawRect(-1.0f-1.75f, 1.75f+0.50f, 1.0f+1.25f, 2.50f+0.25f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2f(-0.25f+0.05f, 1.25f+0.10f);
  writeText("\n\nSCORE");
  glRasterPos2f(-0.25f+0.12f, 1.25f-0.10f);
	char scor[8];
	scor[0] = '\n';
	scor[1] = '\n';
	scor[2] = cnt[0] + '0';
	scor[3] =' ';
	scor[4] = '-';
	scor[5] = ' ';
	scor[6] = cnt[1] + '0';
  glLoadIdentity();
  writeText(scor);
  glRasterPos2f(-1.40f, 1.f);
	writeText("Player 1");
	glRasterPos2f(1.15f, 1.f);
	writeText("Player 2");
  glFlush();
  glutSwapBuffers();
}


void onMouseClick (int btn, int state, int x, int y) {
}

void reshape(int w, int h) {
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


int mainLoop (int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(1000, 750);
  glutCreateWindow("Battleship la cel mai inalt nivel");
  
  

  
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
  
  
  glutReshapeFunc(reshape);
  glutIdleFunc(nextmove);
  glutDisplayFunc(display);
  glutMouseFunc(onMouseClick);
  glutMainLoop();
  return 0;	
}
