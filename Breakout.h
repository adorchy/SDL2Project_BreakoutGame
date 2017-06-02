#ifndef BREAKOUT_H_INCLUDED
#define BREAKOUT_H_INCLUDED

#endif // BREAKOUT_H_INCLUDED
#include "Define.h"

/** position, speed and size values are in pixels by default, unless specified otherwise. **/

//Paddle data, for for position, d for direction
typedef struct Paddle{
    float px,py,dx;

}Paddle;

//Ball data, p for position, s for speed, value are in pixels
typedef struct Ball{
    float px, py, sx, sy;
    float radius;
}Ball;

// SDL2 pointer
typedef struct Display{
     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_pTextureText1;
     SDL_Texture *g_pTextureText2;
     SDL_Texture *g_pTextureText3;
     SDL_Texture *g_pTextureText4;
     SDL_Texture *g_pTexturePaddle;
     SDL_Texture *g_pTextureBrick;
     SDL_Texture *g_pTextureSide;
     SDL_Surface *g_pSurface;
}Display;

// Brick structure
typedef struct Brick{
    float x,y;
    int state,color;

}Brick;

// Breakout Game structure
typedef struct BreakoutGame{
    Display display;
    Ball ball;
    Paddle paddle;
    Brick bricks[BRICK_NUMBER];
    int playerTotalScore;
    int playerCurrentTurnScore;
    int life;
    int ballIsMoving;

}BreakoutGame;

//Used to load a font
typedef struct font{
    TTF_Font *g_font;
}font;


enum BOOL{
    True,False
};

//prototypes
void initBreakoutGame (BreakoutGame *myGame);
void initFont (font *myFont);
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,BreakoutGame *myGame);

void loadIntroTextures (BreakoutGame *myGame, font myFont);
void handleIntroEvents(int *introIsRunning, int *gameIsRunning, BreakoutGame *myGame);
void displayIntroWindow(BreakoutGame *myGame);
void destroyIntroTextures(BreakoutGame *myGame);
void loadGameTextures (BreakoutGame *myGame);
void handleGameEvents(int *gameIsRunning, BreakoutGame *myGame);
void playerPaddleMove (BreakoutGame *myGame);

void renderBricks (BreakoutGame *myGame);
void renderPaddle(BreakoutGame *myGame);
void renderSides(BreakoutGame *myGame);
void renderBall(BreakoutGame *myGame, int R, int G, int B);
void renderPlayerScore (BreakoutGame *myGame, font myFont);
void renderBreakoutGame (BreakoutGame myGame, font myFont);


void checkPlayerStats (int *gameIsRunning, BreakoutGame *myGame, font myFont);
void resetBall (BreakoutGame *myGame);
enum BOOL CheckCollisionBallPaddles (BreakoutGame myGame);
void handleCollisionBallWalls (BreakoutGame *myGame);
void handleCollisionBallBrick (BreakoutGame *myGame);
void handleCollisionBallPaddle (BreakoutGame *myGame);
void ballMovement(BreakoutGame *myGame);
void delay(unsigned int frameLimit);
void displayEndWindow (BreakoutGame *myGame, font myFont);
void destroyGame(BreakoutGame *myGame);
void releaseFont (font *myFont);
