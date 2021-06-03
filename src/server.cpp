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
Player2 KinderBueno = {-1, -1, -1};
int turn;

int len[2][15], cnt[2], len2[2][15], killed[2][10], bomb[2][2];
board b[2];

FILE* pipe[4];

bool lmao;

int type = 1;

int ended;


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
	string state;
	if (ended == 0) state = "";
	if (ended == 1) state = "Winner is Player 1";
	if (ended == 2) state = "Winner is Player 2";
	if (ended == 3) state = "Draw"; 
	drawBoard(b, cnt, KinderBueno, turn, type, killed, state);
	return;
}

void schema (int turn, Player2 KinderBueno) {
	if (b[turn].board[KinderBueno.x][KinderBueno.y] != '.')
	  return;
	if (Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] != FREE) {
			int id = Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] - '0';
			len[1 - turn][id]--;
			if (len[1 - turn][id] == 0) {
				killed[turn][len2[1 - turn][id]]++;
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

bool drawCheck;

bool sendSmecherie(int turn) {
  bool ok = sendBoard(pipe[2*turn], b[turn])
			| sendShips(pipe[2 * turn], killed[turn])
			| sendBomb(pipe[2 * turn], bomb[turn]);
  fflush(pipe[turn]);
  return ok;
}
int given[2][2];

int cnttt = 1;

void nextmove () {
  cnttt++;
  if (cnt[0] < 10 && cnt[1] < 10) {
		//cout << "actually does something";
		bool lol = sendSmecherie(turn);
		if (lol == 0) {
		  b[0].state = 1;
			b[1].state = 1;
			sendSmecherie(0);
			sendSmecherie(1);
			exit(0);
		}
		fread(&KinderBueno, sizeof(KinderBueno), 1, pipe[1+2*turn]);
		if (KinderBueno.type == 1) {
			schema (turn, KinderBueno);
		}
		if (KinderBueno.type == 2) {
			if (bomb[turn][0] == 0) {
				cout << "Winner is player " << 2 - turn << " ";
				b[0].state = 1;
				b[1].state = 1;
				sendSmecherie(0);
				sendSmecherie(1);
			}
			bomb[turn][0]--;
		  for (int i = 0; i < 15; i++)
		    schema(turn, {KinderBueno.type, KinderBueno.x, i});
		   KinderBueno.x = -1;
		}
		if (KinderBueno.type == 3) {
			if (bomb[turn][0] == 0) {
				cout << "Winner is player " << 2 - turn << " ";
				b[0].state = 1;
				b[1].state = 1;
				sendSmecherie(0);
				sendSmecherie(1);
			}
			bomb[turn][0]--;
			for (int i = 0; i < 15; i++)
			  schema(turn, {KinderBueno.type, i,KinderBueno.x});
			KinderBueno.x = -1;
		}
		if (KinderBueno.type == 4) {
			if (bomb[turn][1] == 0) {
				cout << "Winner is player " << 2 - turn << " ";
				b[0].state = 1;
				b[1].state = 1;
				sendSmecherie(0);
				sendSmecherie(1);
			}
			bomb[turn][1]--;
		  for (int i = 0; i < 2; i++)
		    for (int j = 0; j < 2; j++)
		      if (KinderBueno.x + i < 15 && KinderBueno.y + j < 15) {
						schema(turn, {1, KinderBueno.x + i, KinderBueno.y + j});
					}
			KinderBueno.x = -1;
		}
		if (killed[turn][5] == 1 && !given[turn][0]) {
			bomb[turn][1]++;
			given[turn][0] = 1;
		}
		if (killed[turn][5] == 2  && !given[turn][1]) {
			cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
		  bomb[turn][0]++;
			given[turn][1] = 1;
		}
		turn = 1 - turn;
		type = 1;
		display();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		type = 0;
		display();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  else if (!drawCheck) {
		//cout << "drawwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n";
		drawCheck = 1;
		if (turn == 1) {
		  sendSmecherie(turn);
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
		if (cnt[0] == 10 && cnt[1] == 10)
		  ended = 3;
		else if (cnt[0] == 10)
		  ended = 1;
		else if (cnt[1] == 10)
		  ended = 2;
		display();
		b[0].state = 1;
		b[1].state = 1;
		sendSmecherie(0);
		sendSmecherie(1);
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
	short ships[2][6] = {{0, 0, 3, 3, 2, 2}, {0, 0, 3, 3, 2, 2}};
	setFree(b[0]);
	setFree(b[1]);
	bomb[0][1] = bomb[1][1] = 3;
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
			  len2[turn][nr[turn]] = x.len; 
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
