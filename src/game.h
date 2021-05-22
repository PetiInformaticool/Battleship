#pragma once


const char FREE = '.';
const char SHIP = '*';

const bool PLACE = 0;
const bool DESTROY = 1;
const int LENGTH = 10;
const int WIDTH = 10;
const char HIT = 'X';
const char MISS = '*';

const int dl[] = {0, 0, -1, 1};
const int dc[] = {1, -1, 0, 0};


struct board {
	bool state;
  char board[LENGTH][WIDTH];
};

struct Player1 {
  int x, y, dir, len;
};

struct Player2 {
  int x, y;
};

bool placeShip (board &b, const Player1 a, short ships[], char ch);

void discoverShip(board &b, const board Fleet, int id);

void setFree (board &b);


