#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>


#include "../protocol.h"
#include "Game.h"

#define TRUE   (1)
#define FALSE (0)
#define PORT (8888)

//=========== SHARED MEMORY ========================
char buff[BUF_SIZE];
int client_socket[CLIENTS_MAX_AMOUNT];

int master_socket, addrlen, new_socket, sd;
int max_sd;
struct sockaddr_in master_addr;
fd_set readfds;

pthread_t thread_id[2];
pthread_mutex_t lock;
int is_thread_running = TRUE;
//==================================================

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

void clear_buff() {
    memset(buff, '\0', BUF_SIZE);
}

void interpret_message(char *buff, int player_number) {
    Message *message = (Message *) buff;
    switch (message->type) {
        case MSG_KI: {
            struct KeyboardInput *payload = (struct KeyboardInput *) message->payload;
            struct KeyboardInput input;
            input.x = payload->x;
            input.y = payload->y;
            UpdateKeyboardInput(player_number, input);
            // To do move based on input
            // printf("Keyboard input x:%d y:%d from player %d\n", payload->x, payload->y, player_number);
            break;
        }
        default:
            break;
    }
}

void int_handler() {
    // send message to all clients, that server is stopped and close fds and exit
    clear_buff();
    sprintf(buff, "\nServer disconnected!\n");
    for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
        if (client_socket[i] != 0) {
            if (send(client_socket[i], buff, BUF_SIZE, 0) != BUF_SIZE) {
                perror("ERROR  send");
                close(client_socket[i]);
            }
        }
    }
    is_thread_running = FALSE;
    error("\nERROR Server is disconnected\n");
}

void *run() {
    pthread_t id = pthread_self();

    // each thread has its task, 0-th thread read from client messages, but 1-th publish to clients updates
    if (pthread_equal(id, thread_id[0])) {
        printf("Server thread started processing...\n");

        while (is_thread_running) {
            // clear the socket set
            FD_ZERO(&readfds);

            // add master socket to set
            FD_SET(master_socket, &readfds);
            max_sd = master_socket;

            // add child sockets to set
            for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                //socket descriptor
                sd = client_socket[i];
                //if valid socket descriptor then add to read list
                if (sd > 0) FD_SET(sd, &readfds);
                //highest file descriptor number, need it for the select function
                if (sd > max_sd) max_sd = sd;
            }

            // wait for an activity on one of the sockets , timeout is NULL ,
            // select() returns amount of ready fds
            if ((select(max_sd + 1, &readfds, NULL, NULL, NULL) == -1) && (errno != EINTR)) perror("ERROR select");


            // if something happened on the master socket then its an incoming connection
            if (FD_ISSET(master_socket, &readfds)) {
                if ((new_socket = accept(master_socket, (struct sockaddr *) &master_addr, (socklen_t *) &addrlen)) ==
                    -1)
                    perror("ERROR accept");

                // if we have enough players, close client connection
                if (PlayersLeft() == 0) {
                    clear_buff();
                    sprintf(buff, "Sorry, game already started!\n");
                    if (send(new_socket, buff, BUF_SIZE, 0) != BUF_SIZE) perror("send");
                    printf("Refusal for connection to game sent successfully\n");
                    close(new_socket);
                }

                printf("\nNew connection, socket fd is %d , ip is : %s , port : %d\n", new_socket,
                       inet_ntoa(master_addr.sin_addr), ntohs(master_addr.sin_port));

                // add new socket to array of sockets
                for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                    if (client_socket[i] == 0) {
                        client_socket[i] = new_socket;
                        AddPlayer(i);
                        printf("Player N %d created\n\n", i);
                        break;
                    }
                }

                // send greeting message
                if (PlayersLeft() > 0) {
                    clear_buff();
                    sprintf(buff, "Welcome to the racing game! Waiting for %d more players...\n", PlayersLeft());
                } else if (PlayersLeft() == 0) {
                    clear_buff();
                    sprintf(buff, "Welcome to the racing game! Enough players are connected \n");
                }
                if (send(new_socket, buff, BUF_SIZE, 0) != BUF_SIZE) perror("ERROR send");
                printf("Greeting message sent successfully\n");

                // report about new player to other players
                for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                    if (client_socket[i] != 0 && client_socket[i] != new_socket) {
                        clear_buff();
                        sprintf(buff, "\nNew player entered\n");
                        if (send(client_socket[i], buff, BUF_SIZE, 0) != BUF_SIZE) {
                            perror("ERROR send");
                        }
                    }
                }

                // start game if enought players connected
                if (PlayersLeft() == 0) {
                    for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                        if (client_socket[i] != 0) {
                            clear_buff();
                            sprintf(buff, "Game started!\n");
                            if (send(client_socket[i], buff, BUF_SIZE, 0) != BUF_SIZE) perror("ERROR send");
                        }
                    }
                }
            } else {
                //some I/O operation on some other socket
                for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                    sd = client_socket[i];

                    // check whether other socket is ready for communication
                    if (FD_ISSET(sd, &readfds)) {
                        // check if it was for closing , and also read the incoming message
                        clear_buff();
                        if (read(sd, buff, BUF_SIZE) == 0) {
                            // client disconnected
                            getpeername(sd, (struct sockaddr *) &master_addr, (socklen_t *) &addrlen);
                            printf("Host disconnected , ip %s , port %d \n", inet_ntoa(master_addr.sin_addr),
                                   ntohs(master_addr.sin_port));
                            printf("Player N %d exited\n\n", i);

                            // close the socket and mark as 0 in list for reuse
                            close(sd);
                            client_socket[i] = 0;
                        } else {
                            //======================Communication with players=================================

                            // host disconnection send empty buffer, but read() returned val is not yet zero
                            if (strlen(buff) == 0) {
                                continue;
                            }

                            interpret_message(buff, i);

                        }
                    }
                }
            }
        }
        // when sending SIGINT to Server, breaks loop and exit thread
        pthread_exit(NULL);
    } else {
        printf("Publish thread started processing...\n");
        // loop all clients to publish player coordinates
        int UPS = 90; // updates per second
        long sleepTime = 1000 / UPS;
        double time_spent = 0.0;
        while (is_thread_running) {
            // lock SHARED MEMORY
            pthread_mutex_lock(&lock);
            clock_t begin = clock();

            // create message
            clear_buff();
            Message *message = (Message *) buff;
            message->type = MSG_PU;
            memcpy((void *) message->payload, (void *) &players_data, sizeof(players_data));
            UpdateGame();
            // publish
            for (int i = 0; i < CLIENTS_MAX_AMOUNT; i++) {
                if (client_socket[i] != 0) {
                    send(i, buff, sizeof(buff), 0);
                    if (send(client_socket[i], buff, BUF_SIZE, 0) != BUF_SIZE) perror("ERROR send");
                }
            }
            //  printf("Player updates published to all\n");

            // unlock SHARED MEMORY
            clock_t end = clock();
            time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
            pthread_mutex_unlock(&lock);
            if (time_spent < sleepTime) {
                sleep_ms(sleepTime - time_spent);
            }
        }
        // when sending SIGINT to Server, breaks loop and exit thread
        pthread_exit(NULL);
    }
}

int main() {
    signal(SIGINT, int_handler);

    pthread_mutex_init(&lock, NULL);

    // initialize all client sockets and positions to zero
    InitializeGame();

    // create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) error("ERROR opening socket");
    printf("Socket successfully created\n");

    // set master socket to allow multiple connections
    int opt = TRUE;
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) == -1)
        error("ERROR setting socket options");
    printf("Socket options successfully set\n");

    // assign socket type, IP, port
    master_addr.sin_family = AF_INET;
    master_addr.sin_addr.s_addr = INADDR_ANY;
    master_addr.sin_port = htons(PORT);

    // bind the socket to localhost port
    if (bind(master_socket, (struct sockaddr *) &master_addr, sizeof(master_addr)) == -1) error("Error binding socket");
    printf("Socket successfully binded to port %d \n", PORT);

    // 3 pending connections for the master socket
    if (listen(master_socket, 3) == -1) error("Error listening to connections");
    printf("Server waiting for connections...\n\n");

    addrlen = sizeof(master_addr);

    // create separate thread for publishing player updates
    if (pthread_create(&(thread_id[0]), NULL, run, NULL) != 0) perror("ERROR can't create a server thread");
    printf("Server thread created successfully\n");

    if (pthread_create(&(thread_id[1]), NULL, run, NULL) != 0) perror("ERROR can't create a publish thread");
    printf("Publish thread created successfully\n\n");

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}