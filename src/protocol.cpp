#include "protocol.h"

void sendBoard(FILE* pipe, board m) {
  fwrite(&m, sizeof(m), 1, pipe);
  fflush(pipe);
}
board getBoard(FILE* pipe) {
  board m;
  fread(&m, sizeof(m), 1, pipe);
  return m;
}
