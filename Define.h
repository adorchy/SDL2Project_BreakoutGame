#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

// Include
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <math.h>

#define TRY_NUMBER 9


// Screen size
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780

// Ball
#define BALL_RADIUS 12.0
#define BOUNCE_SPEED 1.1

// Paddle
#define PADDLE_MOVE_INCREMENT 280
#define PADDLE_MAX_SPEED 9
#define PADDLE_WIDTH 128
#define PADDLE_HEIGHT 32

//Side
#define SIDE_WIDTH 16
#define SIDE_HEIGHT 600

// Brick
#define BRICK_NUMBER 48
#define BRICK_WIDTH 83
#define BRICK_HEIGHT 31

//Position of Score window
#define SCORE_WINDOW_X 20
#define SCORE_WINDOW_Y 20
#define SCORE_WINDOW_W 100
#define SCORE_WINDOW_H 50

// Position of Main Title window
#define  MAIN_TITLE_X SCREEN_WIDTH/2-SCREEN_WIDTH/4;//start point (x)
#define  MAIN_TITLE_Y SCREEN_HEIGHT/3;// start point (y)
#define  MAIN_TITLE_W SCREEN_WIDTH/2;
#define  MAIN_TITLE_H SCREEN_HEIGHT/8;

// Position of Secondary window

#define SECONDARY_TITLE_X SCREEN_WIDTH/2-SCREEN_WIDTH/8;//start point (x)
#define SECONDARY_TITLE_Y SCREEN_HEIGHT/1.8;// start point (y)
#define SECONDARY_TITLE_W SCREEN_WIDTH/4;
#define SECONDARY_TITLE_H SCREEN_HEIGHT/15;


#endif // DEFINE_H_INCLUDED
