#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
using namespace std;

bool endGame;

//set up the constant values of the game
const int screenWidth = 40;
const int screenHeight = 30;
int headPosX, headPosY;
int fruitPosX, fruitPosY, score;
enum snakeDir {STOP, LEFT, RIGHT, UP, DOWN};
snakeDir status;

void setup()
{
    endGame = false;
    score = 0;
    status = STOP;
    //make the snake head appear in the middle of the screen
    headPosX = screenWidth / 2;
    headPosY = screenHeight / 2;
    //randomize the fruit spawn, x pos in the range from 0 to max screen width,
    //y pos in the range from 0 to max screen height
    srand(time(0));
    fruitPosX = rand() % screenWidth;
    fruitPosY = rand() % screenHeight;

}

void clearGameScreen()
{
    //reduces flickering
    COORD cursorPos;
    cursorPos.X = 0;
    cursorPos.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //removes flickering console cursor
    CONSOLE_CURSOR_INFO CursorInfo;
    CursorInfo.dwSize = 1;
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &CursorInfo);
}

void draw()
{
    //clear the screen
    clearGameScreen();
    //create simple matrix or screen for the snake
    for(int i = 0; i < screenHeight; i++)
    {
        if(i == 0 || i == screenHeight - 1)
        {
            for(int x = 0; x < screenWidth; x++)
                cout << "-";
        }

        else
        {
            for(int j = 0; j < screenWidth; j++)
            {
                if(j == 0 || j == screenWidth - 1)
                    cout << "|";

                //prints the head of the snake at start
                else if(i == headPosY && j == headPosX)
                    cout << "O";

                else if(i == fruitPosY && j == fruitPosX)
                    cout << "X";

                else
                    cout << " ";
            }
        }
        cout << endl;
    }
}

void input()
{
    //checks if the keyboard is hit (returns boolean),
    //returns positive value if true and 0 if false
    if(_kbhit())
    {
        //set WASD for snakeDir
        switch(_getch())
        {
            case 'a':
                status = LEFT;
                break;

            case 's':
                status = DOWN;
                break;

            case 'w':
                status = UP;
                break;

            case 'd':
                status = RIGHT;
                break;

            case 'q' :
                endGame = true;
                break;
        }
    }
}



//when snake hits a wall, gameOver = true
//when snake eats a fruit, snake gets bigger
//the snake direction will change the x and y coordinate
void logic()
{
    if(status == RIGHT)
        headPosX++;

    else if(status == LEFT)
        headPosX--;

    else if(status == DOWN)
        headPosY++;

    else if(status == UP)
        headPosY--;

    else
        status = STOP;

    //end the game when the snake head hits the wall
    if(headPosX == screenWidth || headPosY == screenHeight || headPosX == 0 || headPosY == 0)
        endGame = true;

    if(headPosX == fruitPosX && headPosY == fruitPosY)
    {
        score++;
        for(int i = 0; i < headPosY + 1; i++)
        {
            for(int j = 0; j <= headPosX + 1; j++)
            {
                if(i == headPosY + 1 && j == headPosX + 1)
                {
                    cout << "O";
                }
            }
        }
        fruitPosX = rand() % screenWidth;
        fruitPosY = rand() % screenHeight;
    }

}


int main()
{
    setup();
    while(endGame != true)
    {
        draw();
        input();
        logic();
    }

    cout << endl << "You died! The score for this round is: " << score;

    //add continue and quit

    return 0;
}


