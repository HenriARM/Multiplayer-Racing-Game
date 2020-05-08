#include "TextureManager.h"


SDL_Texture *loadTexture(const char *texture) {
    SDL_Surface *temp = IMG_Load(texture);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(gameGetRenderer(), temp);
    SDL_FreeSurface(temp);

    return tex;
}

void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(gameGetRenderer(), tex, &src, &dest);
}