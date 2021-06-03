#pragma once

#include "game.h"
#include <stdio.h>

int sendBoard(FILE* pipe, board m);
board getBoard(FILE* pipe);

int sendShips (FILE* pipe, int ship[]);
void getShips (FILE* pipe, int ship[]);

int sendBomb (FILE* pipe, int bomb[]);
void getBomb (FILE* pipe, int bomb[]);



