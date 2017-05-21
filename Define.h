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

// Screen size
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780

// Ball
#define BALL_RADIUS 12.0
#define BOUNCE_SPEED 1.2

// Paddle
#define PADDLE_MOVE_INCREMENT 280
#define PADDLE_MAX_SPEED 9
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 13

// Brick
#define BRICK_NUMBER 7

// Position of "Welcome to Pong" window
#define  MAIN_TEXT_X SCREEN_WIDTH/2-SCREEN_WIDTH/4;//start point (x)
#define  MAIN_TEXT_Y SCREEN_HEIGHT/3;// start point (y)
#define  MAIN_TEXT_W SCREEN_WIDTH/2; //Width
#define  MAIN_TEXT_H SCREEN_HEIGHT/8; //Height

// Position of "Press space to start" window

#define START_X SCREEN_WIDTH/2-SCREEN_WIDTH/8;//start point (x)
#define START_Y SCREEN_HEIGHT/1.8;// start point (y)
#define START_W SCREEN_WIDTH/4; //Width
#define START_H SCREEN_HEIGHT/15; //Height


#endif // DEFINE_H_INCLUDED
