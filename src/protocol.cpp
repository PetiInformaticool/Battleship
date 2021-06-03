#include "protocol.h"
#include <signal.h>
#include <stdlib.h>


int sigpipeReceived = 0;

void sigpipeHandler(int parameter) {
  sigpipeReceived = 1;
}

int sendBoard(FILE* pipe, board m) {
	sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);
  fwrite(&m, sizeof(m), 1, pipe);
  fflush(pipe);
  return !sigpipeReceived;
}
board getBoard(FILE* pipe) {
  board m;
  if (fread(&m, sizeof(m), 1, pipe) != 1) {
		system("killall sevrer");
		system("killall player");
		system("killall player2");
	}
  return m;
}



int sendShips (FILE* pipe, int ship[]) {
	sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);
	for (int i = 2; i <= 5; i++)
	  fwrite(&ship[i], sizeof(ship[i]), 1, pipe);
	fflush(pipe);
	return !sigpipeReceived;
}
void getShips (FILE* pipe, int ship[]) {
	for (int i = 2; i <= 5; i++)
    if (fread(&ship[i], sizeof(ship[i]), 1, pipe) != 1) {
			system("killall sevrer");
			system("killall player");
			system("killall player2");
		}
}

int sendBomb (FILE* pipe, int bomb[]) {
	sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);
	for (int i = 0; i < 2; i++)
  fwrite(&bomb[i], sizeof(bomb[i]), 1, pipe);
  fflush(pipe);
 return !sigpipeReceived;
}
void getBomb (FILE* pipe, int bomb[]) {
	for (int i = 0; i < 2; i++) 
		 if (fread(&bomb[i], sizeof(bomb[i]), 1, pipe) != 1) {
		  system("killall sevrer");
			system("killall player");
			system("killall player2");
		 }
}


