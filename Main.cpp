#include <iostream>
#include<conio.h>
#include<windows.h>
#include <thread>

using namespace std ;
const int UP = 1, DOWN = 2, RIGHT = 3 , LEFT = 4 ; // Constants for direction.
/*
CMD Window Size: h = 31, w = 70
SM
Retry/GameOver/Lives
Multiple Enemies

(Rows = y, Column = x)
 x x x
y
y
y

Directions
Up = Y--
Down = Y++
Left = X--
Right X++
*/

//Level 1 Map - 2D character array.

struct LevelData{
int * pY, * pX, * score, * lives, * time, * SMsttime, *dotsEaten;
int * eY1, * eX1;
bool * SM;
char * _level[21][28];

};

void resetCursor(){
    //(windows.h)
    HANDLE hOut; //Declares a handle with the identifier hOut.
    //COORD - Defines the coordinates of a character cell in a console screen buffer.
    COORD Position; //COORD structure declared 'Position'.

    hOut = GetStdHandle(STD_OUTPUT_HANDLE); //Handle hOut now points to the active console screen buffer (standard output device).

    Position.X = 0; //The horizontal coordinate or column value = 0
    Position.Y = 0; // The vertical coordinate or row value = 0
    SetConsoleCursorPosition(hOut, Position); //Sets cursor position to origin (0,0), before the map is printed again, so that the previous screen is over
}

void setConsoleColour(int colour){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush(); //To ensure that the active screen buffer is cleared and the characters are written to their destination.
    SetConsoleTextAttribute(hOut, colour);
    //Sets the console text color attribute to specified 'colour' value.
}

int titleScreen(){// Displays title screen.
    setConsoleColour(9);
    string text = "PACMAN C++";

    cout << "       ###############              "<< endl;

    cout << "      ##################        "<< endl;

    cout << "    #######################         "<< endl;

    cout << "  ##############   ###########                  "<< endl;

    cout << "##############################                  "<< endl;

    cout << "#######################                 "<< endl;

    cout << "####################            "<< endl;

    cout << "################            "<< endl;

    cout << "#############               "<< endl;

    cout << "###########                 "<< endl;

    cout << "#########                       "<< endl;

    cout << "############            ";
    setConsoleColour(14);
    cout<<text << endl;
    setConsoleColour(9);
    cout << "##################          "<< endl;

    cout << "#######################         "<< endl;
    cout << "###########################                 "<< endl;

    cout << "##############################                  "<< endl;
    cout << "    #######################                 "<< endl;

    cout << "      ####################              "<< endl;

    cout << "        ################            "<< endl;
    setConsoleColour(14);
    cout<< "     _____               _____    __  __                _   _ "<<endl;
    cout<< "    |  __ \\    /\\      / ____|   |  \\/  |      /\\      | \\ | |"<<endl;
    cout<< "    | |__) |  /  \\     | |       | \\  / |     /  \\     |  \\| |"<<endl;
    cout<< "    |  ___/  / /\\ \\    | |       | |\\/| |    / /\\ \\    |     |"<<endl;
    cout<< "    | |     / ____ \\   | |___    | |  | |   / ____ \\   | |\\  |  "<<endl;
    cout<< "    |_|    /_/   \\__\\  \\____|    |_|  |_|  /_/    \\_\\  |_| \\_|"<<endl<<endl;
    cout << "Press s to start, q to quit." << endl;


char inp;
inp = getch();
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
        case 'M': //Ghost
            if(*(LD.SM)) {
            (*LD.score) += 100;

            //ENEMY 1
            *(LD._level[*LD.eY1][*LD.eX1]) = ' ';
            *LD.eY1 = 11;  *LD.eX1 = 13;
            *(LD._level[*LD.eY1][*LD.eX1]) = 'M';

            }
            else{
            (*LD.lives)--;
            *(LD._level[*LD.pY][*LD.pX]) = ' ';
            *LD.pY = 15;  *LD.pX = 13;
            *(LD._level[*LD.pY][*LD.pX]) = '<';
            }
            return false;
            break;
        case 'O': //Super Pellet
            (*LD.SM) = true;
            *LD.SMsttime = *LD.time;
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
int * _pY = LD.pY, *_pX = LD.pX, *_score = LD.score, *_lives = LD.lives;
bool * _SM = LD.SM;

char pac = '<'; //Character for the Pacman. (Initially '<')
char mov, nc; //Char mov used for 4D motion input (w,a,s,d) of the Pacman.
//nc is used to get the next character to be checked to move onto.

while((*_lives > 0) && (*LD.dotsEaten < 210)){
       resetCursor();
       (*LD.time)++;
       ///
       if (*LD.time -  *LD.SMsttime == 30){
       (*LD.SM) = false;
       }
       *(LD._level[*_pY][*_pX])= ' '; //Space occupied by the Pacman (either '.' or 'o') is cleared to ' '
       switch(pac)
       {case 'v': //UP
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
        setConsoleColour(14);
        cout<<"Score: "<<*_score<<" " ;
        setConsoleColour(12);
        cout<<"Lives: "<<*_lives<<" ";
        cout<<"Time: "<<*LD.time<<endl;

        ///PRINT LEVEL
        for( int y = 0 ; y<21; y++){
        for(int x = 0 ; x<27; x++){
           switch(*(LD._level[y][x])){ // Color coding every character using setConsoleColour()
           case '#': case ']': case '[': setConsoleColour(1); break;
           case '<': case '^': case '>': case 'v': setConsoleColour(14); break;
           case 'M':
           if (*_SM) setConsoleColour(9);
           else setConsoleColour(12);
           break;
           case '.': setConsoleColour(5); break;
           case 'O':setConsoleColour(13);break;
           }
           putch(*(LD._level[y][x])); //Print the current character of level array. (Faster than cout)
        } putch('\n');
        }

       if(kbhit()){
       mov=getch(); // If keyboard button press detected, kbhit() returns true, if this is true, then input is taken using getch() and stored in mov.
       switch(mov){ //Changes pacman's character depending on key pressed.
            case 'a':
            pac = '>';
            break;
            case 'w':
            pac = 'v';
            break;
            case 's':
            pac = '^';
            break;
            case 'd':
            pac = '<';
            break;
            case 't':
            exit(0);
            break;
       }
       }
       Sleep(50); //Delay for 0.15s

    }
}

bool ghost_moveallowed (char e, int * _eX, LevelData LD){ //Next character in specific direction of ghost.
    switch(e)
        {
        case 'v': case '>':case '<':case '^': //Pacman
            if(!(*LD.SM)){
            (*LD.lives)--;
            *(LD._level[*LD.pY][*LD.pX]) = ' ';
            *LD.pY = 15;  *LD.pX = 13;
            *(LD._level[*LD.pY][*LD.pX]) = '<';
            return false;
            }
            break;
        case 'O':
            return true;
            break;
        case '.':
            return true ;
            break;
        case '#':
            return false ;
            break;
           case '[':
            *_eX = 26;
            return true;
            break;
        case ']':
            *_eX = 0;
            return true;
            break;
        }
     //This function returns true if the ghost is allowed to move onto the next character / replace its position.
}

void changeGhostDirection (int& d , int disy , int disx, int * _eY, int * _eX, LevelData LD){
char cUp , cDown, cLeft , cRight ;
int currentd = d ;
cUp = *(LD._level[*_eY-1][*_eX]); //Character upwards of ghost.
cDown = *(LD._level[*_eY+1][*_eX]); // down
cRight = *(LD._level[*_eY][*_eX+1]); // right
cLeft = *(LD._level[*_eY][*_eX-1]); // left

 ///Add comments here.
 if (d != DOWN && ghost_moveallowed (cUp, _eX, LD) && disy>0) d = UP ;
 else if (d != RIGHT && ghost_moveallowed (cLeft, _eX, LD) &&  disx>0) d = LEFT ;
 else if (d != UP && ghost_moveallowed (cDown, _eX, LD) && disy<0 ) d = DOWN ;
 else if (d != LEFT && ghost_moveallowed (cRight, _eX, LD) &&  disx<0) d = RIGHT;
}

void ghost1Thread(LevelData LD){
int * _pY = LD.pY, *_pX = LD.pX, *_lives = LD.lives;
int * _eY = LD.eY1, *_eX = LD.eX1;
bool * _SM = LD.SM;

int dY,dX; //Vertical and horizontal distances from Pacman to Ghost.
int dir = UP ;
char ghost1 = 'M'; //Character for Ghost 1.
char nc , cb = ' '; //cb, literally 'character below'.

while((*_lives > 0) && (*LD.dotsEaten < 210)){
Sleep(250); //Delay for 0.25s

       ///Chase mode
       dY = *_eY - *_pY ; //'Vertical' distance between y coordinates of Pacman and Ghost (Enemy)
       dX = *_eX - *_pX ; //'Horizontal' distance between x coordinates of Pacman and Ghost (Enemy)

       *(LD._level[*_eY][*_eX])= cb; //Replace the previous character that was below the Ghost, back in its position, as Ghost moves.
       switch(dir)
       {
       case UP:
            nc = *(LD._level[*_eY-1][*_eX]);
           if (ghost_moveallowed(nc, _eX, LD))
            {cb = nc ;//Update the value of character below to the n*_eXt character that the Ghost is moving onto.
           (*_eY)--;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX, LD); //If false (wall present), change direction.
        break;
        case DOWN:
             nc = *(LD._level[*_eY+1][*_eX]);
           if (ghost_moveallowed(nc, _eX, LD))
           {cb = nc ;
           (*_eY)++;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX, LD);
        break;
        case RIGHT:
               nc = *(LD._level[*_eY][*_eX+1]);
           if (ghost_moveallowed(nc, _eX, LD))
            {          cb = nc ;
                       (*_eX)++;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX,  LD);
        break;
        case LEFT:
            nc = *(LD._level[*_eY][*_eX-1]);
           if (ghost_moveallowed(nc, _eX, LD))
            {          cb = nc ;
                       (*_eX)--;}
           else changeGhostDirection(dir,dY, dX, _eY, _eX,  LD);
        break;
       }
       *(LD._level[*_eY][*_eX]) = ghost1; //*_eY, *_eX are changed every time, Ghost's character printed on the new position.

}
}

int main(){
char map1[21][28] = { //27 rows, 29 columns (1 used for '/0' terminating character)
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
"#O...#...............#...O#",
"####.#.#.#########.#.#.####",
"#......#.....#.....#......#",
"#.####.#####.#.#####.####.#",
"#.........................#",
"###########################"
};
char map2[21][28] = { //27 rows, 29 columns (1 used for '/0' terminating character)
"###########################",
"#            #            #",
"#O#### ##### # #### #####O#",
"#                         #",
"# #### # ######### # #### #",
"#      #     #     #      #",
"###### ##### # ##### ######",
"     # #           # #     ",
"###### #           # ######",
"[                         ]",
"###### #           # ######",
"     # #           # #     ",
"###### #  #######  # ######",
"#            #            #",
"# #### ##### # ##### #### #",
"#O   #               #   O#",
"#### # # ######### # # ####",
"#      #     #     #      #",
"# #### ##### # ##### #### #",
"#                         #",
"###########################"
};

int pY, pX, eY1, eX1, score, lives, time, SMsttime, dotsEaten;
bool SM;
char level[21][28];
LevelData LD;

LD.pY = &pY;
LD.pX = &pX;
LD.eY1 = &eY1;
LD.eX1 = &eX1;
LD.score = &score;
LD.dotsEaten = &dotsEaten;
LD.lives = &lives;
LD.time = &time;
LD.SMsttime = &SMsttime;
LD.SM = &SM;

for( int y = 0 ; y<21; y++){
for(int x = 0 ; x<28; x++){
    LD._level[y][x] = &(level[y][x]);
    }
}

char in = 'r';
while(in == 'r'){
int lvlsel = titleScreen();
system("cls"); //Clear screen.

if (lvlsel == 1){
for( int y = 0 ; y<21; y++){
for(int x = 0 ; x<28; x++){
    level[y][x] = map1[y][x];
    }
}
}
else if (lvlsel == 2){
for( int y = 0 ; y<21; y++){
for(int x = 0 ; x<28; x++){
    level[y][x] = map2[y][x];
    }
}
}

pY = 15, pX = 13; //Initial position of Pacman character.
eY1 = 11, eX1 = 13; //Initial position of Ghost1 character.
time = 0;
score = 0 ; // User score of current round.
dotsEaten = 0;
lives = 3; // Number of lives for the current round.
SM = false; // To be implemented later.


thread pac (pacmanThread, LD);
thread ghost1 (ghost1Thread, LD);
//pac and ghost1 will run concurrently.

pac.join(); //Pause until pac thread ends.
ghost1.join();

setConsoleColour(14);
cout << "GAME OVER. \nRetry? (Press r, or any other key to exit.)" << endl;
cin >> in;
system("cls");
}

cout << "Exiting game... " << endl;
return 0;
}
