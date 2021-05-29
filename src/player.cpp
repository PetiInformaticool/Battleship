#include "game.h"
#include "protocol.h"
#include <bits/stdc++.h>


using namespace std;

bool vecin(int i, int j, char b[][WIDTH]) {
	for (int k = 0; k < 4; ++k) {
		int _i = i + dl[k];
		int _j = j + dc[k];
		if (_i < 0 || _i >= LENGTH || _j < 0 || _j >= WIDTH) 
			continue;
		if (b[_i][_j] >= '0' && b[_i][_j] < '9')
			return 1;	
	}
	return 0;
}

pair < int, int > partial(char b[][WIDTH]) {
	for (int i = 0; i < LENGTH; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			if (b[i][j] == HIT) {
				int vecini(0), orientare(-1);
				for (int k = 0; k < 4; ++k) {
					int _i = i + dl[k];
					int _j = j + dc[k];
					if (_i < 0 || _i >= LENGTH || _j < 0 || _j >= WIDTH) 
						continue;
					if (b[_i][_j] == HIT)
						++vecini, orientare = (k > 1);
				}
				if (vecini > 1)
					continue;
				if (orientare + 1) {
					for (int k = orientare * 2; k < (1 + orientare) * 2; ++k) {
						int _i = i + dl[k];
						int _j = j + dc[k];
						if (_i < 0 || _i >= LENGTH || _j < 0 || _j >= WIDTH) 
							continue;
						if (b[_i][_j] == FREE && !vecin(_i, _j, b)) 
							return {_i, _j};
					}
				}
				else {
					for (int k = 0; k < 4; ++k) {
						int _i = i + dl[k];
						int _j = j + dc[k];
						if (_i < 0 || _i >= LENGTH || _j < 0 || _j >= WIDTH) 
							continue;
						if (b[_i][_j] == FREE && !vecin(_i, _j, b)) 
							return {_i, _j};
					}
				}
			}
		}
	}
	return {-1, -1};
}

int main () {
	srand(time(NULL)*123456);
	FILE* pipe[2];
	pipe[0] = fopen("pipes/serverX", "rb");
	pipe[1] = fopen("pipes/playerX", "wb");
	board b;
	int ships[]= {0, 0, 3, 2, 1, 1};
	while (1) {
		b = getBoard(pipe[0]);
		if (b.state == 1) {
			break;
		}
		int shiplen, correct;
		do {
			shiplen = rand() % 4 + 2;
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
		b = getBoard(pipe[0]);
		if (b.state == 1) 
		  break;
		
		int x, y;	
		pair < int, int > part = partial(b.board);
		if (part.first == -1) {
			do {
				y = rand() % WIDTH;
				x = rand() % LENGTH;
			}while (b.board[x][y] != FREE || ((x + y)&1) == 0 || vecin(x, y, b.board));
		}
		else 
			x = part.first, y = part.second;
	  Player2 lmao = {x, y};
	  fwrite(&lmao, sizeof(Player2), 1, pipe[1]);
	  fflush(pipe[1]);
	  
	}
  //cout << "player2 ends";
  return 0;
}
