#include <iostream>
#include <conio.h>   // For _kbhit() and _getch()
#include <windows.h> // For Sleep() on Windows
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For seeding random numbers
#include <vector>
#include <fstream>   // For file handling (read/write high score)

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, ballX, ballY, score, highScore;
int tailX[100], tailY[100];
int nTail;
int level = 1;
int lives = 3;
bool paused = false;
vector<pair<int, int>> obstacles;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Function to load the high score from a file
void LoadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0; // Set to 0 if the file doesn't exist
    }
}

// Function to save the high score to a file
void SaveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

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
    lives = 3;
    paused = false;
    obstacles.clear();
    LoadHighScore(); // Load the high score when the game starts
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
    
    if (paused) {
        cout << "Game Paused. Press 'p' to resume.\n";
        return; // Don't draw the game if paused
    }
    
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

    // Display score, level, lives, and high score
    cout << "Score: " << score << " | Level: " << level << " | Lives: " << lives << " | High Score: " << highScore << endl;
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        
        // Toggle pause
        if (key == 'p') {
            paused = !paused;
        }

        // Only process movement if not paused
        if (!paused) {
            switch (key) {
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
}

void Logic() {
    // If the game is paused, skip the logic update
    if (paused) {
        return;
    }

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
    if (x >= width || x < 0 || y >= heig
