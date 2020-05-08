#ifndef KEYBOARDCONTROLLER_H_INCLUDED
#define KEYBOARDCONTROLLER_H_INCLUDED

#include "../GameObject.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
/* ^^ these are the include guards */

/* Prototypes for the functions */
/* Sums two ints */
struct KeyboardMovement {
    int x;
    int y;
} KeyboardMovement;

void handleKeyboardInputs(SDL_Event event);

void handleEventsInputs();

struct KeyboardInput getAction();

#endif