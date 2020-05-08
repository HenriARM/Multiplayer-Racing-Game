#include <stdlib.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include "Game.h"
#include "Client.h"

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 900;
#define FULLSCREEN false

int main() {
    // client start should be always executed from the beginning,
    // since it is returned when game is started and SDL window could be initialized
    start_client();

    const int FPS = 90;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frametime;
    gameInitialize("CarGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN);

    // main loop is sending Keyboard Inputs to Server
    // and separate thread waits for Player Updates from Server
    while (running()) {
        frameStart = SDL_GetTicks();
        send_message();
        update();
        render();

        frametime = SDL_GetTicks() - frameStart;
        if (frameDelay > frametime) {
            SDL_Delay(frameDelay - frametime);
        }
    }
    stop_client();
    clean();

    return 0;
} 
