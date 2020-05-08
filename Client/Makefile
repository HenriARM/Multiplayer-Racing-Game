# A simple Makefile for compiling small SDL projects

# set the compiler
CC := gcc

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0  -Wall -lSDL2_image -lm -pthread -lSDL2_ttf
# add header files here
HDRS := Game.h TextureManager.h GameObject.h Map.h Controllers/CarController.h Controllers/KeyboardController.h Client.h ../protocol.h

# add source files here
SRCS := main.c Game.c TextureManager.c GameObject.c Map.c Controllers/CarController.c Controllers/KeyboardController.c Client.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := a

# default recipe
all: $(EXEC)
 
showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o Client $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
