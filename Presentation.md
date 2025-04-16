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

---

## Local Setup Instructions

### Prerequisites

- **Windows OS** (Game uses Windows console-specific functions like `SetConsoleCursorPosition`)
- **C++ Compiler** (e.g., MinGW/g++, MSVC)

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
     g++ pacman.cpp -o pacman.exe
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

## ⚙️ Basic Working

The game begins in a maze filled with pellets. The player navigates PacMan using keyboard input. The goal is to eat all pellets while avoiding moving ghosts. The game updates in a loop, refreshing the screen and checking for user input, movement logic, collisions, and score changes.

### Controls

- **W**: Move Up  
- **A**: Move Left  
- **S**: Move Down  
- **D**: Move Right  
- **P**: Pause  
- **Q**: Quit  

---

## 🧠 Data Structures & Algorithms

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
   - Moves if it’s not a wall.

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

## How Things Are Modeled

### Game Entities

- **PacMan**: Has direction, position, and pellet-eating logic.
- **Ghosts**: Independent AI-driven agents.
- **Maze**: Grid-based layout stored in memory.
- **Pellets**: Static items; disappear on contact with PacMan.

### Game Logic

- **Movement**: Position updates based on keyboard input; validated against walls.
- **Collision Detection**: Checks if PacMan’s coordinates match any ghost's—ends game on match.
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

> *(Add gameplay screenshots here by running the game and taking console captures)*

---

## License

This project is open-source and free to use under the terms of its original repository. Feel free to fork, enhance, or contribute!

---

## Authors

- **Zoraiz Qureshi**
- **Zainab Aamir**
