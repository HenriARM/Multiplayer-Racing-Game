#include "Client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "Game.h"
#include "Controllers/KeyboardController.h"
#include <SDL2/SDL_timer.h>
#include "GameObject.h"

#define PORT (8888)
#define TRUE   (1)

int socket_fd;
struct sockaddr_in serv_addr;
char buff[BUF_SIZE];

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void clear_buff() {
    memset(buff, '\0', BUF_SIZE);
}

void read_from_buff() {
    clear_buff();
    ssize_t n = read(socket_fd, buff, BUF_SIZE);
    if (n < 0) error("ERROR reading from socket");
    if (strlen(buff)) printf("Message from server: %s\n", buff);
}

void *listen_server() {
    while (TRUE) {
        read_from_buff();
        Message *message = (Message *) buff;
        struct PlayerUpdate *payload;
        switch (message->type) {
            case MSG_PU:
                payload = (struct PlayerUpdate *) message->payload;
                updateGameObjects_players(payload);
                printf("Player Updates arrived successfully\n");
                break;
            default:
                break;
        }
    }
}

void send_message() {
    // Sending Keyboard Inputs for now only
    handleEvents();
    Message *message = (Message *) buff;
    message->type = MSG_KI;
    struct KeyboardInput input = getAction();
    printf("Client Keyboard x:%d y:%d\n", input.x, input.y);
    memcpy((void *) message->payload, (void *) &input, sizeof(input));
    send(socket_fd, buff, sizeof(buff), 0);
}

void start_client() {
    // socket creation
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) error("ERROR opening socket");
    printf("Socket successfully created\n");

    // connect to server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    // check connection
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting to server");
    printf("Socket successfully connected to server with ip %s and port %d\n", inet_ntoa(serv_addr.sin_addr), PORT);

    // first message from server could be refusal to enter the game
    read_from_buff();
    if (strcmp(buff, "Sorry, game already started!\n") == 0) error("ERROR game already started");

    // wait until game is started (<=> enough players connected)
    do {
        read_from_buff();
    } while (strcmp(buff, "Game started!\n") != 0);

    // create separate thread which listens server
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, listen_server, NULL);
    printf("Listening server...");
}

void stop_client() {
    close(socket_fd);
    printf("Client is stopped\n");
}
