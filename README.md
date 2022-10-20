# Console-Pacman
First C++ project. PACMAN++ uses threading, windows graphics, struct pointers, recursive functions and basic AI for all 4 ghosts, along with being dynamic enough to scale to multiple levels. Completely text-based on the Windows console.

![pacman](https://user-images.githubusercontent.com/45783867/196856309-a79d3a0b-b26c-4dd9-ba9b-6819862c072d.png)
![gameplay](https://user-images.githubusercontent.com/45783867/196856327-42de01e2-f029-48a2-9e5c-6781266206a2.png)

## Instructions

FOR THREADS: 2011 C++ must be enabled. Settings > Compiler > Compiler Settings > Compiler Flags > ENABLE -std=c+11 (2nd option)

FOR SOUNDS: Windows multimedia library required. -lwinmm Add this in Settings> Compiler> Linker settings> Other linker options box

Optimal CMD Window Size: H = 35, W = 70

## Implementation details
The project is an ASCII text-based C++ 2D Console version of Pacman, where the map of the level is stored as a 2D character array 'level' with specific dimensions. 2 threads (pac and ghost1) are used for the parallel processing of the Pacman and the Ghost, with their required variables
- We use 2 similar boolean type functions (ghost_moveAllowed() and pac_moveAllowed()) for the interactions of the Pacman and Ghost with the next character to be checked to move onto.
- In the main Pacman thread, input is taken using getch() for movement (4D - w, a, s, d) within a kbhit() if selection, to only take it when a key press is detected. 
Whenever the ghost collides with a wall, we call changeGhostDirection() that acts as a basic pathfinding decision maker by finding the best direction to change to, to get closer to the Pacman using horizontal and vertical distances from it. 
- resetCursor() is used before printing to reset the position of the active screen buffer cursor to origin (0,0),so that the previous map display is overwritten without the need to clear the screen every time. 
- In printbox(), putchar() is used for printing the level array within dual for loops. Also, here SetConsoleColor() is used that sets the screen buffer's current color attribute to the specified color value, we use this for color coding the array characters by using a switch.
- titleScreen() displays the initial game menu allowing the user to start or quit the game.
