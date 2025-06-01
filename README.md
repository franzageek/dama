# dama
A Checkers implementation in C

## Introduction
dama (which is the italian word for the Checkers game) first started as an attempt by [@Raparubata](https://github.com/Raparubata) and me to "serialize" and enumerate all the practices involved in the game.<br>
We quickly discovered that the structure was pretty simple, so we started by laying out a base for a C implementation of the game. We planned out every aspect of it and then translated our logic into code. 
After figuring out all the logic (moves, captures and multiple captures), I wrote a basic GUI using SDL3 for us to be able to tinker with the game and discover eventual problems in a more "visual" way. 
We didn't even think once of building a GUI at first, our original idea was to prompt the user for some X,Y coordinates of a specific source piece and then prompt them again for a destination, all while writing a bunch of `[ ]` and `[O]` representing empty tiles and busy tiles.<br>
It then turned out to be a lot easier for us to build a full-fledged GUI using some simple cross-platform video framework such as SDL.
It took about a week's worth of efforts to effectively build the game piece-by-piece, and then another three days for it to turn into something vaguely resembling a chess board with some functional pieces.

## Logic
We split up the main logic in two parts:
- a 24-item array of `piece_t` structures, each holding a bunch of properties regarding that specific piece, such as:
    - the tile it is on (`coord_t coord`);
    - the player it belongs to (`bool player`);
    - whether it has already been captured (`bool valid`);
    - whether it is a king or not (`bool king`);
- a 4x8 array of bytes (the so-called `a32`) which represents all the black tiles: each byte holds the index in the `piece_t` array of the piece that sits on top of it. This is extremely efficient when moving pieces, as you just have to move the index from a tile to another and then clear the old one.
<br>

There is an additional `board_t` structure which holds three main properties:
- whose turn it is;
- a pointer to the `piece_t` array;
- a pointer to the `a32` array.

## Directory structure
- `board.h` and `board.c`: board structure and functions to allocate/free space for it;
- `coord.h` and `coord.c`: structures and helper functions for working with the coordinate system used throughout the whole program;
- `game.h` and `game.c`: core game logic (main loop, click handling, move and capture calculation);
- `piece.h` and `piece.c`: structures for pieces, tiles and capture chains, along with functions to move pieces, calculate moves and captures that are mainly used inside `game.c`;
- `ui.h` and `ui.c`: SDL3 initialization code, along with functions to draw cirlces and rectangles that are then used to draw tiles and pieces in `game.c`;
- `main.c`: calls to functions for initializing SDL3, call to main game loop, calls to destruction functions.