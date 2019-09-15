#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif

#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <fstream>
#include "lib/game.h"

using namespace std;

/*------------------------------------*
 *         WINDOW OBJECT              *
 *------------------------------------*/

// Constructors
Window::Window() {
    this->WIN_WIDTH = 640;
    this->WIN_HEIGHT = 480;
            
    SDL_Init(SDL_INIT_VIDEO); 
}

Window::Window(int WIN_WIDTH, int WIN_HEIGHT) {
    this->WIN_WIDTH = WIN_WIDTH;
    this->WIN_HEIGHT = WIN_HEIGHT;
}

// Member Functions
void Window::SetWindowSize(int WIN_WIDTH, int WIN_HEIGHT) {
    this->WIN_WIDTH = WIN_WIDTH;
    this->WIN_HEIGHT = WIN_HEIGHT;
}

/* Setup-create the Window and renderer */
void Window::CreateWindow() {
    // create the window for the game.
    window = SDL_CreateWindow(
               "Snake Game",
               SDL_WINDOWPOS_UNDEFINED,
               SDL_WINDOWPOS_UNDEFINED,
               WIN_WIDTH,
               WIN_HEIGHT,
               SDL_WINDOW_OPENGL);
    
    // create renderer to draw graphics on the window.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Throw an error on the failure of window creation.
    if (window == NULL) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Throw an error, if renderer is not created.
    if (renderer == NULL) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return;
    }
}

void Window::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}



/*------------------------------------*
 *         POSITION OBJECT            *
 *------------------------------------*/

// Get X-Coordinate
int Position::getXPos() {
    return x;
}

// Get Y-Coordinate
int Position::getYPos() {
    return y;
}

// Set x and y coordinates of the objects.
void Position::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}



/*------------------------------------*
 *         SNAKE OBJECT               *
 *------------------------------------*/

/* Initialize properties of snake */
Snake::Snake() {
    length = 0;
    dimension = 30;
    
    velocity = 'x';
    veloSign = 'p';
}

// ** Getter & Setter for snake property.
int Snake::getLength() {
    return this->length;
}

// Set the length of snake
void Snake::setLength(int length) {
    this->length = length;
}

// Get dimension (aka width) of snake
int Snake::getDimension() {
    return this->dimension;
}

// Set the position of snake on window
void Snake::setPosition(int x, int y) {
    CurrPosition.setPosition(x, y);
}

// Get X-coord of snake's position
int Snake::getXPos() {
    return CurrPosition.getXPos();
}

// Get Y-coord of snake's position
int Snake::getYPos() {
    return CurrPosition.getYPos();
}

/*| Set the direction of snake to move
  
  | Velocity:
  | --------------------------------
  | X -or- Y coordinate
  | --
  | 'x' : left or right direction
  | 'y' : upward or downward direction
  |
  |
  
  | Velocity Sign:
  | -----------------------------------
  | Positive -or- Negative
  | --
  | 'p' : right or downward direction
  | 'n' : left or upward direction
  |
  |*/
void Snake::setVelocity(char velocity, char veloSign) {
    this->velocity = velocity;
    this->veloSign = veloSign;    
}

// Fetch current direction of moving snake.
char Snake::getVelocity() {
    return velocity;
}

char Snake::getVeloSign() {
    return veloSign;
}

// resets snake's state - useful when snake crashes
void Snake::reset() {
    length = 0;
    dimension = 30;
    
    velocity = 'x';
    veloSign = 'p';
}

// Move the snake
void Snake::move() {
    int distance = 5;

    // Move in right direction (--->)
    if ((velocity == 'x') && (veloSign == 'p')) {
        setPosition(getXPos() + distance, getYPos());
    }
    // Move in left direction (<---)
    else if ((velocity == 'x') && (veloSign == 'n')) {
        setPosition(getXPos() - distance, getYPos());
    }
    // Move in downward direction (v)
    else if ((velocity == 'y') && (veloSign == 'p')) {
        setPosition(getXPos(), getYPos() + distance);
    }
    // Move in upward direction (^)
    else if ((velocity == 'y') && (veloSign == 'n')) {
        setPosition(getXPos(), getYPos() - distance);
    }
}

bool Snake::eat(SDL_Rect *snake, SDL_Rect *food) {
    int leftSnake, rightSnake,
        topSnake, bottomSnake,
        leftFood, rightFood,
        topFood, bottomFood;

    // Set four points of the Snake rectangle
    leftSnake = snake->x;
    rightSnake = snake->x + snake->w;
    topSnake = snake->y;
    bottomSnake = snake->y + snake->h;

    // Set four points of Food rectangle
    leftFood = food->x;
    rightFood = food->x + food->w;
    topFood = food->y;
    bottomFood = food->y + food->h;

    /* Check Collision between food and Snake */
    if (bottomSnake <= topFood) {
        return false;
    }

    if (topSnake >= bottomFood ) {
        return false;
    }

    if (rightSnake <= leftFood ) {
        return false;
    }

    if (leftSnake >= rightFood) {
        return false;
    }

    return true;
}



/*------------------------------------*
 *          FOOD OBJECT               *
 *------------------------------------*/

void Food::showFood() {
    srand((int)time(0));

    int x = rand() % 1130 + 6,
        y = rand() % 700 + 80;

    CurrPosition.setPosition(x, y);
}

int Food::getDimension() {
    return this->dimension;
}

int Food::getXPos() {
    return CurrPosition.getXPos();
}

int Food::getYPos() {
    return CurrPosition.getYPos();
}

void Food::setPosition(int x, int y) {
    CurrPosition.setPosition(x, y);
}



/*------------------------------------*
 *          SCORE OBJECT              *
 *------------------------------------*/

Score::Score() {
    currScore = 0;

    getFileHighScore();
}

void Score::increaseScore() {
    currScore += 10;
}

int Score::getCurrScore() {
    return this->currScore;
}

int Score::getHighScore() {
    return this->highScore;
}

void Score::resetScore() {
    currScore = 0;
}

void Score::getFileHighScore() {
    char num[5];
    ifstream input;
    
    // Get the number from file (char form - 5 digits) 
    input.open("score/highscore.txt", ios::in);
    input.getline(num, 5);
    
    int temp;
    
    // convert char array to int
    sscanf(num, "%d", &temp);
   
    // store the retrieved highscore 
    this->highScore = temp;

    input.close();
}

void Score::setFileHighScore() {
    ofstream output;

    output.open("score/highscore.txt", ios::out | ios::trunc);

    output << this->highScore;

    output.close();
}

void Score::checkIfNewHighScore() {
    if (currScore > highScore) {
        highScore = currScore;
        
        // Update the file with the new high score.
        setFileHighScore();
    }
}
