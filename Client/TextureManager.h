#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

SDL_Texture *loadTexture(const char *texture);

void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);

#endif