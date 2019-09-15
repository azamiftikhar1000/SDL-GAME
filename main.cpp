#include <iostream>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "lib/game.h"

using namespace std;

class GamePlay {
    Snake snake;
    Food food;
    Score score;
    
    Window gameWindow;
    SDL_Rect *rectSnakeTail, rectSnake, rectFood, scoreContainer, mainContainer;
    SDL_Event event;
    
    TTF_Font *font;

    bool isStarted, gameReset;
    
    public:
        // ** Constructor & Destructor
        GamePlay(int WinWidth, int WinHeight) {
            gameWindow.SetWindowSize(WinWidth, WinHeight);
           
            isStarted = false;   // Game is not started
            gameReset = false;   // Game doesn't neet resetting
            
            snake.setPosition((WinWidth/2), (WinHeight * 0.9)/2); 
            //rectSnakeTail = new SDL_Rect[snake.getLength()];
            
            rectSnake.w = snake.getDimension();
            rectSnake.h = snake.getDimension();
            rectSnake.x = snake.getXPos();
            rectSnake.y = snake.getYPos();

            /* Code for moving snake's tail */
            /*for (int i = 0; i < snake.getLength(); i++) {
                // Dimensions of snake rectangle
                rectSnake[i].w = snake.getDimension();
                rectSnake[i].h = snake.getDimension();
                // Position of snake rectangle
                rectSnake[i].x = (snake.getXPos() - (snake.getDimension() * i) );
                rectSnake[i].y = snake.getYPos();
            }*/

            // Set up the container for Score
            scoreContainer.x = 0;
            scoreContainer.y = 0;
            scoreContainer.w = gameWindow.WIN_WIDTH;
            scoreContainer.h = gameWindow.WIN_HEIGHT * 0.1;

            // Set up container for snake & food
            mainContainer.x = 0;
            mainContainer.y = scoreContainer.h;
            mainContainer.w = gameWindow.WIN_WIDTH;
            mainContainer.h = gameWindow.WIN_HEIGHT * 0.9;
        }

        // ** Member Functions
        void RenderObjects() {
            // put Score container on window
            SDL_RenderSetClipRect(gameWindow.renderer, &scoreContainer);
            SDL_SetRenderDrawColor(gameWindow.renderer, 255, 191, 87, 255);
            SDL_RenderFillRect(gameWindow.renderer, &scoreContainer);
            DisplayScore();

            // put Game container on window
            SDL_SetRenderDrawColor(gameWindow.renderer, 255, 217, 109, 255);
            SDL_RenderSetClipRect(gameWindow.renderer, &mainContainer);
            SDL_RenderFillRect(gameWindow.renderer, &mainContainer);

            // put food on renderer
            SDL_SetRenderDrawColor(gameWindow.renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(gameWindow.renderer, &rectFood); 

            // put snake on renderer
            SDL_SetRenderDrawColor(gameWindow.renderer, 131, 153, 197, 255);
            SDL_RenderFillRect(gameWindow.renderer, &rectSnake);
            

            // Display everything on renderer
            SDL_RenderPresent(gameWindow.renderer); 

            // Continuously move the snake with velocity
            if (isStarted) {
                snake.move();
            }
            
            // Update the Position of the snake
            rectSnake.x = snake.getXPos();
            rectSnake.y = snake.getYPos();
            
            // Update the position of food
            rectFood.x = food.getXPos();
            rectFood.y = food.getYPos();

            // Check if snake has touched the boundaries
            // & if it touches boundaries, stop the game and reset snake's position.
            int sx = rectSnake.x,
                sy = rectSnake.y;
            
            // check if snake eats food
            if (snake.eat(&rectSnake, &rectFood)) {
               food.showFood();
               score.increaseScore();
               snake.setLength(snake.getLength() + 1);
            }

            // Display score and snake's Length
            //cout << "Score: " << score.getCurrScore() << endl;
            //cout << "Snake Length: " << snake.getLength() << endl << endl;
            
            if ((sx == 0)  || (sx == 1170) ||
                (sy == 80) || (sy == 770)) {
                isStarted = false;
                gameReset = true;
                score.checkIfNewHighScore();
            }
        }

        // Draw Text on window
        void DisplayScore() {
            // Initialize font rendering library 
            TTF_Init();

            // Open font in the memory
            TTF_Font *font = TTF_OpenFont("font/RockoFLF-Bold.ttf", 150);
           
            // Display Current Score
            char *text = new char[16];
            sprintf(text, "Score: %d", score.getCurrScore());

            SDL_Color color = {170, 68, 0};
            SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
            
            int TextWidth = 200, 
                TextHeight = 90;
            
            SDL_Rect rect = {970, -7, TextWidth, TextHeight};
            SDL_RenderCopy(gameWindow.renderer, texture, NULL, &rect);

            // Display the High Score
            sprintf(text, "High Score: %d", score.getHighScore());

            surface = TTF_RenderText_Blended(font, text, color);
            texture = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);

            rect.x = 20;
            rect.w = 330;

            SDL_RenderCopy(gameWindow.renderer, texture, NULL, &rect);

            // Release memory of resources.
            delete text;
            TTF_CloseFont(font);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture); 
            TTF_Quit();
        }

        // checks if isStarted is true or false
        // based on its value, it notifies that game is started or not.
        bool checkIsGameStarted() {
            if (!isStarted) {
                return true;
            } else {
                return false;
            }
        }
        
        // Starts the game - if not started 
        // or
        // restarts game - if snake has crashed.
        void startRestartGame() {
            if (gameReset == true) {
                // reset snake's position
                snake.setPosition(
                    (gameWindow.WIN_WIDTH/2), 
                    (gameWindow.WIN_HEIGHT * 0.9)/2
                );

                food.setPosition(0, 0); 
                snake.reset();
                score.resetScore();

                // Do this so reset doesn't take place again.
                gameReset = false;
            } else {
                // Generate food 
                food.showFood();

                rectFood.w = food.getDimension();
                rectFood.h = food.getDimension();
                rectFood.x = food.getXPos();
                rectFood.y = food.getYPos();

                // Start the game
                isStarted = true;
            }
        }


        bool HandleEvents() {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_WINDOWEVENT_CLOSE:
                        if (gameWindow.window) {
                             gameWindow.destroy();
                             gameWindow.window = NULL;
                        }
                        break;
                          
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            /**** Changes Direction(aka Velocity) of Snake ****/

                            //  =========@
                            //
                            // if Snake is facing right or left direction:
                            //   do nothing
                            // else:
                            //   change the direction of snake to LEFT.
                            //
                            case SDLK_LEFT:
                                if (checkIsGameStarted()) {
                                    startRestartGame();
                                    break;
                                } else {
                                if ((snake.getVelocity() == 'x') && (snake.getVeloSign() == 'p') ||
                                    (snake.getVelocity() == 'x') && (snake.getVeloSign() == 'n')) {
                                        break;
                                    } else {
                                        snake.setVelocity('x', 'n');
                                    }
                                }
                                break;

                            // @=========
                            //
                            // if Snake is facing Right or Left Direction:
                            //   do nothing
                            // else:
                            //   change the direction of snake to RIGHT.
                            //     
                            case SDLK_RIGHT:
                                if (checkIsGameStarted()) {
                                    startRestartGame();
                                    break;
                                } else {
                                if ((snake.getVelocity() == 'x') && (snake.getVeloSign() == 'p') ||
                                    (snake.getVelocity() == 'x') && (snake.getVeloSign() == 'n')) {
                                        break;
                                    } else {
                                        snake.setVelocity('x', 'p');
                                    }
                                }
                                break;

                            // ()
                            // ||
                            // ||
                            // ||
                            // ||
                            // 
                            // if Snake is facing upward or downward Direction:
                            //   do nothing
                            // else
                            //   change direction of snake to UP.
                            //      
                            case SDLK_UP:
                                if (checkIsGameStarted()) {
                                    startRestartGame();
                                    break;
                                } else {
                                if ((snake.getVelocity() == 'y') && (snake.getVeloSign() == 'p') ||
                                    (snake.getVelocity() == 'y') && (snake.getVeloSign() == 'n')) {
                                        break;
                                    } else {
                                        snake.setVelocity('y', 'n');
                                    }
                                }
                                break;

                            // ||
                            // ||
                            // ||
                            // ||
                            // ()
                            //
                            // if Snake is facing upward or downward Direction:
                            //   do nothing
                            // else
                            //   change direction of snake to DOWN.
                            //      
                            case SDLK_DOWN:
                                if (checkIsGameStarted()) {
                                    startRestartGame();
                                    break;
                                } else {
                                if ((snake.getVelocity() == 'y') && (snake.getVeloSign() == 'p') ||
                                    (snake.getVelocity() == 'y') && (snake.getVeloSign() == 'n')) {
                                        break;
                                    } else {
                                        snake.setVelocity('y', 'p');
                                    }
                                }
                                break;

                             default:
                                if (checkIsGameStarted()) {
                                    startRestartGame();
                                }
                        }
                        break;

                    case SDL_QUIT:
                        gameWindow.destroy();
                        return true;
                }
            }
        }
        
        void StartGame() {
            gameWindow.CreateWindow();
            
            bool done = false;
            
            while (!done) {
                done = HandleEvents();
                
                RenderObjects();
                
                SDL_Delay(18);
            }
        }
};

main() {
    GamePlay game(1200, 800);

    game.StartGame();
}
