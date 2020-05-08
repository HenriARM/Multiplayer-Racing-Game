#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
/* ^^ these are the include guards */

/* Prototypes for the functions */
/* Sums two ints */


void gameInitialize(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

void update();

void render();

void handleEvents();

void clean();

void GameDeinitialize();

bool running();

void STOP_GAME();

SDL_Renderer *gameGetRenderer();

#endif