#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Window {
    private: 
        SDL_Window *window;
        SDL_Renderer *renderer;
        int WIN_WIDTH, WIN_HEIGHT;

    public:
        // Constructors
        Window();
        Window(int, int);

        friend class GamePlay;

        // Member Functions
        void SetWindowSize(int, int);
        void CreateWindow();
        void destroy();
};

class Position {
    int x, y;

    public: 
        int getXPos();
        int getYPos();
        void setPosition(int, int);  
};

class Snake {
    Position CurrPosition;
    char velocity, veloSign;
    int length, dimension;

    public:
        Snake();

        int getLength();
        void setLength(int);
        int getDimension();     // Width & Height of a single block of snake
        
        // Getters and Setter for snake position
        void setPosition(int, int);
        int getXPos();
        int getYPos();
        void setVelocity(char, char);
        char getVelocity();
        char getVeloSign();
        void reset();
        
        // main behavior of snake
        void move();
        bool eat(SDL_Rect *, SDL_Rect *);
};

class Food {
    Position CurrPosition;
    const int dimension = 20; 

    public:
        void showFood();
        
        int getDimension();
        int getXPos();
        int getYPos();
        void setPosition(int, int);
};

class Score {
    Position fixedPosition;
    int currScore;
    int highScore;

    public:
        Score();

        void increaseScore();
        int getCurrScore();
        int getHighScore();
        void resetScore();

        void getFileHighScore();  // Retrieve high score from the file
        void setFileHighScore();  // Overwrite high score into the file
        void checkIfNewHighScore(); 

};

#endif


