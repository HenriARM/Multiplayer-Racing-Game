#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../protocol.h"
#include <stdbool.h>
#include <math.h>

struct PlayerUpdate players_data;
struct PlayersInfo {
    int id;
    float acceleration;
    float speed;
    struct KeyboardInput lastInput;
} game_data[CLIENTS_MAX_AMOUNT];

void InitializeGame();

int PlayersLeft();

bool AddPlayer(int socket);

void RemovePlayer(int socket);

void UpdatePlayer(int index);

void UpdateKeyboardInput(int socket, struct KeyboardInput input);

void UpdateGame();

#endif