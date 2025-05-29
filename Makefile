CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS) -g

main: main.c piece.c board.c coord.c
	$(CC) $(CC_FLAGS) $^ -o $@ 