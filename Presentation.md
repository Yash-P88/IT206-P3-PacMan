# PacMan Game (C++)

## Project Overview

This is a **console-based PacMan game** implemented in **C++**. The game involves navigating a character (PacMan) through a maze, eating pellets, and avoiding ghosts. The game follows the basic mechanics of the original PacMan game, with added complexity such as multiple levels and ghosts with different behaviors.

---

### Key Features

- **Maze Navigation**: Player moves PacMan through a 2D grid while avoiding walls and ghosts.
- **Multiple Ghosts**: Each ghost has a unique movement style—some chase, others wander randomly.
- **Level Progression**: With each level, maze complexity and ghost AI difficulty increases.
- **Score Tracking**: Score increments with each pellet eaten; can include bonus scores for risky moves.
- **Game Over Mechanism**: Game ends if PacMan and any ghost collide.
- **Threaded Implementation**: Uses parallel processing for Pacman and Ghost movements.
- **Console-based Graphics**: ASCII text-based display with color coding.
- **Sound Effects**: Windows multimedia library support for game sounds.

---

## Local Setup Instructions

### Prerequisites

- **Windows OS** (Game uses Windows console-specific functions like `SetConsoleCursorPosition`)
- **C++ Compiler** (e.g., MinGW/g++, MSVC)
- **C++11 Support**: Must be enabled in compiler settings
- **Windows Multimedia Library**: Required for sound effects

### Compiler Settings

1. **For Threads**:
   - Enable C++11 in compiler settings:
     - Settings > Compiler > Compiler Settings > Compiler Flags > ENABLE -std=c++11

### Optimal CMD Window Size

- Height: 35
- Width: 70

### Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/Yash-P88/IT206-P3-PacMan.git
   ```

2. Navigate into the directory:

   ```bash
   cd IT206-P3-PacMan
   ```

3. Compile:

   - Using g++:

     ```bash
     g++ pacman.cpp -o pacman.exe -std=c++11 -lwinmm
     ```

   - Using MSVC:

     ```bash
     cl pacman.cpp
     ```

4. Run the game:

   ```bash
   pacman.exe
   ```

---

## Basic Working

The game begins in a maze filled with pellets. The player navigates PacMan using keyboard input. The goal is to eat all pellets while avoiding moving ghosts. The game updates in a loop, refreshing the screen and checking for user input, movement logic, collisions, and score changes.

### Implementation Details

- The game map is stored as a 2D character array 'level' with specific dimensions
- Two threads (pac and ghost1) are used for parallel processing of Pacman and Ghost movements
- Movement validation functions (ghost_moveAllowed() and pac_moveAllowed()) check valid moves
- Input is taken using getch() within a kbhit() check for responsive controls
- Ghost pathfinding uses changeGhostDirection() which calculates optimal direction based on Pacman's position
- Console display uses resetCursor() to overwrite previous frames without clearing screen
- printbox() renders the level array with color coding using SetConsoleColor()

### Controls

- **W**: Move Up  
- **A**: Move Left  
- **S**: Move Down  
- **D**: Move Right  
- **P**: Pause  
- **Q**: Quit  

---

## Data Structures & Algorithms

### 1. **Point Class**

- **Purpose**: Basic building block for grid-based positions.
- **Fields**: `x`, `y` coordinates.
- **Use**: Tracks positions of PacMan, ghosts, and pellets in the maze.
  
```cpp
class Point {
public:
    int x, y;
    Point(int _x = 0, int _y = 0): x(_x), y(_y) {}
};
```

> Useful for: Grid-based logic, distance calculation, movement updates.

---

### 2. **Maze Class**

- **Purpose**: Handles maze generation, layout, and movement validation.
- **Structure**: 2D array (`char grid[rows][cols]`) with different symbols:  
  - `#` for wall  
  - `.` for pellet  
  - `` for path  
- **Responsibilities**:
  - Load maze layout from file or string.
  - Check if a given move is valid (not a wall).
  - Display updated maze state.

---

### 3. **Ghost Class**

- **Purpose**: Models behavior of ghosts with varying AI.
- **Structure**:  
  - `Point position`  
  - `char displayChar`  
  - Movement function (random or strategic)

#### 🔁 Ghost Movement Algorithms

1. **Random Movement**:
   - Picks a direction (up/down/left/right) randomly.
   - Moves if it's not a wall.

2. **Chase Mode (Simple AI)**:
   - Calculates Manhattan distance to PacMan.
   - Chooses the move that reduces the distance.
   - Can be improved using **Breadth-First Search (BFS)** or **A\*** in future versions.

```cpp
int distance = abs(pacman.x - ghost.x) + abs(pacman.y - ghost.y);
```

> Tradeoff: Random AI is easy to implement but less challenging. Strategic AI improves difficulty and engagement.

---

### 4. **Game Class**

- **Purpose**: Central manager of game loop and state.
- **Responsibilities**:
  - Handle keyboard input.
  - Update game objects (PacMan, ghosts).
  - Detect collisions and update score.
  - Manage game states (start, pause, game over).

#### 🔁 Game Loop Algorithm

```cpp
while (!gameOver) {
    getInput();
    updatePositions();
    checkCollisions();
    drawScreen();
    sleep();
}
```

> Efficient and minimal design makes it responsive and easy to debug.

---

### 5. **Threaded Implementation Details**

The game uses a multi-threaded architecture for responsive gameplay:

**Core Components**:

```cpp
struct LevelData {
    // Shared game state
    int* pY, *pX; // Pacman position
    int* eY1, *eX1, *eY2, *eX2, *eY3, *eX3, *eY4, *eX4; // Ghost positions
    int* score;
    int* dotsEaten;
    int* maxDots; // 210 (Level 1) or 251 (Level 2)
    int* lives; // 3
    bool* SM; // Super Mode flag
    char* _level[21][28]; // Maze map
};
```

**Thread Management**:

```cpp
// In main():
thread pac(pacmanThread, LD);
thread ghost1(ghost1Thread, LD);
thread ghost2(ghost2Thread, LD);
thread ghost3(ghost3Thread, LD);
thread ghost4(ghost4Thread, LD);

// ... game runs ...

pac.join();
ghost1.join();
// ... etc
```

**Ghost Behaviors**:

1. **Ghost 1 (M)**:
   - Speed: 250ms delay
   - Chase: Directly follows Pacman
   - Flee: Mirrors position (15 - pacman)

2. **Ghost 2 (W)**:
   - Speed: 250ms
   - Chase: Targets Pacman+2 tiles ahead
   - Flee: Mirrors position

3. **Ghost 3 (Y)**:
   - Speed: 450ms (slow)
   - Standard chase/flee logic

4. **Ghost 4 (U)**:
   - Speed: 150ms (fast)
   - Standard chase/flee

---

## How Things Are Modeled

### Game Entities

- **PacMan**: Has direction, position, and pellet-eating logic.
- **Ghosts**: Independent AI-driven agents.
- **Maze**: Grid-based layout stored in memory.
- **Pellets**: Static items; disappear on contact with PacMan.

### Game Logic

- **Movement**: Position updates based on keyboard input; validated against walls.
- **Collision Detection**: Checks if PacMan's coordinates match any ghost's—ends game on match.
- **Scoring**: Score variable increments each time PacMan eats a pellet ('.').

---

## Future Work

- **Add Power-ups**: Energizers that allow PacMan to eat ghosts temporarily.
- **Improve Ghost AI**: Use pathfinding algorithms like A* for smarter chasing.
- **Procedural Maze Generation**: Random maze layouts for replayability.
- **Save System**: Save game state and load later.
- **Multiplayer Mode**: One player controls PacMan, the other controls ghosts.

---

## Screenshots

![image](https://github.com/user-attachments/assets/d97d9251-85ad-4690-9e7f-e1cf7d02e4de)
![image](https://github.com/user-attachments/assets/5fc2042e-3fe7-4e2a-8b5b-3277546129b0)

---

## License

This project is open-source and free to use under the terms of its original repository. Feel free to fork, enhance, or contribute!

---

## Authors

- **Zoraiz Qureshi**
- **Zainab Aamir**
