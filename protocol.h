#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#define BUF_SIZE (512)
#define PAYLOAD_SIZE (BUF_SIZE - 4) // minus sizeof(enum) mtype_e for 64arch
#define WIN_LAP_COUNT (3)
#define CLIENTS_MAX_AMOUNT (2)

/*=====================================================*/

typedef enum {
    // server -> client Game Event
            MSG_GE = 1,
    // server -> client Player Update
            MSG_PU = 2,
    // server <- client Keyboard Input
            MSG_KI = 3,
} mtype_e;

typedef struct Message {
    mtype_e type;
    char payload[PAYLOAD_SIZE];
} Message;

/*=====================================================*/

struct Position {
    float x;
    float y;
};

struct Player {
    int id;
    struct Position position;
    // angle used to render correct direction of car
    float angle;
    // laps start from 0
    int laps;
};

// messages send from server to client to render next frame
struct PlayerUpdate {
    struct Player players[CLIENTS_MAX_AMOUNT];
};

/*=====================================================*/

typedef enum {
    // CONNECTION SUCCESSFUL
            EVENT_CS = 1,
    // NEW PLAYER ADDED
            EVENT_NP = 2,
} gevent_e;

// messages send from server to client about game event
struct GameEvent {
    gevent_e type;
};

/*=====================================================*/

//typedef enum {
//    // INPUT_UP = 1
//} kinput_e;

struct KeyboardInput {
    // x and y unit vectors, e.x. x = 0, y = 1
    // means vector (0,1) <=> keyboard UP
    int x;
    int y;
};

#endif