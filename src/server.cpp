#include "protocol.h"
#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <cassert>


using namespace std;

int len[2][10];

int sum (short ship[]) {
  return ship[1] + ship[2] + ship[3] + ship[4];
}

void print(board b[2]) {
  for (int i= 0; i < LENGTH; i++, printf("\n"))
		for (int j = 0; j < 2 * WIDTH + 5; j++)
		  if (j < WIDTH)
		    cout << b[0].board[i][j];
		  else if (j < WIDTH + 5)
	      cout << " ";
	    else 
	    	cout << b[1].board[i][j - WIDTH - 5];
	return;
}


int main (int argc, char** argv) {
	FILE* pipe[4];
	pipe[0] = fopen("/home/alex/Desktop/battleship/pipes/serverX", "wb");
	pipe[1] = fopen("/home/alex/Desktop/battleship/pipes/playerX", "rb");
	pipe[2] = fopen("/home/alex/Desktop/battleship/pipes/server0", "wb");
	pipe[3] = fopen("/home/alex/Desktop/battleship/pipes/player0", "rb");
	short ships[2][5] = {{0, 3, 3, 2, 1}, {0, 3, 3, 2, 1}};
	board b[2];
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
		printf("Player %d turn:\n", turn);
		for  (int i = 0; i < LENGTH; i++, printf("\n"))
			for (int j = 0; j < WIDTH; j++)
			  printf("%c", b[turn].board[i][j]);
		printf("\n");
		nr[turn]++;
		turn = 1 - turn;
	}
	system("clear");
	b[0].state = 1;
	b[1].state = 1;
	sendBoard(pipe[0], b[0]);
	sendBoard(pipe[2], b[1]);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	board Fleet[2] = {b[0], b[1]}; 
	setFree(b[0]);
	setFree(b[1]);
	int count[2] = {0, 0};
	Player2 KinderBueno;
	turn = 0;
	while (count[0] < 9 && count[1] < 9) {
		sendBoard(pipe[2 * turn], b[turn]);
		fread(&KinderBueno, sizeof(KinderBueno), 1, pipe[1+2*turn]);
		if (Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] != FREE) {
		  int id = Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] - '0';
		  len[1 - turn][id]--;
		  if (len[1 - turn][id] == 0) {
				discoverShip(b[turn], Fleet[1 - turn], id);
			  count[turn]++;
			}
			else
			  b[turn].board[KinderBueno.x][KinderBueno.y] = HIT;
		}
		else  {
			b[turn].board[KinderBueno.x][KinderBueno.y] = MISS;
		}
	  printf ("Player %d turn:\n", turn);
	  print(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		system("clear");
		printf("\n\n");
		turn = 1 - turn;
 	}
 	if (turn == 1) {
		///caz de egalitate
		fwrite(&b[turn], sizeof(b[turn]), 1, pipe[2 * turn]);
		fflush(pipe[2 * turn]);
		fread(&KinderBueno, sizeof(KinderBueno), 1, pipe[1+2*turn]);
		if (Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] != FREE) {
		  int id = Fleet[1 - turn].board[KinderBueno.x][KinderBueno.y] - '0';
		  len[1 - turn][id]--;
		  if (len[1 - turn][id] == 0) {
				discoverShip(b[turn], Fleet[1 - turn], id);
			  count[turn]++;
			}
			else
			  b[turn].board[KinderBueno.x][KinderBueno.y] = HIT;
		}
		else  {
			b[turn].board[KinderBueno.x][KinderBueno.y] = MISS;
		}
		printf ("Player %d turn:\n", turn);
		print(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		system("clear");
		printf("\n\n");
	}
	if (count[0] == 9 && count[1] == 9) {
		cout << "DRAW!!\n\n\n\n";
		print(b);
	}
	else {
		printf("WINNER IS PLAYER %d !!!\n\n", 1 - turn);
		print(b);
	}
	b[0].state = 1;
	b[1].state = 1;
	sendBoard(pipe[0], b[0]);
	sendBoard(pipe[2], b[1]);
	//cout << "server ends!";
  return 0;
}
