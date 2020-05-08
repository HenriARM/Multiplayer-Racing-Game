#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "TextureManager.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../protocol.h"

struct GameObject {
    struct GameObject *next;
    int ID;
    float xpos;
    float ypos;
    float angle;
    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer *renderer;
} GameObject;

struct GameObject *headGameObject;
struct GameObject *tailGameObject;

void GameObjectUpdate(struct GameObject **object);

void GameObjectRender(struct GameObject *object);

struct GameObject *LoadNewGameObject(const char *texturesheet, int x, int y);

void updateGameObjects_players(struct PlayerUpdate * update);
struct GameObject *GameObjectCreateNode();

#endif