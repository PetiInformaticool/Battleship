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
#include <thread>
#include <chrono>


using namespace std;

float coordx, coordy;

const float PI = 4 * atan(1);

int windowHeight, windowWidth;

struct Picture {
  int width, height, nr;
  unsigned char* texDat;
  GLuint tex;
};

Picture header;
Picture water;

Picture loadTexture(char* path) {
  Picture p;
  p.texDat = stbi_load(path, &p.width, &p.height, &p.nr, 0);
  glGenTextures(1, &p.tex);
  glBindTexture(GL_TEXTURE_2D, p.tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  int channels;
  if (p.nr == 4) {
    channels = GL_RGBA;
  } else if (p.nr == 3) {
    channels = GL_RGB;
  } else {
    assert(false);
  }
	glTexImage2D(GL_TEXTURE_2D, 0, channels, p.width, p.height, 0, channels, GL_UNSIGNED_BYTE, p.texDat);
  return p;
}

void writeText(const char* string) {
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void drawRegPoly(float x, float y, float r, int edges) {
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0f);
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
	glScalef(10.0/15, 10.0/15, 1.0);
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
  glLoadIdentity();
}
void drawBoat2() {
  glScalef(10.0/15, 10.0/15, 1.0);
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
  glLoadIdentity();
}


void drawBomb(float x, float y, float r, int edges) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glScalef((GLfloat)4.0/5.0, (GLfloat)4.0/5.0, 1.0);
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
	//glScalef(5.0/4.0, 5.0/4.0, 1.0);
}

void findBoat(board b, char l, int &x1, int &y1, int &x2, int &y2) {
  x1 = -1;
  for (int i = 0; i < 15; i++)
    for (int j = 0; j < 15; j++) {
			if (b.board[i][j]==l) {
				if (x1 == -1) {
					x1 = i, y1 = j;
				}
				else
				  x2 = i, y2 = j;
			}
		}
}

void drawBoard (board b[], int cnt[], Player2 Move, int turn, int type, int killed[][10], string winnerwinnerchickendinner) {
	glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, water.tex);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex2f(-2.50f,  0.75f);
    glTexCoord2f(0, 1); glVertex2f(-2.50f, -1.75f);
    glTexCoord2f(1, 1); glVertex2f( 2.50f, -1.75f);
    glTexCoord2f(1, 0); glVertex2f( 2.50f,  0.75f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindTexture(GL_TEXTURE_2D, header.tex);
	glLoadIdentity();
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.f, 1.f, 1.f);
  glBegin(GL_POLYGON);
		glTexCoord2f(0, 0); glVertex2f(-1.0f-1.75f+0.25f, 1.75+0.50f-1.5f);
    glTexCoord2f(0, 1); glVertex2f(-1.0f-1.75f+0.25f, 2.50+0.25-1.5f);
    glTexCoord2f(1, 1); glVertex2f(1.0f + 1.25f+0.25f, 2.50+0.25-1.5f);
    glTexCoord2f(1, 0); glVertex2f(1.0f + 1.25f+0.25f, 1.75f+0.50f-1.50f);
	glEnd();
	glBegin(GL_POLYGON);
	  glTexCoord2f(0, 0); glVertex2f(-1.5f, 1.25);
    glTexCoord2f(0, 1); glVertex2f(1.50f, 1.25);
    glTexCoord2f(1, 1); glVertex2f(1.25f, 1.50f);
    glTexCoord2f(1, 0); glVertex2f(-1.25, 1.50f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glRasterPos2f(-0.5, 1.35);
	writeText(winnerwinnerchickendinner.c_str());
	//drawRect(-2.50f, 0.75f, 2.50f, -1.75f);
  glColor3f(1.0f, 1.0f, 1.0f);
  string idk, lmao;
  lmao = "length 2  |  3 |  4  |  5";
             // 0/3 0/3 0/2 0/2;
  for (int i = 2; i <= 5; i++) {
	  idk += to_string(killed[0][i]);
	  idk += "/";
	  if (i == 2)
	    idk += "3 ";
	  if (i == 3)
	    idk += "3 ";
	  if (i == 4)
	    idk += "2 ";
	  if (i == 5)
	    idk += "2";
	}
  glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
  idk = idk;
  int n = idk.size(), m = lmao.size();
  char text[n + 1], text2[m + 1];
  strcpy(text, idk.c_str());
  strcpy(text2, lmao.c_str());
  glRasterPos2f(0.5, 2.35);
  writeText(text);
  glRasterPos2f(0.17, 2.5);
  writeText(text2);
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++) {
			glColor3f(0.0, 69.0/255.0, 94.0/255.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0f);
			glBegin(GL_POLYGON);
				glVertex2f(2.0f * i / 15, 2.0f * j / 15);
				glVertex2f(2.0f * (i + 1.0f) / 15, 2.0f * j / 15);
				glVertex2f(2.0f * (i + 1.0f) / 15, 2.0f * (j + 1.0f) / 15);
				glVertex2f(2.0f * i / 15, 2.0f * (j + 1.0f) / 15);
			glEnd();
		}
  cout << Move.x << " " << Move.y << "\n"; 
  if (turn == 1 && Move.x != -1 && type == 1) {
		for (int i = 0; i < 15; i++)
		  for (int j = 0; j < 15; j++)
		    if (15 - j - 1 == Move.x && i == Move.y) {
					drawRegPoly(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 1.0/15, 50);
					drawLine(2.0 * i / 15 + 1.0/15, 0, 2.0 * i / 15+1.0/15, 2.0);
					drawLine(0, 2.0 * j / 15 + 1.0/15, 2, 2.0 * j / 15  + 1.0/15);
				}
	  std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	for (int i = 0; i < 15; i++)
	  for (int j = 0; j < 15; j++) {
			int x = 15 - j - 1;
			int y = i;
			if (b[0].board[x][y] == FREE);
			if (turn == 1 && type && 15 - j - 1 == Move.x && i== Move.y)
			  continue;
			else if (b[0].board[x][y] == 'X') {
				glColor3f(0.8, 0.0, 0.0);
				drawBomb(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 0.5f/15, 400);
			}
			else if (b[0].board[x][y] == '*') {
				glColor3f(0.25, 0.60, 0.75);
				drawBomb(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 0.5f/15, 400);
			}
 		}
 	
 	for (char l = '0'; l <= '9'; l++) {
		int x1, y1, x2, y2;
		findBoat(b[0], l, x1, y1, x2, y2);
		if (x1 == x2 && x1 != -1) {
			int i = y1;
			int j = 15 - x1 - 1;
			glTranslatef(2.0f * i / 15, 2.0f * j / 15, 0.0f); 
			glScalef((y2 - y1 + 1) / 2.0f , 1.0f, 1.0f);
			drawBoat();
			glLoadIdentity();
			glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
		}
		if (y1 == y2 && x1 != -1) {
			int i = y2;
			int j = 15 - x2 - 1;
			glTranslatef(2.0f * i / 15, 2.0f * j / 15, 0.0f); 
			glScalef(1.0f, (x2 - x1 + 1) / 2.0f, 1.0f);
			drawBoat2();
			glLoadIdentity();
			glTranslatef(-2.5f+0.25f, -1.5f, 0.0);
		}
	}
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.5f-0.25f, -1.5f, 0.0);
	
	idk = "";
	for (int i = 2; i <= 5; i++) {
	  idk += to_string(killed[1][i]);
	  idk += "/";
	  if (i == 2)
	    idk += "3 ";
	  if (i == 3)
	    idk += "3 ";
	  if (i == 4)
	    idk += "2 ";
	  if (i == 5)
	    idk += "2";
	}
  idk = idk;
  strcpy(text, idk.c_str());
  idk = "\n" + idk;
  lmao = "length 2  |  3 |  4  |  5";
  glRasterPos2f(0.7, 2.3);
  writeText(text);
  glRasterPos2f(0.70-0.33, 2.5);
  writeText(lmao.c_str());
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++) {
			glColor3f(0.0, 69.0/255.0, 94.0/255.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0f);
			glBegin(GL_POLYGON);
				glVertex2f(2.0f * i / 15, 2.0f * j / 15);
				glVertex2f(2.0f * (i + 1.0f) / 15, 2.0f * j / 15);
				glVertex2f(2.0f * (i + 1.0f) / 15, 2.0f * (j + 1.0f) / 15);
				glVertex2f(2.0f * i / 15, 2.0f * (j + 1.0f) / 15);
			glEnd();
		}
	if (turn == 0 && Move.x != -1 && type == 1) {
		for (int i = 0; i < 15; i++)
		  for (int j = 0; j < 15; j++)
		    if (15 - j - 1 == Move.x && i == Move.y) {
					drawRegPoly(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 1.0/15, 50);
					drawLine(2.0 * i / 15 + 1.0/15, 0, 2.0 * i / 15+1.0/15, 2.0);
					drawLine(0, 2.0 * j / 15 + 1.0/15, 2, 2.0 * j / 15  + 1.0/15);
				}
	}
	for (int i = 0; i < 15; i++)
	  for (int j = 0; j < 15; j++) {
			int x = 15 - j - 1;
			int y = i;
		  if (turn == 0 && type && 15 - j - 1 == Move.x && i== Move.y)
			  continue;
			if (b[1].board[x][y] == FREE);
			else if (b[1].board[x][y] == 'X') {
				glColor3f(0.8, 0.0, 0.0);
				drawBomb(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 0.5f/15, 400);
			}
			else if (b[1].board[x][y] == '*') {
				glColor3f(0.25, 0.60, 0.75);
				drawBomb(2.0 * i / 15 + 1.0/15, 2.0 * j / 15 + 1.0/15, 0.5f/15, 400);
			}
 		}
 	for (char l = '0'; l <= '9'; l++) {
		int x1, y1, x2, y2;
		findBoat(b[1], l, x1, y1, x2, y2);
		if (x1 == x2 && x1 != -1) {
			int i = y1;
			int j = 15 - x1 - 1;
			glTranslatef(2.0f * i / 15, 2.0f * j / 15, 0.0f); 
						glScalef((y2 - y1 + 1) / 2.0f , 1.0f, 1.0f);

			//printf("%c &d\n", l, y2 - y1  + 1);
			drawBoat();
			glLoadIdentity();
			glTranslatef(0.5f-0.25f, -1.5f, 0.0);
			//glScalef(1.0f, 1.0f, 1.0f);
		}
		if (y1 == y2 && x1 != -1) {
			int i = y2;
			int j = 15 - x2 - 1;
			glTranslatef(2.0f * i / 15, 2.0f * j / 15, 0.0f); 
			glScalef(1.0f, (x2 - x1 + 1) / 2.0f, 1.0f);
			drawBoat2();
			glLoadIdentity();
			glTranslatef(0.5f-0.25f, -1.5f, 0.0);
		}
	}
	
	//drawCoordinatesSystem();
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2f(-0.25f+0.05f, 1.25f+0.10f);
  writeText("\n\nSCORE"); 
  glRasterPos2f(-0.25f+0.12f, 1.25f-0.10f);
	char scor[12];
	scor[0] = scor[1] = '\n';
	int lel;
	if (cnt[0] == 10) {
		scor[2] = '1'; scor[3] = '0';
		lel = 4;
	}
	else
	  scor[2] = cnt[0] + '0', lel = 3;
	scor[lel] = ' ';
	scor[lel + 1] = '-';
	scor[lel + 2] = ' ';
	lel += 3;
	if (cnt[1] == 10) {
		scor[lel] = '1'; scor[lel + 1] = '0';
		lel += 2;
	}
	else
	  scor[lel] = cnt[1] + '0', lel++;
	scor[lel] = '\0';
	 
  glLoadIdentity();
  writeText(scor);
  //glRasterPos2f(-1.40f, 1.f);
	//writeText("Player 1");
	//glRasterPos2f(1.15f, 1.f);
	//writeText("Player 2");
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
  //glOrtho(-2.7, 2.7, -2.0, 2.0, -10.0, 10.0);
	if (w <= h)
    glOrtho(-2.7, 2.7, -2.0 * (GLfloat) h / (GLfloat) w,
      2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
  else {
		//cout << "salllllll\n\n\n";
		float x = 1.0 * w / h, y;
		if (x >= 1.6)
		  y = 1.5;
		else
		  y = 1;
		glOrtho(-2.7 * y,
      2.7 * y, -2.0, 2.0, -10.0, 10.0);
	}
  glMatrixMode(GL_MODELVIEW);
}


int mainLoop (int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(1000, 750);
  glutCreateWindow("Battleship la cel mai inalt nivel");

  header = loadTexture("poza.bmp");
  water = loadTexture("water-texture-breeze (1).jpg");
  
  glutReshapeFunc(reshape);
  glutIdleFunc(nextmove);
  glutDisplayFunc(display);
  glutMouseFunc(onMouseClick);
  glutMainLoop();
  return 0;	
}
