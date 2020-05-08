#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "Game.h"
#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "TextureManager.h"
#include <stdbool.h>
#include <stdlib.h>
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
#include "../protocol.h"

void send_message();

void start_client();

void stop_client();

#endif