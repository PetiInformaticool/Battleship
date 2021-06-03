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
		if (b[_i][_j] >= '0' && b[_i][_j] <= '9')
			return 1;	
	}
	return 0;
}

pair < int, int > partial(char b[][WIDTH]) {
	for (int i = 0; i < LENGTH; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			if (b[i][j] == HIT) {
				int vecini(0), orientare(-1);
				int Free = 0;
				for (int k = 0; k < 4; ++k) {
					int _i = i + dl[k];
					int _j = j + dc[k];
					if (_i < 0 || _i >= LENGTH || _j < 0 || _j >= WIDTH) 
						continue;
					if (b[_i][_j] == HIT) {
						++vecini, orientare = (k > 1);
					}
					if (b[_i][_j] == FREE)
					  Free++;
				}
				if (vecini > 1 || Free == 0)
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
	FILE* pipe[2];


	int ships[6]= {0, 0, 3, 3, 2, 2};
	int bomb[2] = {1, 1};

	board b;
void receiveSmecherie () {
  b = getBoard(pipe[0]);
  getShips(pipe[0], ships);
  getBomb(pipe[0], bomb);
}

int main () {
	srand(time(NULL));
	pipe[0] = fopen("pipes/server0", "rb");
	pipe[1] = fopen("pipes/player0", "wb");

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
		receiveSmecherie();
		if (b.state == 1) 
		  break;
		cout << "BOOOOOOOOOOOOMBS: " << bomb[0] << "\n";
		if (bomb[1] > 0) {
			cout << "lmaoooooooooooooo\n";
			Player2 lmao;
				lmao = {4, rand() % 14, rand() % 14};

			fwrite(&lmao, sizeof(Player2), 1, pipe[1]);
			fflush(pipe[1]);
			continue;
		}
	  if (bomb[0] > 0) {
			Player2 lmao = {2, rand() % 15, 1};
			fwrite(&lmao, sizeof(Player2), 1, pipe[1]);
			fflush(pipe[1]);
			continue;
		}
		cout << "cooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooox\n";
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
	  Player2 lmao = {1, x, y};
	  fwrite(&lmao, sizeof(Player2), 1, pipe[1]);
	  fflush(pipe[1]);
	  
	}
  //cout << "player2 ends";
  return 0;
}
