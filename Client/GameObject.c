#include "GameObject.h"
#include "../protocol.h"

bool firstObject = true;
int ID = 0;


struct GameObject *LoadNewGameObject(const char *texturesheet, int x, int y) {
    if (firstObject) {
        headGameObject = GameObjectCreateNode();
        tailGameObject = headGameObject;
        firstObject = false;
    } else {
        tailGameObject->next = GameObjectCreateNode();
        tailGameObject = tailGameObject->next;
    }
    tailGameObject->xpos = x;
    tailGameObject->ypos = y;
    tailGameObject->destRect.x = x;
    tailGameObject->destRect.y = y;
    tailGameObject->srcRect.h = 240;
    tailGameObject->srcRect.w = 120;
    tailGameObject->destRect.w = tailGameObject->srcRect.w / 2;
    tailGameObject->destRect.h = tailGameObject->srcRect.h / 2;
    tailGameObject->renderer = gameGetRenderer();
    tailGameObject->objTexture = loadTexture(texturesheet);
    return tailGameObject;
}
void updateGameObjects_players(struct PlayerUpdate *update){
    struct GameObject * temp= headGameObject;
    int i=0;
    while(temp) 
    {
        temp->xpos = update->players[i].position.x;
        temp->ypos = update->players[i].position.y;
        temp->destRect.x= temp->xpos;
        temp->destRect.y= temp->ypos;
        temp->angle = update->players[i].angle;
        temp = temp->next;
        i++;
    }
}

struct GameObject *GameObjectCreateNode() {
    struct GameObject *temp = malloc(sizeof(GameObject));
    temp->next = NULL;
    temp->ID = ID;
    temp->angle = 0;
    temp->srcRect.x = 0;
    temp->srcRect.y = 0;
    ID++;
    return temp;
}

void GameObjectUpdate(struct GameObject **object) {

}

void GameObjectRender(struct GameObject *object) {
    SDL_Point center;
    center.x = 30;
    center.y = 60;
    SDL_RenderCopyEx((object)->renderer, (object)->objTexture, (&((object)->srcRect)), (&((object)->destRect)),
                     (object)->angle, &center, SDL_FLIP_NONE);
}