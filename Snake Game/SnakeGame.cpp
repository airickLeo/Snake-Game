#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
using namespace std;

// Sentinel that terminates the game when it becomes true
bool endGame = false;

// Set up the constants of the game
const int screenWidth = 40;
const int screenHeight = 30;
int headPosX, headPosY;
int fruitPosX, fruitPosY, score;
enum snakeDir {STOP = 0, LEFT, RIGHT, UP, DOWN};
snakeDir status;

// Set up two arrays to record the x and y position of the snake's tail
int tail_x_pos[200]; // Set the arrays to record up to 200 coordinates
int tail_y_pos[200];

// Counter for the number of tails
int num_tails;

// Function that ensures fruit position is not out of bounds
void check_pos(int &x_pos, int &y_pos)
{
    // Ensure the x and y positions are not out of bounds
    if(x_pos == 0)
        x_pos += 5;

    else if(x_pos == screenWidth - 1)
        x_pos -= 5;

    if(y_pos == 0)
        y_pos += 5;

    else if(y_pos == screenHeight - 1)
        y_pos -= 5;
}

// Function that sets up the game
void setup()
{
    score = 0;
    status = STOP;

    //make the snake head appear in the middle of the screen
    headPosX = screenWidth / 2;
    headPosY = screenHeight / 2;

    //randomize the fruit spawn, x pos in the range from 0 to max screen width - 1,
    //y pos in the range from 0 to max screen height - 1
    srand(time(0));
    fruitPosX = rand() % screenWidth;
    fruitPosY = rand() % screenHeight;

    // Check position of the fruit
    check_pos(fruitPosX, fruitPosY);
}

// Function that reduces screen flickering
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

// Function that draws the fruit and the snake
void draw()
{
    //clear the screen
    clearGameScreen();

    //create simple matrix or screen for the snake
    for(int i = 0; i < screenHeight; i++)
    {
        // if the current position is the first row or the last row of the matrix
        if(i == 0 || i == screenHeight - 1)
        {
            // Draw "-" until the end
            for(int x = 0; x < screenWidth; x++)
                cout << "-";
        }

        else
        {
            // Loop through each column of the row
            for(int j = 0; j < screenWidth; j++)
            {
                // if it is the first or last column of the row, print "|"
                if(j == 0 || j == screenWidth - 1)
                    cout << "|";

                // Prints the head of the snake at start
                else if(i == headPosY && j == headPosX)
                    cout << "O";

                // Draw the fruit
                else if(i == fruitPosY && j == fruitPosX)
                    cout << "X";

                else
                {
                    // Variable that determines if a space or a tail will be drawn
                    bool print_space = true;

                    // Loop through all the tails
                    for(int x = 0; x < num_tails; x++)
                    {
                        // Draw the tail
                        if(tail_x_pos[x] == j && tail_y_pos[x] == i)
                        {
                            cout << "O";
                            print_space = false;
                        }
                    }
                    // Print the space
                    if(print_space)
                    {
                        cout << " ";
                    }
                }
            }
        }
        cout << endl;
    }

    // Display the score
    cout << "The score is: " << score;
}

// Function that takes in user inputs
void input()
{
    //checks if the keyboard is hit (returns boolean),
    //returns positive value if true and 0 if false
    if(_kbhit())
    {
        //set w a s d for snakeDir
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

// Function that represents the logic of the game
void logic()
{
    // Loop through all tail elements and update their positions accordingly
    // Once a direction key is pressed, each tail will obtain the position of its previous tail
    for(int i = num_tails; i > 0; i--)
    {
        tail_x_pos[i] = tail_x_pos[i - 1];
        tail_y_pos[i] = tail_y_pos[i - 1];
    }

    // Update the first tail to the position of the current head position
    tail_x_pos[0] = headPosX;
    tail_y_pos[0] = headPosY;

    // Increment the snake head's x and y position according to the keys pressed
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
    if(headPosX > screenWidth || headPosY > screenHeight || headPosX < 0 || headPosY < 0)
        endGame = true;

    // Loop through all tails
    for(int i = 0; i < num_tails; i++)
    {
        // If the head hits the tail, game over
        if(tail_x_pos[i] == headPosX && tail_y_pos[i] == headPosY)
        {
            endGame = true;
        }
    }

    // If the snake reaches the fruit
    if(headPosX == fruitPosX && headPosY == fruitPosY)
    {
        // Increment the score
        score++;

        // Redraw the fruit at random position
        fruitPosX = rand() % screenWidth;
        fruitPosY = rand() % screenHeight;

        // Check position of the fruit
        check_pos(fruitPosX, fruitPosY);

        // Increment the number of tails
        num_tails++;
    }
}

int main()
{
    setup();

    // Keeps iterating until the game is over
    while(endGame != true)
    {
        draw();
        input();
        logic();

        // Pausing the game for 15 milliseconds to reduce the snake's velocity
        Sleep(30);
    }

    // Clear the output screen
    system("cls");

    // Output the total score when the game is over
    cout << endl << "Game over! The score for this round is: " << score << endl;

    return 0;
}
