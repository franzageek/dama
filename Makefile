CC=gcc
LIBS=include
CC_FLAGS=-I$(LIBS) -g -DDEBUG

main: main.c piece.c board.c coord.c
	$(CC) $(CC_FLAGS) $^ -o $@ 

clean:	
	rm main