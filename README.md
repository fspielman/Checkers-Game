# Checkers-Game
## Overview
American Checkers game using the SFML C++ game library with:
- Mandatory maximum captures
- Backward captures
- A 5 minute timer to track players' time moving pieces

## Piece Outlines

### Gray Outline
Normal outline for checkers

### Blue Outline
A maximum jump is found, and pieces with a blue outline must be moved

Two single jumps are found: 

![alt text](images/twosinglejumps.png)

Only the piece with the most jumps is outlined in blue; the piece to the right of it does not have the longest path so it doesn't have a blue outline:

![alt text](images/longestjump.png)

### Yellow Outline
The yellow outline indicates that the piece that is selected, and the moves for that piece are displayed (shown below in the moves display)

## Moves display
The green moves are places where the piece can move to 

### Normal Move
![alt text](images/normalmove.png)

### Single Jump
![alt text](images/singlejump.png)

### Multi-Jump
![alt text](images/multijump1.png)
![alt text](images/multijump2.png)

## Classes

### Piece Class
- Takes care of Movement validation / moving piece circle position (normal move, single jump, and multi-jump)
- Making piece into a king
- Drawing pieces onto the board
- Handles piece selection 

### Board Class
- Draws board squares and initializes pieces to board(vector of vectors of shared pointers to Piece object) 
- Drawing the moves when a piece is selected
- Finding attacks
- Counting and displaying the number of pieces taken and handles losing condition (no pieces left)
- Moving a piece to a board coordinate

### Timer Class
- Tracks the timer during the current player's turn and displays it in the window
- Handles the losing condition when a player runs out of time




