CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS) -g

main: main.c piece.c board.c
	$(CC) $(CC_FLAGS) $^ -o $@ 