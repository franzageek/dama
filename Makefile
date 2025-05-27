CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS)

main: main.c piece.c
	$(CC) $(CC_FLAGS) $^ -o $@ 