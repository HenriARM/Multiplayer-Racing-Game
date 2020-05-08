#include "Game.h"
#include "../protocol.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

int joined_players = 0;
#define PI 3.14159265
double val = PI / 180;

void InitializeGame() {
    memset(&players_data, 0, sizeof(players_data));
    memset(&game_data, 0, sizeof(game_data));
    for(int i= 0; i< CLIENTS_MAX_AMOUNT; i++) {
        players_data.players[i].position.x = 200;
        players_data.players[i].position.y = 200;
    }
}

// returns how much players we should wait to start game
int PlayersLeft() {
    return CLIENTS_MAX_AMOUNT - joined_players;
}

bool AddPlayer(int number) {
    joined_players++;
    players_data.players[number].id = number;
    game_data[number].id = number;
    return true;
}

void UpdateGame() {
    for (int i = 0; i < joined_players; i++) {
        UpdatePlayer(i);
    }
}
struct Position getRotation(struct Position pos,float angle);
bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh, float angle);
void UpdatePlayer(int index) {
    float carSpeed = game_data[index].speed;
    struct KeyboardInput *movement = &game_data[index].lastInput;
    struct Player *player = &players_data.players[index];
    bool collision = lineRect(100,100, 1500, 100, player->position.x , player->position.y, 60, 120,player->angle);
    collision += lineRect(1500, 100, 1500, 800, player->position.x , player->position.y, 60, 120,player->angle);
    collision += lineRect(1500, 800, 100, 800, player->position.x , player->position.y, 60, 120,player->angle);
    collision += lineRect(100, 800, 100, 100, player->position.x , player->position.y, 60, 120,player->angle);
    if(collision) {
        game_data[index].speed= 0;
        player->laps=0;
        player->position.x = 200;
        player->position.y = 200;
        return;
    }
    if (carSpeed != 0) {
        if (carSpeed >= 20)
            player->angle += movement->x;
        if (carSpeed < 20)
            player->angle += movement->x;
    }
    double angle = player->angle;
    double res = angle * val;
    float resultX = carSpeed / 10 * sin(res);
    float resultY = -carSpeed / 10 * cos(res);
    player->position.x += resultX;
    player->position.y += resultY;
    if (movement->y == -1 && carSpeed > -30) {
        carSpeed -= 1;
    }
    if (movement->y == 0 && carSpeed > 0) {
        carSpeed = carSpeed - 0.5;
    }
    if (movement->y == 1 && carSpeed < 50) {
        if (carSpeed > 10) {
            carSpeed += 0.5;
        } else
            carSpeed += 1;
    }
    if (movement->y == 0 && carSpeed < 0) {
        carSpeed = carSpeed + 0.5;
    }
    game_data[index].speed = carSpeed;
}

void UpdateKeyboardInput(int player_number, struct KeyboardInput input) {
    game_data[player_number].lastInput = input;
}


bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

    // calculate the direction of the lines
    float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

    // if uA and uB are between 0-1, lines are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
    return true;
    }
    return false;
}

bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh, float angle) {// TO DO: cos sin........
    struct Position center;
    center.x=rx+rw/2;
    center.y=ry+rh/2;
    struct Position point_1;
    point_1.x=-rw/2;
    point_1.y=-rh/2;
    point_1 = getRotation(point_1,angle);

    struct Position point_2;
    point_2.x=-rw/2;
    point_2.y=rh/2;
    point_2 = getRotation(point_2,angle);

    struct Position point_3;
    point_3.x=rw/2;
    point_3.y=-rh/2;
    point_3 = getRotation(point_3,angle);

    struct Position point_4;
    point_4.x=rw/2;
    point_4.y=rh/2;
    point_4 = getRotation(point_4,angle);

    // check if the line has hit any of the rectangle's sides
    // uses the Line/Line function below
    bool left =   lineLine(x1,y1,x2,y2, center.x+point_1.x,center.y+point_1.y, center.x+point_2.x, center.y+point_2.y);
    bool right =  lineLine(x1,y1,x2,y2, center.x+point_3.x,center.y+point_3.y, center.x+point_4.x, center.y+point_4.y);
    bool top =    lineLine(x1,y1,x2,y2, center.x+point_1.x,center.y+point_1.y, center.x+point_3.x, center.y+point_3.y);
    bool bottom = lineLine(x1,y1,x2,y2, center.x+point_2.x,center.y+point_2.y, center.x+point_4.x, center.y+point_4.y);

    // if ANY of the above are true, the line
    // has hit the rectangle
    if (left || right || top || bottom) {
    return true;
    }
    return false;
}
struct Position getRotation(struct Position pos,float angle){
    double val = PI / 180;
    int xp=pos.x;
    int yp=pos.y;
    pos.x = xp*cos(angle*val)- yp*sin(angle*val);
    pos.y = xp*sin(angle*val) + yp*cos(angle*val);
    return pos;
}