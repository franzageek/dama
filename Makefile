CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS)

main: main.c
	$(CC) $(CC_FLAGS) $^ -o $@ 