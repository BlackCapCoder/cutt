#include <stdbool.h>

// int is a bit wasteful because we only use the first 9 bits.
typedef int Board;

// Represents a game of ultimate tic tac toe
typedef struct Game {

  // Which local boards the next player can play on
  Board focus;

  // Which local boards has been won. 0=first player, 1=second player, 2=both
  Board stats[3];

  // Local boards. 0-8=first player, 9-17=second player
  Board board[2*9*9];

  // Current round number
  int round;

} Game;

// Utility variables
const Board

    // Individual tiles
    t1 = 0b000000001
  , t2 = 0b000000010
  , t3 = 0b000000100
  , t4 = 0b000001000
  , t5 = 0b000010000
  , t6 = 0b000100000
  , t7 = 0b001000000
  , t8 = 0b010000000
  , t9 = 0b100000000

  // All tiles
  , ta = 0b111111111

  // Rows
  , r1 = t1 | t2 | t3
  , r2 = t4 | t5 | t6
  , r3 = t7 | t8 | t9

  // Columns
  , c1 = t1 | t4 | t7
  , c2 = t2 | t5 | t8
  , c3 = t3 | t6 | t9

  // Diagonals
  , d1 = t1 | t5 | t9
  , d2 = t3 | t5 | t7
  ;


// Get a local board by index
Board getLocal (Game *g, int ix) {
  return (*g).board[ix];
}

// Has a board been won?
bool isWin (Board b) {

  // This monster should be slightly faster..
  return ( ((b & t5) == t5)
         && ( ((b & d1) == d1) || ((b & d2) == d2)
           || ((b & c2) == c2) || ((b & r2) == r2)
            )
         )
        || ( ((b & r1) == r1) || ((b & r3) == r3)
          || ((b & c1) == c1) || ((b & c3) == c3)
           );

  return (r1 == (b & r1)) || (r2 == (b & r2)) || (r3 == (b & r3))
      || (c1 == (b & c1)) || (c2 == (b & c2)) || (c3 == (b & c3))
      || (d1 == (b & d1)) || (d2 == (b & d2));

}

// Claims a square for the current player. Returns true if successful
bool _makeMove (Game *g, int gi, int li) {
  int g_bit = 1 << gi;

  if (!((*g).focus & g_bit)) return false;

  int turn  = (*g).round & 1;
  int bi    = gi + turn*9;
  Board b   = (*g).board[bi];
  int l_bit = 1 << li;

  if (b & l_bit) return false;

  b |= l_bit;

  if (isWin(b)) {
    (*g).stats[turn] |= g_bit;
    (*g).stats[2]    |= g_bit;
  }

  (*g).focus = (*g).stats[2] & l_bit? ~(*g).stats[2]: l_bit;
  (*g).board[bi] = b;
  (*g).round++;

  return true;

}
bool makeMove (Game *g, int gx, int gy, int lx, int ly) {
  return _makeMove(g, gy*3+gx, ly*3+lx);
}

