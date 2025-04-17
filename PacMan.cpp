#include <iostream>
#include<conio.h>
#include<windows.h>
#include <thread>

using namespace std ;

struct LevelData;  
void pacmanThread(LevelData LD);
void ghost1Thread(LevelData LD);
void ghost2Thread(LevelData LD);
void ghost3Thread(LevelData LD);
void ghost4Thread(LevelData LD);


/*
Optimal CMD Window Size: h = 32, w = 70

(Rows = y, Column = x)

Directions
Up = Y--
Down = Y++
Left = X--
Right X++
*/



void resetCursor(){
    //(windows.h)
    HANDLE hOut; //Declares a handle with the identifier hOut.
    //COORD - Defines the coordinates of a character cell in a console screen buffer.
    COORD Position; //COORD structure declared 'Position'.
    
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    Position.X = 0; //The horizontal coordinate or column value = 0
    Position.Y = 0; // The vertical coordinate or row value = 0
    SetConsoleCursorPosition(hOut, Position); //Sets cursor position to origin (0,0), before the map is printed again, so that the previous screen is over
}

void hideCursor(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cInfo;
    
    GetConsoleCursorInfo(hOut, &cInfo); //get cursor info for current output handle
    cInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(hOut, &cInfo); //set cursor info
}

void setConsoleColour(int colour){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush(); //To ensure that the active screen buffer is cleared and the characters are written to their destination.
    SetConsoleTextAttribute(hOut, colour);
    //Sets the console text color attribute to specified 'colour' value.
}

///'DEFINING' a structure 'LevelData' of all the pointers necessary to store addresses for our local variables in main.

struct LevelData {
    int *pY, *pX, *score, *lives, *time, *SMtime, *dotsEaten, *maxDots;
    int *eY1, *eX1, *eY2, *eX2, *eY3, *eX3, *eY4, *eX4;
    bool *SM;
    char *_level[21][28];
    string *message;
};

DWORD WINAPI pacmanThreadWrapper(LPVOID lpParam) {
    LevelData* LD = (LevelData*)lpParam;
    pacmanThread(*LD);
    return 0;
}

DWORD WINAPI ghost1ThreadWrapper(LPVOID lpParam) {
    LevelData* LD = (LevelData*)lpParam;
    ghost1Thread(*LD);
    return 0;
}

DWORD WINAPI ghost2ThreadWrapper(LPVOID lpParam) {
    LevelData* LD = (LevelData*)lpParam;
    ghost2Thread(*LD);
    return 0;
}

DWORD WINAPI ghost3ThreadWrapper(LPVOID lpParam) {
    LevelData* LD = (LevelData*)lpParam;
    ghost3Thread(*LD);
    return 0;
}

DWORD WINAPI ghost4ThreadWrapper(LPVOID lpParam) {
    LevelData* LD = (LevelData*)lpParam;
    ghost4Thread(*LD);
    return 0;
}

int titleScreen(){// Displays title screen.
    string gameName = "PACMAN C++";
    setConsoleColour(7);
    cout << "Zoraiz Qureshi         Zainab Aamir" << endl << endl;
    setConsoleColour(9);
    cout << "       ###############              \n      ##################        \n    #######################         \n  ##############   ###########                  \n##############################                  \n#######################                 \n####################            \n################            \n#############               \n###########                 \n#########                       \n############            ";
    setConsoleColour(14);
    cout<< gameName << endl;
    setConsoleColour(9);
    cout << "##################          \n#######################         \n###########################                 \n##############################                  \n    #######################                 \n      ####################              \n        ################            "<< endl;;
    setConsoleColour(14);
    cout<< "     _____              _____     __  __                _   _ \n    |  __ \\    /\\      / ____|   |  \\/  |      /\\      | \\ | |\n    | |__) |  /  \\     | |       | \\  / |     /  \\     |  \\| |\n    |  ___/  / /\\ \\    | |       | |\\/| |    / /\\ \\    |     |\n    | |     / ____ \\   | |___    | |  | |   / ____ \\   | |\\  |  \n    |_|    /_/   \\__\\  \\____|    |_|  |_|  /_/    \\_\\  |_| \\_|"<<endl;
    cout << "\nPress s to start, q to quit." << endl;

    char inp;
    inp = getch();
    cout << " ";
    switch(inp){
    case 's':
        cout << "New game starting...\nSelect level (1 or 2): ";
        int lvl;
        cin >> lvl;
        return lvl;
        break;
    case 'q':
        exit(0);
        break;
    }
}

bool pac_moveallowed (char c, LevelData LD){ //Next character in specific direction of pacman.
    switch(c)
        {
        case 'M': case 'W': case 'Y': case 'U': //... //Ghost
            if(!*LD.SM) { ///Super mode is not active, then the PAC is eaten by Ghost, and its position is reseted to initial. Lives decremented.
            *LD.message = "You were eaten by a ghost! You lost a life. :(";
            (*LD.lives)--;
            *(LD._level[*LD.pY][*LD.pX]) = ' ';
            *LD.pY = 15; *LD.pX = 13;
            *(LD._level[*LD.pY][*LD.pX]) = '<';
            }
            else{ /// Super mode is active, PAC eats the Ghost, 100 score added! Ghost position reseted to initial.
            *LD.message = "You ate a ghost! +100 SCORE!";
            (*LD.score) += 100;
            switch(c){
            case 'M':
            *(LD._level[*LD.eY1][*LD.eX1]) = ' ';
             *LD.eY1 = 9; *LD.eX1 = 12;
            *(LD._level[*LD.eY1][*LD.eX1]) = 'M';
            break;
            case 'W':
            *(LD._level[*LD.eY2][*LD.eX2]) = ' ';
             *LD.eY2 = 9; *LD.eX2 = 14;
            *(LD._level[*LD.eY2][*LD.eX2]) = 'W';
            break;
            case 'Y':
            *(LD._level[*LD.eY3][*LD.eX3]) = ' ';
             *LD.eY3 = 10; *LD.eX3 = 12;
            *(LD._level[*LD.eY3][*LD.eX3]) = 'Y';
            break;
            case 'U':
            *(LD._level[*LD.eY4][*LD.eX4]) = ' ';
             *LD.eY4 = 10; *LD.eX4 = 14;
            *(LD._level[*LD.eY4][*LD.eX4]) = 'U';
            break;
            }

            }
            return false;
            break;
        case 'O': //Super Pellet
            (*LD.SM) = true;
            *LD.message = "Super mode is now active!";
            *LD.SMtime = *LD.time;
            return true;
            break;
        case '.': //Dots
             (*LD.dotsEaten)++;
             (*LD.score)++; //Each worth 1 score.
            return true ;
            break;
        case '#': //Wall
            return false ;
            break;
        case '[':  //Portal Left
            *(LD.pX) = 26; //Teleport pacman in front of right portal.
            return true;
            break;
        case ']': //Portal Right
            *(LD.pX) = 0; //Teleport pacman in front of left portal.
            return true;
            break;
        }
//This function returns true if the Pacman is allowed to move onto the next character / replace its position.
}

void pacmanThread(LevelData LD){
    int * _pY = LD.pY, *_pX = LD.pX;
    char pac = '<'; //Character for the Pacman. (Initially '<')
    char mov, nc; //Local char mov used for 4D motion input (w,a,s,d) of the Pacman.
    //Local char nc is used to get the next character to be checked to move onto.

    while((*LD.lives > 0) && (*LD.dotsEaten < *LD.maxDots)){
           resetCursor();
           (*LD.time)++;

           /// The time interval for the super mode to remain active  = 40 PAC Steps
           if (*LD.time -  *LD.SMtime >= 40){
           *LD.message = "Super mode is now over.";
           (*LD.SM) = false; ///Super mode automatically deactivated if this time interval is exceeded.
           }

           *(LD._level[*_pY][*_pX])= ' '; //Space occupied by the Pacman (either '.' or 'o') is cleared to ' '
           switch(pac){
            case 'v': //UP
               nc = *(LD._level[*_pY-1][*_pX]);
               if (pac_moveallowed(nc, LD)) //if true (movement onto that character is allowed), then move one unit as so.
               (*_pY)--;
            break;
            case '^': //DOWN
               nc = *(LD._level[*_pY+1][*_pX]);
               if (pac_moveallowed(nc, LD))
               (*_pY)++;
            break;
            case '<': //RIGHT
                   nc = *(LD._level[*_pY][*_pX+1]);
               if (pac_moveallowed(nc, LD))
               (*_pX)++;
            break;
            case '>': //LEFT
                 nc = *(LD._level[*_pY][*_pX-1]);
               if (pac_moveallowed(nc, LD))
               (*_pX)--;
            break;
           }

           *(LD._level[*_pY][*_pX])= pac;

            //Print level array with color coding.
            setConsoleColour(12);
            cout<<"    Score: "<<*LD.score;
            setConsoleColour(14);
            cout<<"  Lives: "<<*LD.lives<<" " << endl;

            ///PRINT LEVEL WITH COLOR CODING
            for( int y = 0 ; y<21; y++){
            for(int x = 0 ; x<27; x++){
               switch(*(LD._level[y][x])){ // Color coding every character using setConsoleColour()
               case '#': case ']': case '[': setConsoleColour(9); break;
               case '<': case '^': case '>': case 'v': setConsoleColour(14); break;
               case 'M':
               if (!*LD.SM) setConsoleColour(12);
               else setConsoleColour(7);
               break;
               case 'Y':
               if (!*LD.SM) setConsoleColour(10);
               else setConsoleColour(7);
               break;
               case 'W':
               if (!*LD.SM) setConsoleColour(13);
               else setConsoleColour(7);
               break;
               case 'U':
               if (!*LD.SM) setConsoleColour(11);
               else setConsoleColour(7);
               break;
               case '.': setConsoleColour(5); break;
               case 'O':setConsoleColour(15);break;
               }
               putch(*(LD._level[y][x])); //Print the current character of level array. (Faster than cout)
            }
            putch('\n');
            }
            setConsoleColour(6);
            cout<<"[GAME] "<<*LD.message<< "                          " << endl; ///GLOBAL MESSAGE PRINTED FOR USER INFO

           if(kbhit()){
           mov=getch(); // If keyboard button press detected, kbhit() returns true, if this is true, then input is taken using getch() and stored in mov.
           switch(mov){ //Changes pacman's character depending on key pressed.
                case 'w': pac = 'v'; break;
                case 'a': pac = '>'; break;
                case 's': pac = '^'; break;
                case 'd': pac = '<'; break;
           }}

           Sleep(150);
        }
}

bool ghost_moveallowed (char e, int * _eX, LevelData LD){ //Next character in specific direction of ghost.
    switch(e)
        {
        case 'v': case '>':case '<':case '^': //Pacman
            if(!*LD.SM){
            *LD.message = "You were eaten by a ghost! You lost a life. :(";
            (*LD.lives)--;
            *(LD._level[*LD.pY][*LD.pX]) = ' ';
            *LD.pY = 15;  *LD.pX = 13;
            *(LD._level[*LD.pY][*LD.pX]) = '<';
            }
            return false;
            break;
        case 'O': return true; break;
        case '.': return true ; break;
        case '#': case 'M': case 'W': case 'Y': case 'U': return false; break;
        case '[': *_eX = 25; return false; break;
        case ']': *_eX = 1; return false; break;
        }
     //This function returns true if the ghost is allowed to move onto the next character / replace its position.
}

void changeGhostDirection (int& d , int disy , int disx, int * _eY, int * _eX, LevelData LD){
    const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;
    int currentd = d;
    char cUp , cDown, cLeft , cRight ;
    cUp = *(LD._level[*_eY-1][*_eX]); //Character upwards of ghost.
    cDown = *(LD._level[*_eY+1][*_eX]); // down
    cRight = *(LD._level[*_eY][*_eX+1]); // right
    cLeft = *(LD._level[*_eY][*_eX-1]); // left

    ///Do not allow reversal of direction, or selection of direction that points to a wall directly, and calculate direction depending on vertical
    ///and horizontal distances from the Pacman.
    if (d != DOWN && ghost_moveallowed (cUp, _eX, LD) && disy>0) d = UP ;
    else if (d != RIGHT && ghost_moveallowed (cLeft, _eX, LD) &&  disx>0) d = LEFT ;
    else if (d != UP && ghost_moveallowed (cDown, _eX, LD) && disy<0 ) d = DOWN ;
    else if (d != LEFT && ghost_moveallowed (cRight, _eX, LD) &&  disx<0) d = RIGHT;

    if (d == currentd) d = (rand() % 4) + 1; ///If stuck in junction, select random direction (shortest or longest route randomly).
 }

void ghostMotion(char ghost,int & dir,  int * _eY, int * _eX, int targetX, int targetY,  LevelData LD, char & nc, char & cb){
       const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;

       int dY = *_eY - targetY; //'Vertical' distance between y coordinates of Target and Ghost (Enemy)
       int dX = *_eX - targetX; //'Horizontal' distance between x coordinates of Target and Ghost (Enemy)

       *(LD._level[*_eY][*_eX])= cb; //Replace the previous character that was below the Ghost, back in its position, as Ghost moves.
       switch(dir){
       case UP:
            nc = *(LD._level[*_eY-1][*_eX]);
           if (ghost_moveallowed(nc, _eX, LD))
            {cb = nc;//Update the value of character below to the n*_eXt character that the Ghost is moving onto.
           (*_eY)--;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX, LD); //If false (wall present), change direction.
        break;
        case DOWN:
            nc = *(LD._level[*_eY+1][*_eX]);
           if (ghost_moveallowed(nc, _eX, LD)){
           cb = nc;
           (*_eY)++;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX, LD);
        break;
        case RIGHT:
            nc = *(LD._level[*_eY][*_eX+1]);
           if (ghost_moveallowed(nc, _eX, LD)){
            cb = nc;       char nc , cb; //cb, literally 'character below'.

            (*_eX)++;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX,  LD);
        break;
        case LEFT:
            nc = *(LD._level[*_eY][*_eX-1]);
            if (ghost_moveallowed(nc, _eX, LD)){
            cb = nc;
            (*_eX)--;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX,  LD);
        break;
       }
       *(LD._level[*_eY][*_eX]) = ghost; //*_eY, *_eX are changed every time, Ghost's character printed on the new position.
    }

void ghost1Thread(LevelData LD){
    const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;
    char nc , cb; //cb, literally 'character below'.
    int targetY, targetX;
    int dir = UP;
    char ghost = 'M'; //Character for Ghost 1.

    while((*LD.lives > 0) && (*LD.dotsEaten < *LD.maxDots)){
    Sleep(250); //Delay for 0.25s
           /// Determines the state of the ghost (different targets) depending on whether the super mode is activated or not.
           if (!*LD.SM){
           ///Chase state
           targetY = *LD.pY; //Target = Pacman
           targetX = *LD.pX;
           }
           else{
           ///Flee state
           targetY = 15 - *LD.pY; //Target away from Pacman
           targetX = 15 - *LD.pX;
           }
           ghostMotion(ghost, dir, LD.eY1, LD.eX1, targetX, targetY, LD, nc, cb);
    }
}

void ghost2Thread(LevelData LD){
    const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;
    char nc , cb; //cb, literally 'character below'.
    int targetY, targetX;
    int dir = DOWN;
    char ghost = 'W'; //Character for Ghost 1.

    while((*LD.lives > 0) && (*LD.dotsEaten < *LD.maxDots)){
    Sleep(250); //Delay for 0.25s
           /// Determines the state of the ghost (different targets) depending on whether the super mode is activated or not.
           if (!*LD.SM){
           ///Chase state
           targetY = *LD.pY+2; //Target = Pacman
           targetX = *LD.pX+2;
           }
           else{
           ///Flee state
           targetY = 15 - *LD.pY; //Target away from Pacman
           targetX = 15 - *LD.pX;
           }
           ghostMotion(ghost, dir, LD.eY2, LD.eX2, targetX, targetY, LD, nc, cb);
    }
}

void ghost3Thread(LevelData LD){
    const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;
    char nc , cb; //cb, literally 'character below'.
    int targetY, targetX;
    int dir = RIGHT;
    char ghost = 'Y'; //Character for Ghost 1.

    while((*LD.lives > 0) && (*LD.dotsEaten < *LD.maxDots)){
    Sleep(450); //Delay for 0.25s
           /// Determines the state of the ghost (different targets) depending on whether the super mode is activated or not.
           if (!*LD.SM){
           ///Chase state
           targetY = *LD.pY; //Target = Pacman
           targetX = *LD.pX;
           }
           else{
           ///Flee state
           targetY = 15 - *LD.pY; //Target away from Pacman
           targetX = 15 - *LD.pX;
           }
           ghostMotion(ghost, dir, LD.eY3, LD.eX3, targetX, targetY, LD, nc, cb);
    }
}

void ghost4Thread(LevelData LD){
    const int UP = 1 ,  DOWN = 2,  RIGHT = 3,  LEFT = 4;
    char nc , cb; //cb, literally 'character below'.
    int targetY, targetX;
    int dir = LEFT;
    char ghost = 'U'; //Character for Ghost 1.

    while((*LD.lives > 0) && (*LD.dotsEaten < *LD.maxDots)){
    Sleep(150); //Delay for 0.25s
           /// Determines the state of the ghost (different targets) depending on whether the super mode is activated or not.
           if (!*LD.SM){
           ///Chase state
           targetY = *LD.pY; //Target = Pacman
           targetX = *LD.pX;
           }
           else{
           ///Flee state
           targetY = 15 - *LD.pY; //Target away from Pacman
           targetX = 15 - *LD.pX;
           }
           ghostMotion(ghost, dir, LD.eY4, LD.eX4, targetX, targetY, LD, nc, cb);
    }
}

int main(){
    hideCursor(); ///Disable console cursor
    char map1[21][28] = { //Level 1 Map - 21 rows, 28 columns (1 used for '/0' terminating character)
    "###########################",
    "#............#............#",
    "#O####.#####.#.####.#####O#",
    "#.........................#",
    "#.####.#.#########.#.####.#",
    "#......#.....#.....#......#",
    "######.#####.#.#####.######",
    "     #.#           #.#     ",
    "######.#           #.######",
    "[     .             .     ]",
    "######.#           #.######",
    "     #.#           #.#     ",
    "######.#..#######..#.######",
    "#............#............#",
    "#.####.#####.#.#####.####.#",
    "#O...#.......<.......#...O#",
    "####.#.#.#########.#.#.####",
    "#......#.....#.....#......#",
    "#.####.#####.#.#####.####.#",
    "#.........................#",
    "###########################"
    };
    char map2[21][28] = { //Level 2 Map - 21 rows, 28 columns (1 used for '/0' terminating character)
    "###########################",
    "#O..........###..........O#",
    "#....#################....#",
    "#.........................#",
    "#.######.#########.######.#",
    "[......#.....#.....#......]",
    "######....#######....######",
    "#O.....#           #.....O#",
    "######.#           #.######",
    "[......             ......]",
    "######.#           #.######",
    "#O.....#           #.....O#",
    "######.#..#######..#.######",
    "[.........................]",
    "#.####.#############.####.#",
    "#............<............#",
    "######...###...###...######",
    "#....#...###...###...#....#",
    "#O.......................O#",
    "#..........#####..........#",
    "###########################"
    };

    ///Our main local variables.
    int pY, pX, score, lives, time, SMtime, dotsEaten, maxDots, eY1, eX1, eY2, eX2, eY3, eX3, eY4, eX4;

    bool SM;
    string message;
    char level[21][28];
    LevelData LD; ///'DECLARED' a local structure LD of Level Data type.

    ///Assigning addresses of these local variables to the respective pointers in the new local structure LD.
    LD.pY = &pY;
    LD.pX = &pX;
    LD.eY1 = &eY1; LD.eX1 = &eX1;
    LD.eY2 = &eY2; LD.eX2 = &eX2;
    LD.eY3 = &eY3; LD.eX3 = &eX3;
    LD.eY4 = &eY4; LD.eX4 = &eX4;
    LD.score = &score;
    LD.dotsEaten = &dotsEaten;
    LD.maxDots = &maxDots;
    LD.lives = &lives;
    LD.time = &time;
    LD.SMtime = &SMtime;
    LD.SM = &SM;
    LD.message = &message;

    ///Assigning addresses for successive characters of the 2D level character array to the successive pointers in the 2D pointer array of LD.
    for( int y = 0 ; y<21; y++){
    for(int x = 0 ; x<28; x++){
        LD._level[y][x] = &(level[y][x]);
        }
    }

    char in = 'r';
    while(in == 'r'){ ///While the user keeps entering 'r' for retry at the end of each round, game loop does not stop.
    int lvlsel = titleScreen(); ///Level selected by user from titleScreen is stored here.
    system("cls"); //Clear screen.

    ///Depending on level choice of user, set initial max dots, and assign (copy) the respective map to our main local variable level.
    if (lvlsel == 1){
    for( int y = 0 ; y<21; y++){
    for(int x = 0 ; x<28; x++){
        level[y][x] = map1[y][x];
        }
    }
    maxDots = 210;
    }
    else if (lvlsel == 2){
    for( int y = 0 ; y<21; y++){
    for(int x = 0 ; x<28; x++){
        level[y][x] = map2[y][x];
        }
    }
    maxDots = 251;
    }

    pY = 15, pX = 13; //Initial position of Pacman character.
    eY1 = 9, eX1 = 12; //Initial position of Ghost1 character.
    eY2 = 9, eX2 = 14; //Initial position of Ghost2 character.
    eY3 = 10, eX3 = 12; //Initial position of Ghost3 character.
    eY4 = 10, eX4 = 14; //Initial position of Ghost4 character.
    time = 0;
    score = 0 ; // User score of current round.
    dotsEaten = 0;
    lives = 3; // Number of lives for the current round.
    SM = false; // To be implemented later.
    message = "Round start! ";

  
    HANDLE hThreads[5];  // Stores thread handles

    hThreads[0] = CreateThread(NULL, 0, pacmanThreadWrapper, &LD, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, ghost1ThreadWrapper, &LD, 0, NULL);
    hThreads[2] = CreateThread(NULL, 0, ghost2ThreadWrapper, &LD, 0, NULL);
    hThreads[3] = CreateThread(NULL, 0, ghost3ThreadWrapper, &LD, 0, NULL);
    hThreads[4] = CreateThread(NULL, 0, ghost4ThreadWrapper, &LD, 0, NULL);

    // Wait for all threads to finish (replaces .join())
    WaitForMultipleObjects(5, hThreads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < 5; i++) {
        CloseHandle(hThreads[i]);
    }

    system("cls");
    if (dotsEaten == *LD.maxDots){ //If max number of dots in the current level have been eaten.
    ///YOU WIN!
    setConsoleColour(14);
    cout<< " __     ______  _    _  __          _______ _   _ _ \n \\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | | |\n  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| | |\n   \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` | |\n    | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |_|\n    |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_(_)"<<endl;
    cout << "\nCongratulations! \nYOUR SCORE: " << score;
    setConsoleColour(2);
    cout<< "\n\nWould you like to play again?" << endl;
    }
    else{
    ///GAME OVER
    setConsoleColour(12);
    cout<<  "  _____              __  __   ______      ____  __      __  ______   ____  \n / ____|     /\\     |  \\/  | |   ___|    / __ \\ \\ \\    / / |  ____| |  __ \\ \n | |  __    /  \\    | \\  / | |  |__     | |  | | \\ \\  / /  | |__    | |__) |\n | | |_ |  / /\\ \\   | |\\/| | |  __|     | |  | |  \\ \\/ /   |  __|   |  _  / \n | |__| | / ____ \\  | |  | | |  |____   | |__| |   \\  /    | |____  | | \\ \\ \n \\_____/ /_/   \\_ \\ |_|  |_| |______|    \\____/     \\/     |______| |_|  \\_\\" <<endl;
    setConsoleColour(2);
    cout << "\nRetry?" << endl;
    }
    cout << "(Press r to play again, or any other key to exit.)" << endl;
    cin >> in;
    system("cls");
    }

    cout << "THANKS FOR PLAYING! " << endl;
    return 0;
}
