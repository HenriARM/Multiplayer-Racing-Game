#include "CarController.h"

#define PI 3.14159265

double carSpeed = 0;
double val = PI / 180;

void MoveForwardCarObject(struct GameObject **object, struct KeyboardMovement movement) {
    if (carSpeed != 0) {
        if (carSpeed >= 20)
            (*object)->angle += movement.x;
        if (carSpeed < 20)
            (*object)->angle += 2 * movement.x;
    }
    float resultX = carSpeed / 10 * sin((*object)->angle * val);
    float resultY = -carSpeed / 10 * cos((*object)->angle * val);
    if (IsCarPositionLegitimate(resultX + 30, resultY + 60, (*object)->angle)) {
        //  printf("Legitimate\n");
    } else {
        printf("ERORRR\n");
    }
    // test if there has been a collision
    (*object)->xpos += resultX;
    (*object)->ypos += resultY;
    (*object)->destRect.x = (int) (*object)->xpos;
    (*object)->destRect.y = (int) (*object)->ypos;
    /*
    (*object)->xpos += movement.x;
    (*object)->ypos -= movement.y;
    (*object)->destRect.x=(*object)->xpos;
    (*object)->destRect.y=(*object)->ypos;
    */
    if (movement.y == -1 && carSpeed > -30) {
        carSpeed -= 1;
    }
    if (movement.y == 0 && carSpeed > 0) {
        carSpeed = carSpeed - 0.5;
    }
    if (movement.y == 1 && carSpeed < 50) {
        if (carSpeed > 20) {
            carSpeed += 0.5;
        } else
            carSpeed += 1;
    }
}

bool IsCarPositionLegitimate(float center_xpos, float center_ypos, float angle) {
   // float ypos_1 = (center_ypos - 30) + 30 * sin(angle * val);
   // float xpos_1 = (center_xpos - 60);
    return true;
}