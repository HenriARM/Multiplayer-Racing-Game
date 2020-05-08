CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -pthread
LMFLAG = -lm

# the build target executable:
TARGET = Server

all: $(TARGET).c Game.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c Game.c $(LMFLAG)

clean:
	$(RM) $(TARGET)
