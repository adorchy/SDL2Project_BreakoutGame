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
     SDL_Texture *g_pTexture1;
     SDL_Texture *g_pTexture2;
     SDL_Texture *g_pTexture3;
     SDL_Surface *g_pSurface;
}Display;

// Brick structure
typedef struct Brick{
    float x,y;
    int state;

}Brick;

// Breakout Game structure
typedef struct BreakoutGame{
    Display display;
    Ball ball;
    Paddle paddle;
    Brick bricks[BRICK_NUMBER];
    float timer;

}BreakoutGame;



//Used to load a font
typedef struct font{
    TTF_Font *g_font;
}font;

enum Collision {
    Top, Bottom, Right, Left, None
};

enum BOOL{
    True,False
};

//prototypes
void initBreakoutGame (BreakoutGame *myGame);
void initFont (font *myFont);
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,BreakoutGame *myGame);

void handleIntroEvents(int *introIsRunning, int *gameIsRunning, BreakoutGame *myGame);
void introWindow(BreakoutGame *myGame, font myFont);
void handleGameEvents(int *gameIsRunning, BreakoutGame *myGame);
void playerPaddleMove (BreakoutGame *myGame);

void renderBricks (BreakoutGame *myGame);
void renderPaddle(BreakoutGame *myGame);
void renderBorders(BreakoutGame *myGame, int colorR, int colorG, int colorB);
void renderBall(BreakoutGame *myGame, int R, int G, int B);
void renderPlayerScore (BreakoutGame *myGame, font myFont);
void renderBreakoutGame (BreakoutGame myGame, font myFont);

enum Collision CheckCollisionBallWalls (BreakoutGame myGame);
void checkVictoryConditions (int *gameIsRunning, BreakoutGame *myGame, font myFont);
void resetBall (BreakoutGame *myGame);
enum BOOL CheckCollisionBallPaddles (BreakoutGame myGame);
void ballMovementAndScore(BreakoutGame *myGame);
void delay(unsigned int frameLimit);
void endWindow (BreakoutGame *myGame, font myFont, int winner);
void destroy(BreakoutGame *myGame);
void releaseFont (font *myFont);
