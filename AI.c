#include "Tic.c"

// Makes the first legal move possible
void AI_stupid (Game *g) {
  for (int gi = 0; gi < 9; gi++)
    for (int li = 0; li < 9; li++)
      if (_makeMove(g, gi, li))
        return;
}

// Win if possible
int win_board (Board b) {
  for (int i = 0; i < 9; i++) {
    int bit = 1 << i;
    if (b & bit || (b >> 9) & bit) continue;
    if (isWin(b | bit)) return i;
  }
  return -1;
}

// Block if possible
int block_board (Board b) {
  for (int i = 0; i < 9; i++) {
    int bit = 1 << i;
    if (b & bit || (b >> 9) & bit) continue;
    if (isWin((b >> 9) | bit)) return i;
  }
  return -1;
}

// Perfect tic tac toe strategy
int perfect_board (Board b) {
  int ret = -1;
  ret = win_board(b); if (ret != -1) return ret;
}

void AIMove (Game *g) {
  AI_stupid(g);
}
