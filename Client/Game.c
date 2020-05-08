#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "Controllers/KeyboardController.h"

bool isRunning = false;
SDL_Window *window;
SDL_Renderer *renderer;
int cnt;
SDL_Texture *playerTex;
SDL_Rect srcR, destR;
TTF_Font* Sans;
SDL_Color White = {255, 255, 255};

struct GameObject *players[CLIENTS_MAX_AMOUNT];

void gameInitialize(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {

    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        printf("Subsystems initialized!...\n");
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            printf("Window created\n");
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Renderer created\n");
        }
        isRunning = true;
    }
    for( int i=0; i<CLIENTS_MAX_AMOUNT; i++)
    {
        char number = '0'+i+1;
        char name []="assets/carx.png";
        name[10]=number;
        players[i] = LoadNewGameObject(name, 200, 200);
    }
    Sans = TTF_OpenFont("Sans.ttf", 24);
    LoadMap();
}

void update() {
    cnt++;
}
void render() {
    SDL_RenderClear(renderer);
    DrawMap();
    SDL_SetRenderDrawColor(renderer, 255,0,0,0);
    SDL_RenderDrawLine(renderer, 800, 100, 800, 300);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i=0; i<CLIENTS_MAX_AMOUNT; i++) {
        GameObjectRender(players[i]);
    }
    SDL_RenderDrawLine(renderer, 100, 100, 1500, 100);
    SDL_RenderDrawLine(renderer, 1500, 100, 1500, 800);
    SDL_RenderDrawLine(renderer, 1500, 800, 100, 800);
    SDL_RenderDrawLine(renderer, 100, 800, 100, 100);

    SDL_RenderPresent(renderer);
}
void handleEvents() {
    handleEventsInputs();
}
void clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    printf("Game cleaned\n");
}

bool running() {
    return isRunning;
}

void STOP_GAME() {
    isRunning = false;
}

SDL_Renderer *gameGetRenderer() {
    return renderer;
}