# Multiplayer Racing Game

A client-server racing game. 

![](./game.gif)

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
Developers:
- Edvards Zakovskis
- Henrik Gabrielyan

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

