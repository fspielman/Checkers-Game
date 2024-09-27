# Checkers-Game
## Overview
American Checkers game using the SFML C++ game library with:
- Mandatory maximum captures
- Backward captures
- 5 minute timer to track players' time moving pieces

## Piece Outlines

### Gray Outline
Normal outline for checkers

### Blue Outline
Maximum jump is found and pieces with blue outline must be moved

Two single jumps are found: 

![alt text](image-3.png)

Only the piece with the most amount of jumps is outlined in blue; the piece to the right of it does not have the longest path:

![alt text](image-8.png)

### Yellow Outline
Yellow outline is the piece that is selected and moves for that piece are displayed (shown below in the moves display section)

## Moves display
The green moves are places where the piece can move to 

### Normal Move
![alt text](image.png)

### Single Jump
![alt text](image-4.png)

### Multi-Jump
![alt text](image-5.png)
![alt text](image-6.png)

## Classes

### Piece Class
- Kinging
- Takes care of Movement validation / moving piece circle position (normal move, single jump, and multi-jump)
- Drawing pieces onto board
- Handles piece selection 

### Board Class
- Drawing board squares and initializing pieces to board (std::vector<std::vector<std::shared_ptr<Piece>>>) 
- Drawing the moves when piece is selected
- Finding attacks
- Counting and displaying number of pieces taken and handles losing condition (no pieces left)
- Moving piece to board coordinate

### Timer Class
- Tracks timer during current player's turn and displays it to screen
- Handles losing condition when a player runs out of time




