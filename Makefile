CC=gcc
LIBS=include
RELEASE_CC_FLAGS=-O2
DEBUG_CC_FLAGS=-g -DDEBUG -Wall -Wextra -pedantic
CC_FLAGS=-I$(LIBS) $(shell pkg-config --cflags --libs sdl3)
CONFIG_RELEASE=$(CC_FLAGS) $(RELEASE_CC_FLAGS)
CONFIG_DEBUG=$(CC_FLAGS) $(DEBUG_CC_FLAGS)

main: main.c piece.c board.c coord.c ui.c game.c
	$(CC) $^ -o $@ $(CONFIG_DEBUG)

clean:	
	rm main