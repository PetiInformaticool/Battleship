#include <stdio.h>
#include "game.h"
#include "protocol.h"
#include <iostream>
#include <cassert>

using namespace std;



int main (int argc, char** argv) {
	FILE* pipe[4];
	srand(time(NULL));
	pipe[0] = fopen("/home/alex/Desktop/battleship/pipes/serverX", "rb");
	pipe[1] = fopen("/home/alex/Desktop/battleship/pipes/playerX", "wb");
	board b;
	int ships[]= {0, 3, 3, 2, 1};
	while (1) {
		b = getBoard(pipe[0]);
		if (b.state == 1) {
			break;
		}
		int shiplen, correct;
		do {
		  shiplen = rand() % 4 + 1;
	  }while(!ships[shiplen]);
	  do {
			int x = rand() % LENGTH;
			int y = rand() % WIDTH;
			int dir = rand() % 4;
			Player1 lmao = {x, y, dir, shiplen};
			fwrite(&lmao, sizeof(lmao), 1, pipe[1]);
			fflush(pipe[1]); 
			fread(&correct, sizeof(correct), 1, pipe[0]);
			if (correct) {
			  ships[lmao.len]--;
			}
 		}while (!correct);
	}
	
	while (1) {
		fread(&b, sizeof(b), 1, pipe[0]);
		if (b.state == 1) {
		  break;
		}
		 int x, y;
		do {
			y = rand() % WIDTH;
			x = rand() % LENGTH;
		}while (b.board[x][y] != FREE);
	  Player2 lmao = {x, y};
	  fwrite(&lmao, sizeof(Player2), 1, pipe[1]);
	  fflush(pipe[1]);
	}
	//cout << "player1 ends";
  return 0;
}
