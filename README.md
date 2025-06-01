# dama
A Checkers implementation in C

## Introduction
dama (which is the italian word for the Checkers game) first started as an attempt by [@Raparubata](https://github.com/Raparubata) and me to "serialize" and enumerate all the practices involved in the game.<br>
We quickly discovered that the structure was pretty simple, so we started laying out a base for a C implementation of the game. We planned out every aspect of it and then translated our logic into code -- it took about a week's worth of efforts to effectively have something vaguely resembling a chess board with some functional pieces.
## Logic
We split up the main logic in two parts:
- a 24-item array of `piece_t` structures, each holding a bunch of properties regarding that specific piece, such as:
    - the tile it is on (`coord_t coord`);
    - the player it belongs to (`bool player`);
    - whether it has already been captured (`bool valid`);
    - whether it is a king or not (`bool king`);
- a 4x8 array of bytes which represents all the black tiles: each byte holds the index in the `piece_t` array of the piece that sits on top of it. This is extremely efficient when moving pieces, as you just have to move the index from a tile to another and then clear the old one.

There is an additional `board_t` structure which holds three main properties: