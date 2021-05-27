#include "game.h"
#include "protocol.h"

bool placeShip (board &b, const Player1 a, short ships[], char ch) {
	if (!ships[a.len])
	  return 0;
	board b1 = b;
  int x1 = a.x, y1 = a.y;
  for (int i = 1; i <= a.len; i++) {
	  if (b.board[x1][y1] != FREE)
      return 0;
    if (x1 < 0 || y1 < 0 || x1 >= LENGTH || y1 >= WIDTH)
	    return 0;
	  x1 += dl[a.dir];
	  y1 += dc[a.dir];
	}
	x1 = a.x; y1 = a.y;
	for (int i = 1; i <= a.len; i++) {
		b.board[x1][y1] = ch;
		for (int j = 0; j < 4; j++) {
			int lmao1 = x1 + dl[j];
			int lmao2 = y1 + dc[j];
			if (lmao1 < 0 || lmao2 < 0 || lmao1 >= LENGTH || lmao2 >= WIDTH)
			  continue;
			if (b.board[lmao1][lmao2] != ch && b.board[lmao1][lmao2] != FREE) {
				b = b1;
				return 0;
			}
		}
		x1 += dl[a.dir];
		y1 += dc[a.dir];
	}
	return 1;
}

void discoverShip (board &b, const board Fleet, int id) {
  for (int i = 0; i < LENGTH; i++)
    for (int j = 0; j < WIDTH; j++)
      if (Fleet.board[i][j] == id + '0') {
				b.board[i][j] = id + '0';
			}
	///functie de mare angajament
}

void setFree (board &b) {
  b.state = 0;
  for (int i = 0; i < LENGTH; i++)
    for (int j = 0; j < WIDTH; j++)
			b.board[i][j] = FREE;
}
