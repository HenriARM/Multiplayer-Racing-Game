#ifndef CARCONTROLLER_H_INCLUDED
#define CARCONTROLLER_H_INCLUDED

#include "../GameObject.h"
#include "KeyboardController.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <math.h>
/* ^^ these are the include guards */

/* Prototypes for the functions */
/* Sums two ints */
void MoveForwardCarObject(struct GameObject **object, struct KeyboardMovement movement);

bool IsCarPositionLegitimate(float center_xpos, float center_ypos, float angle);

#endif