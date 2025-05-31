CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS) -g -DDEBUG $(shell pkg-config --cflags --libs sdl3)

main: main.c piece.c board.c coord.c ui.c game.c
	$(CC) $^ -o $@ $(CC_FLAGS) 

clean:	
	rm main