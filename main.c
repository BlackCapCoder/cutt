#include <SDL2/SDL.h>
#include <stdio.h>
#include "Tic.c"

const float w = 800 // Window width
          , h = 800 // Window height
          , pad = 4 // Padding between local boards
          , s = (w<h? w: h)/3
          ;

// Which square the user is hovering over
int marked_gx = -1
  , marked_gy = -1
  , marked_lx = -1
  , marked_ly = -1;

// The game
Game game = {.focus = ta};


void drawSquare (SDL_Renderer* rend, float x, float y, float size, float r, float g, float b, float a) {
  SDL_SetRenderDrawColor(rend, r*255, g*255, b*255, a*255);
  SDL_Rect rect = { .x = x, .y = y, .w = size, .h = size };
  SDL_RenderFillRect(rend, &rect);
}
void drawBoard (SDL_Renderer* rend, Board b1, Board b2, float bx, float by, float bs, int marked) {
  const float pad = 2, s = bs/3;

  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      int i = y*3+x, t = 1 << i;

      bool red   = b1 & t
         , blue  = b2 & t
         , white = !red && !blue;

      float r = red || white? 1: 0
          , g = white? 1: 0
          , b = blue || white? 1: 0
          , a = white? (marked == i? 0.5: 0.3): 1;

      drawSquare( rend
                , bx + s*x + pad*(x%3)
                , by + s*y + pad*(y%3)
                , s - pad*2
                , r, g, b, a
                );
    }
  }
}
void drawGame (SDL_Renderer* rend, Game g) {
  if (isWin(g.stats[0])) {
    drawSquare(rend, 0, 0, s*3, 1, 0, 0, 1);
    return;
  }

  if (isWin(g.stats[1])) {
    drawSquare(rend, 0, 0, s*3, 0, 0, 1, 1);
    return;
  }

  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      int i = y*3 + x, g_bit = 1 << i;
      Board b1 = getLocal(g, i);
      Board b2 = getLocal(g, i+9);

      const float px = s*x + pad*(x%3)
                , py = s*y + pad*(y%3)
                , ps = s - pad*2;

      if (game.stats[0] & g_bit) {
        drawSquare(rend, px, py, ps, 1, 0, 0, 1);
      } else if (game.stats[1] & g_bit) {
        drawSquare(rend, px, py, ps, 0, 0, 1, 1);
      } else {
        if (g.focus & g_bit)
          drawSquare(rend, px, py, ps, 1,1,1,0.1);

        drawBoard( rend, b1, b2, px, py, ps
                 , marked_gx == x && marked_gy == y? marked_ly*3 + marked_lx: -1);

      }
    }
  }
}

int num_redraws = 0;
void renderLoop (SDL_Window* w) {
  SDL_bool done   = SDL_FALSE;
  SDL_bool redraw = SDL_TRUE;

  SDL_Renderer* rend = NULL;
  rend = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

  // Main loop
  while (!done) {
    SDL_Event event;

    // Process events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          done = SDL_TRUE;
          break;
        }
        case SDL_MOUSEMOTION: {
          int x = event.motion.x;
          int y = event.motion.y;

          int gx, gy, lx, ly;

          gx = x / s;
          gy = y / s;
          lx = (x - marked_gx*s) / (s/3);
          ly = (y - marked_gy*s) / (s/3);

          if (marked_gx != gx || marked_gy != gy || marked_lx != lx || marked_ly != ly) {
            marked_gx = gx; marked_gy = gy; marked_lx = lx; marked_ly = ly;
            redraw = SDL_TRUE;
          }

          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          if (makeMove(&game, marked_gx, marked_gy, marked_lx, marked_ly))
            redraw = SDL_TRUE;
          break;
        }
      }
    }

    // Redraw if we have to
    if (redraw) {
      SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
      SDL_RenderClear(rend);

      drawGame(rend, game);

      SDL_RenderPresent(rend);
      redraw = SDL_FALSE;
      printf("Redraw: %d\n", ++num_redraws);
    }
  }

  SDL_DestroyRenderer(rend);
}

int main(void) {
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Ultimate Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Window is NULL!\n");
    return EXIT_FAILURE;
  }

  renderLoop(window);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
