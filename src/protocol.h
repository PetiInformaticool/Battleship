#pragma once

#include "game.h"
#include <stdio.h>

void sendBoard(FILE* pipe, board m);
board getBoard(FILE* pipe);

