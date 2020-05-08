# LSP_Gala_darbs

A client-server racing game. 

# How to run:

Prerequisites: Ubuntu x86, gcc

1. Install [SDL2](https://gist.github.com/BoredBored/3187339a99f7786c25075d4d9c80fad5)

2. Run server

    ```bash
    cd Server & make all;
    ./Server
    ```

3. Run client

    ```bash
    cd Client & make all;
    ./Client
    ```

# Variable naming helper for Edvards:
1. all structures start from upper case and use bubbel style (e.x. game info -> GameInfo)
2. all primitive variable (inside structure too) start from lower case and use snake style (e.x. game info -> game_info)
3. all predefined, global variables use snake style and all letters are upper case (e.x. #define MAX_ARRAY_SIZE)
4. comments like this "// dsdadasa"

Developers:
- Edvards Zakovskis - 50% contribution
- Henrik Gabrielyan - 50% contribution

## Features

* Client server architecture

* Server listens for multiple client connections using multiplexer (man select())

* Client renders game using SDL. 

Server features:
* Listen to multiple players
* Connect them and start game
* In case, server is disconnected, stop all clients and stop the game
* Publish to all clients player updates using separate thread

Client
* Different amount car rendering
* Collision detection
* Acceleration
* Lap counting

