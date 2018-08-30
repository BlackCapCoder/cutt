#include "Tic.c"

// Makes the first legal move possible
void AI_stupid (Game *g) {
  for (int gi = 0; gi < 9; gi++)
    for (int li = 0; li < 9; li++)
      if (_makeMove(g, gi, li))
        return;
}

void AIMove (Game *g) {
  AI_stupid(g);
}
