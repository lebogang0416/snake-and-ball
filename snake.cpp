#include <iostream>
#include <conio.h>   // For _kbhit() and _getch()
#include <windows.h> // For Sleep() on Windows
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For seeding random numbers
#include <vector>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, ballX, ballY, score;
int tailX[100], tailY[100];
int nTail;
int level = 1;
int lives = 3;
vector<pair<int, int>> obstacles;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    ballX = rand() % width;
    ballY = rand() % height;
    score = 0;
    nTail = 0;
    level = 1;
    obstacles.clear();
    lives = 3; // Player starts with 3 lives
}

void ResetPosition() {
    // Reset snake's position after losing a life
    x = width / 2;
    y = height / 2;
    nTail = 0;
    dir = STOP; // Snake stops after losing a life
}

void Draw() {
    system("cls"); // Clears the screen
    
    // Top wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Game area
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; // Left wall

            // Draw snake head, ball, or empty space
            if (i == y && j == x)
                cout << "O"; // Snake head
            else if (i == ballY && j == ballX)
                cout << "B"; // Ball
            else {
                bool print = false;

                // Check for snake tail
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Snake tail
                        print = true;
                    }
                }

                // Check for obstacles
                for (auto &obstacle : obstacles) {
                    if (obstacle.first == j && obstacle.second == i) {
                        cout << "#"; // Obstacle
                        print = true;
                    }
                }

                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#"; // Right wall
        }
        cout << endl;
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Display score, level, and lives
    cout << "Score: " << score << " | Level: " << level << " | Lives: " << lives << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    // Save the previous position of the head
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    // Move the tail
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the snake head
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // Check for collision with walls
    if (x >= width || x < 0 || y >= height || y < 0) {
        lives--; // Lose a life
        if (lives > 0) {
            ResetPosition();
        } else {
            gameOver = true;
        }
    }

    // If snake eats the ball
    if (x == ballX && y == ballY) {
        score += 10;
        ballX = rand() % width;
        ballY = rand() % height;
        nTail++; // Increase the length of the tail

        // Level-up every 50 points
        if (score % 50 == 0) {
            level++;
            // Add obstacles for each new level
            for (int i = 0; i < level; i++) {
                int obsX = rand() % width;
                int obsY = rand() % height;
                obstacles.push_back(make_pair(obsX, obsY));
            }
        }
    }

    // Check if snake hits itself
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            lives--; // Lose a life
            if (lives > 0) {
                ResetPosition();
            } else {
                gameOver = true;
            }
        }
    }

    // Check if snake hits an obstacle
    for (auto &obstacle : obstacles) {
        if (obstacle.first == x && obstacle.second == y) {
            lives--; // Lose a life
            if (lives > 0) {
                ResetPosition();
            } else {
                gameOver = true;
            }
        }
    }
}

int main() {
    srand(time(0)); // Seed the random number generator
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        // Speed increases with level
        Sleep(200 - (level * 20)); 
    }
    return 0;
}
