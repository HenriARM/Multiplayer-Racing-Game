#include "KeyboardController.h"
#include "../Game.h"
#include "CarController.h"
#include "../../protocol.h"

int x = 0;
int y = 0;

void handleKeyboardInputs(SDL_Event event) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        if (state[SDL_SCANCODE_Q])
            STOP_GAME();
        x = 0;
        y = 0;
        if (state[SDL_SCANCODE_UP]) {
            y = 1;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            x = 1;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            x = -1;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            y = -1;
        }
        if (state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_UP]) {
            y = 0;
        }
        if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_RIGHT]) {
            x = 0;
        }
    }
}

void handleEventsInputs() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            STOP_GAME();
            break;
        case SDL_KEYDOWN:
            handleKeyboardInputs(event);
        case SDL_KEYUP:
            handleKeyboardInputs(event);
            break;
        default:
            break;
    }
    return;
}

struct KeyboardInput getAction() {
    struct KeyboardInput a;
    a.x = x;
    a.y = y;
    return a;
}