#include "protocol.h"
#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <cassert>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "viewer.h"

using namespace std;


board Fleet[2];
Player2 KinderBueno;
int turn;

int len[2][10], cnt[2];
board b[2];

FILE* pipe[4];

bool lmao;


void display() {
	printf ("Player %d turn:\n", turn);
	//printf("display\n");
	for (int i= 0; i < LENGTH; i++, printf("\n"))
		for (int j = 0; j < 2 * WIDTH + 5; j++)
		  if (j < WIDTH)
		    cout << b[0].board[i][j];
		  else if (j < WIDTH + 5)
	      cout << " ";
	    else 
	    	cout << b[1].board[i][j - WIDTH - 5];
	drawBoard(b, cnt);
	return;
}
bool drawCheck;

int cnttt = 1;

void nextmove () {
  cnttt++;
  if (cnt[0] < 7 && cnt[1] < 7) {
		//cout << "actually does something";
		sendBoard(pipe[2 * turn], b[turn]);
		fread(&KinderBueno, sizeof(KinderBueno), 1, pipe[1+2*turn]);
		if (Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] != FREE) {
			int id = Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] - '0';
			len[1 - turn][id]--;
			if (len[1 - turn][id] == 0) {
				discoverShip(b[turn], Fleet[1 - turn], id);
				cnt[turn]++;
			}
			else
				b[turn].board[KinderBueno.x][KinderBueno.y] = HIT;
		}
		else  {
			b[turn].board[KinderBueno.x][KinderBueno.y] = MISS;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		turn = 1 - turn;
	  display();
  }
  else if (!drawCheck) {
		//cout << "drawwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n";
		drawCheck = 1;
		if (turn == 1) {
		  fwrite(&b[turn], sizeof(b[turn]), 1, pipe[2 * turn]);
			fflush(pipe[2 * turn]);
			fread(&KinderBueno, sizeof(KinderBueno), 1, pipe[1+2*turn]);
			if (Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] != FREE) {
				int id = Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] - '0';
				len[1 - turn][id]--;
				if (len[1 - turn][id] == 0) {
					discoverShip(b[turn], Fleet[1 - turn], id);
					cnt[turn]++;
				}
				else
					b[turn].board[KinderBueno.x][KinderBueno.y] = HIT;
			}
			else  {
				b[turn].board[KinderBueno.x][KinderBueno.y] = MISS;
			}
		}
		display();
	}
	else if (!lmao){
		b[0].state = 1;
		b[1].state = 1;
		sendBoard(pipe[0], b[0]);
		sendBoard(pipe[2], b[1]);
		lmao = 1;
	}
	else
	  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}


int sum (short ship[]) {
  return ship[2] + ship[3] + ship[4] + ship[5];
}


int main (int argc, char** argv) {
	pipe[0] = fopen("pipes/serverX", "wb");
	pipe[1] = fopen("pipes/playerX", "rb");
	pipe[2] = fopen("pipes/server0", "wb");
	pipe[3] = fopen("pipes/player0", "rb");
	short ships[2][6] = {{0, 0, 3, 2, 1, 1}, {0, 0, 3, 2, 1, 1}};
	setFree(b[0]);
	setFree(b[1]);
	char* ch = "0123456789";
  int nr[2] = {0, 0};
  int turn = 0;
	while (sum(ships[0]) + sum(ships[1])) {
		sendBoard(pipe[2 * turn], b[turn]);
		while (1){
			Player1 x = {0, 0, 0, 1};
			fread(&x, sizeof(x), 1, pipe[2 * turn + 1]);
			int moveState = placeShip(b[turn], x, ships[turn], ch[nr[turn]]);
			fwrite(&moveState, sizeof(moveState), 1, pipe[2 * turn + 0]);
			fflush(pipe[2 * turn + 0]);
			if (moveState == 1) {
			  ships[turn][x.len]--;	
			  len[turn][nr[turn]] = x.len;		 
			  break;
			}
		}
		nr[turn]++;
		turn = 1 - turn;
	}
	b[0].state = 1;
	b[1].state = 1;
	sendBoard(pipe[0], b[0]);
	sendBoard(pipe[2], b[1]);
	Fleet[0] = b[0];
	Fleet[1] = b[1];
	setFree(b[0]);
	setFree(b[1]);
	mainLoop(argc, argv);
	//cout << "server ends!";
  return 0;
}
