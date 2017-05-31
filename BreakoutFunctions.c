#include "Breakout.h"
// Copryright Arnaud d'Orchymont 2017

void initBreakoutGame (BreakoutGame *myGame){

//ball
 myGame->ball.px=SCREEN_WIDTH/2;
 myGame->ball.py=SCREEN_HEIGHT-45;
 myGame->ball.sx=-4.0;
 myGame->ball.sy=-2.50;
 myGame->ball.radius=BALL_RADIUS;

 //paddle
 myGame->paddle.px=SCREEN_WIDTH/2-PADDLE_WIDTH/2;
 myGame->paddle.py=SCREEN_HEIGHT-PADDLE_HEIGHT;
 myGame->paddle.dx=SCREEN_WIDTH/2-PADDLE_WIDTH/2;

 //results
 myGame->timer=0.0;
 myGame->score=0;
 myGame->life=TRY_NUMBER;
 myGame->ballIsMoving=0;

//Bricks
int i,x,y;
for (i=0, x=SIDE_WIDTH, y=120 ; i<BRICK_NUMBER; i++, x+=BRICK_WIDTH) // increment x
    {
        if (i % 12 == 0) //every x brick number increment y by BRICK_HEIGHT and reset x to 0
        {
            x=SIDE_WIDTH;
            y+=BRICK_HEIGHT;
        }
        myGame->bricks[i].state=1;
        myGame->bricks[i].color=rand() % 4;
        myGame->bricks[i].x=x;
        myGame->bricks[i].y=y;
    }
}

/********************************************************************
PURPOSE : font loading
INPUT : fontObject
OUTPUT : g_font
*********************************************************************/
void initFont (font *myFont){
    if(TTF_Init() == -1)
        {
            fprintf(stderr, "Error unable to initialize TTF_Init : %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
            }

    myFont->g_font=TTF_OpenFont("./assets/fonts/gameplay/Gameplay.ttf",65);

    if (!myFont->g_font)
    {
            fprintf(stderr, "Error font is missing : %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
    }
}

/********************************************************************
PURPOSE : SDL initialization
INPUT : title, window position, window size, flags, gameObject
OUTPUT: g_pWindow, g_pRenderer
*********************************************************************/
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,BreakoutGame *myGame){

    myGame->display.g_pWindow=NULL;
    myGame->display.g_pRenderer=NULL;
    myGame->display.g_pTextureText1=NULL;
    myGame->display.g_pTextureText2=NULL;
    myGame->display.g_pTextureText3=NULL;
    myGame->display.g_pTexturePaddle=NULL;
    myGame->display.g_pTextureBrick=NULL;
    myGame->display.g_pTextureSide=NULL;
    myGame->display.g_pSurface=NULL;

    //initialize SDL

    if(SDL_Init(SDL_INIT_VIDEO)>=0) // Loading video (return 0 on success)
    {
            //if succeeded create our window
            myGame->display.g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, then create our render
            if(myGame->display.g_pWindow!=NULL){
                myGame->display.g_pRenderer=SDL_CreateRenderer(myGame->display.g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);
            }
    }else{

        return 0;
    }
    return 1;
}

/*****************************************************
PURPOSE : Load textures used in introduction window
INPUT : fontObject, gameObject
OUTPUT: g_pTextureText1, g_pTextureText2
*****************************************************/
void loadIntroTextures (BreakoutGame *myGame, font myFont){
    SDL_Color fontColor={255,255,255};

    //Title 1
    myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "WELCOME TO BREAKOUT!", fontColor);//Charge la police
    if(myGame->display.g_pSurface)
    {
             myGame->display.g_pTextureText1 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
             SDL_FreeSurface(myGame->display.g_pSurface);
    }
    else
    {
        fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
    }

    //Title 2
    myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "Press space to start", fontColor);//load font
    if(myGame->display.g_pSurface)
    {
         myGame->display.g_pTextureText2 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
         SDL_FreeSurface(myGame->display.g_pSurface);
    }
    else
    {
    fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
    }

    myGame->display.g_pSurface=NULL;

}

/********************************************************************
PURPOSE : events management (input=>keyboard)
INPUT : 2 states variables, gameObject
OUTPUT : introIsRunning or (gameIsRunning and introIsRunning)
*********************************************************************/
void handleIntroEvents(int *introIsRunning, int *gameIsRunning, BreakoutGame *myGame){
        SDL_Event event;

    if(SDL_PollEvent(&event))
    {
        switch(event.type){
            case SDL_QUIT:
                *introIsRunning=0; *gameIsRunning= 0; break;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE: *introIsRunning=0; break;
                    default:break;
                }
                break;

            case SDL_KEYUP:
                break;

            default:
                break;
        }
    }
}

/********************************************************
PURPOSE : create and display an introduction window
INPUT : gameObject, fontObject
OUTPUT : window with 2 textures containing text
********************************************************/
void displayIntroWindow(BreakoutGame *myGame){

    SDL_Rect IntroRect1; //Rectangle used to display character chain
    IntroRect1.x=MAIN_TITLE_X;//start point (x)
    IntroRect1.y=MAIN_TITLE_Y;// start point (y)
    IntroRect1.w=MAIN_TITLE_W; //Width
    IntroRect1.h=MAIN_TITLE_H; //Height

    //  copy a portion of the texture to the current rendering target
    SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureText1,NULL,&IntroRect1);


    SDL_Rect IntroRect2; //Rectangle to write character chain
    IntroRect2.x=SECONDARY_TITLE_X; //start point (x)
    IntroRect2.y=SECONDARY_TITLE_Y; // start point (y)
    IntroRect2.w=SECONDARY_TITLE_W; //Width
    IntroRect2.h=SECONDARY_TITLE_H; //Height

    //  copy a portion of the texture to the current rendering target
    SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureText2,NULL,&IntroRect2);


    SDL_RenderPresent(myGame->display.g_pRenderer);

}

/********************************************************
PURPOSE : destroy textures used in introduction window
INPUT : gameObject
********************************************************/
void destroyIntroTextures(BreakoutGame *myGame){
    if(myGame->display.g_pTextureText1!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureText1);

    if(myGame->display.g_pTextureText2!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureText2);
}

/**********************************************************
PURPOSE : Load textures used in Breakout game
INPUT : gameObject
OUTPUT: g_pTextureBrick, g_pTexturePaddle, g_pTextureSide
**********************************************************/
void loadGameTextures (BreakoutGame *myGame){
    // bricks
    myGame->display.g_pSurface = IMG_Load("./assets/bricks.png");//Img loading

    if(myGame->display.g_pSurface)
    {
        // create a texture from an existing surface.
        myGame->display.g_pTextureBrick = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
        SDL_FreeSurface(myGame->display.g_pSurface); //  free an RGB surface

        if (!myGame->display.g_pTextureBrick)
        {
            fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
        }
    }
    else
    {
        fprintf(stdout,"IMG_Load: %s\n", IMG_GetError()); // handle error
    }

    // paddle
    myGame->display.g_pSurface = IMG_Load("./assets/paddle.png");//Img loading

    if(myGame->display.g_pSurface){
            // create a texture from an existing surface.
        myGame->display.g_pTexturePaddle = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
        SDL_FreeSurface(myGame->display.g_pSurface); //  free an RGB surface

        if (!myGame->display.g_pTexturePaddle)
        {
            fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
        }
    }
    else
    {
        fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());;
    }

    // side
    myGame->display.g_pSurface = IMG_Load("./assets/side.png");//Img loading

    if(myGame->display.g_pSurface){
            // create a texture from an existing surface.
        myGame->display.g_pTextureSide = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
        SDL_FreeSurface(myGame->display.g_pSurface); //  free an RGB surface

        if (!myGame->display.g_pTextureSide)
        {
            fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
        }
    }
    else
    {
        fprintf(stdout,"IMG_Load: %s\n", IMG_GetError()); // handle error
    }

    //free pointer
    myGame->display.g_pSurface=NULL;
}

/********************************************************************************************
PURPOSE :
Events management (input=>keyboard)
Indicates in what direction player's paddle should go when player press Up or Down arrow key
INPUT : state variable, gameObject
OUTPUT : gameIsRunning, ballIsMoving, paddle1.dy
*********************************************************************************************/
void handleGameEvents(int *gameIsRunning, BreakoutGame *myGame){

    SDL_Event event;

    if(SDL_PollEvent(&event))
    {
        switch(event.type){
            case SDL_QUIT:
                  *gameIsRunning=0; break;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT: myGame->paddle.dx-=PADDLE_MOVE_INCREMENT; break;
                    case SDLK_RIGHT: myGame->paddle.dx+=PADDLE_MOVE_INCREMENT;break;
                    case SDLK_SPACE: myGame->ballIsMoving=1; break;
                    default:break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                   case SDLK_LEFT: myGame->paddle.dx=myGame->paddle.px;break;
                   case SDLK_RIGHT: myGame->paddle.dx=myGame->paddle.px;break;
                   default:break;
                }

                break;

            default:
                break;
        }
    }
}

/*********************************************************************
PURPOSE :
Move player's paddle and give a smooth move to player's paddle
Paddle max speed: 9 pixels per 16 ms
INPUT : gameObject
OUTPUT : paddle.px
*********************************************************************/
void playerPaddleMove (BreakoutGame *myGame){

    if (myGame->paddle.px>myGame->paddle.dx){
        if (myGame->paddle.px>SIDE_WIDTH) // stuck paddle inside the screen (right side)
        {
                myGame->paddle.px-=PADDLE_MAX_SPEED;
        }
    }

    if (myGame->paddle.px<myGame->paddle.dx){
        if (myGame->paddle.px<SCREEN_WIDTH-PADDLE_WIDTH-SIDE_WIDTH) // stuck paddle inside the screen (left side)
        {
            myGame->paddle.px+=PADDLE_MAX_SPEED;
        }
    }
}

/********************************************************************
PURPOSE : render the bricks
INPUT : gameObject
*********************************************************************/
void renderBricks(BreakoutGame *myGame) {

    SDL_Rect rectangleDest;
    rectangleDest.w=BRICK_WIDTH;
    rectangleDest.h=BRICK_HEIGHT;

    SDL_Rect redRectangleSource;
    redRectangleSource.x=0;
    redRectangleSource.y=0;
    redRectangleSource.w=64;
    redRectangleSource.h=24;

    SDL_Rect yellowRectangleSource;
    yellowRectangleSource.x=64;
    yellowRectangleSource.y=0;
    yellowRectangleSource.w=64;
    yellowRectangleSource.h=24;

    SDL_Rect greenRectangleSource;
    greenRectangleSource.x=0;
    greenRectangleSource.y=24;
    greenRectangleSource.w=64;
    greenRectangleSource.h=24;

    SDL_Rect blueRectangleSource;
    blueRectangleSource.x=64;
    blueRectangleSource.y=24;
    blueRectangleSource.w=64;
    blueRectangleSource.h=24;

    int i;
    for (i=0 ; i<BRICK_NUMBER; i++)
    {
        if (myGame->bricks[i].state==1)
        {
            rectangleDest.x=myGame->bricks[i].x;
            rectangleDest.y=myGame->bricks[i].y;

            if (myGame->bricks[i].color == 0)
            {
                SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureBrick,&redRectangleSource,&rectangleDest); // copy a portion of the texture to the current rendering target
            }

            if (myGame->bricks[i].color == 1)
            {
                SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureBrick,&yellowRectangleSource,&rectangleDest); // copy a portion of the texture to the current rendering target
            }

            if (myGame->bricks[i].color == 2)
            {
                SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureBrick,&greenRectangleSource,&rectangleDest); // copy a portion of the texture to the current rendering target
            }

            if (myGame->bricks[i].color == 3)
            {
                SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureBrick,&blueRectangleSource,&rectangleDest); // copy a portion of the texture to the current rendering target
            }
        }
    }
}

/********************************************************************
PURPOSE : render the paddle
INPUT : gameObject
*********************************************************************/
void renderPaddle(BreakoutGame *myGame) {

    SDL_Rect rectangleDest;
    rectangleDest.x=myGame->paddle.px;
    rectangleDest.y=myGame->paddle.py;
    rectangleDest.w=PADDLE_WIDTH;
    rectangleDest.h=PADDLE_HEIGHT;

    SDL_Rect rectangleSource;
    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=128;
    rectangleSource.h=32;

    SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTexturePaddle,&rectangleSource,&rectangleDest); // copy a portion of the texture to the current rendering target

}

/********************************************************************
PURPOSE : render sides
INPUT : gameObject
*********************************************************************/
void renderSides(BreakoutGame *myGame){

    SDL_Rect rectangleDest1;
    rectangleDest1.x=0;
    rectangleDest1.y=0;
    rectangleDest1.w=SIDE_WIDTH;
    rectangleDest1.h=SCREEN_HEIGHT;

    SDL_Rect rectangleDest2;
    rectangleDest2.x=SCREEN_WIDTH-SIDE_WIDTH;
    rectangleDest2.y=0;
    rectangleDest2.w=SIDE_WIDTH;
    rectangleDest2.h=SCREEN_HEIGHT;

    SDL_Rect rectangleSource;
    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=SIDE_WIDTH;
    rectangleSource.h=SIDE_HEIGHT;

    SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureSide,&rectangleSource,&rectangleDest1); // copy a portion of the texture to the current rendering target
    SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureSide,&rectangleSource,&rectangleDest2);
}

/********************************************************************
PURPOSE : render the pong ball
INPUT : gameObject, color (RGB)
OUTPUT : a filled circle
*********************************************************************/
void renderBall(BreakoutGame *myGame, int R, int G, int B){
    SDL_SetRenderDrawColor(myGame->display.g_pRenderer, R, G, B, SDL_ALPHA_OPAQUE);
    int radiusMin;

    int x, y = 0;

    for (radiusMin = myGame->ball.radius ; radiusMin>=0; radiusMin--){
        for (float angle = 0.0; angle<360; angle++){
            x = myGame->ball.px-radiusMin * cos(angle);
            y = myGame->ball.py-radiusMin * sin(angle);
            SDL_RenderDrawPoint(myGame->display.g_pRenderer, x, y);
            }
    }
}

/********************************************************************
PURPOSE : display player's score and player's life
INPUT : gameObject, fontObject
OUTPUT : 2 textures
*********************************************************************/
void renderPlayerScore (BreakoutGame *myGame, font myFont){

        // score
        char playerScoreArr [2];
        sprintf (playerScoreArr, "score: %i", myGame->score);
        //fprintf(stdout,"score player:%c%c\n", playerScoreArr[0],playerScoreArr[1]);
        SDL_Color fontColor={255,255,255};
        myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, playerScoreArr, fontColor);

        // Rectangle used to display score
        SDL_Rect playerScoreRect;
        playerScoreRect.x=SCORE_WINDOW_X;//start point (x)
        playerScoreRect.y=SCORE_WINDOW_Y; //start point (y)
        playerScoreRect.w=SCORE_WINDOW_W; //Width
        playerScoreRect.h=SCORE_WINDOW_Y; //Height

        if(myGame->display.g_pSurface){

                 myGame->display.g_pTextureText1 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                 SDL_FreeSurface(myGame->display.g_pSurface);


                 if(myGame->display.g_pTextureText1){
                        // copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureText1,NULL,&playerScoreRect);
                        SDL_DestroyTexture(myGame->display.g_pTextureText1);
                 }
                 else{
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                }

                }
        else{
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
        //Life
        char playerTryArr [1];
        sprintf (playerTryArr, "Life: %i", myGame->life);


        myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, playerTryArr, fontColor);

        // Rectangle used to display number of try
        SDL_Rect playerLifeRect;
        playerLifeRect.x=150;//start point (x)
        playerLifeRect.y=SCORE_WINDOW_Y; //start point (y)
        playerLifeRect.w=SCORE_WINDOW_W-20; //Width
        playerLifeRect.h=SCORE_WINDOW_Y; //Height

        if(myGame->display.g_pSurface){

                 myGame->display.g_pTextureText2 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                 SDL_FreeSurface(myGame->display.g_pSurface);


                 if(myGame->display.g_pTextureText2){
                        // copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureText2,NULL,&playerLifeRect);
                        SDL_DestroyTexture(myGame->display.g_pTextureText2);
                 }
                 else{
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                }

                }
        else{
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }

}

/********************************************************************
PURPOSE :
Procedure calling all the render functions
Give a black background to the game
INPUT : gameObject, fontObject, color (RGB)
*********************************************************************/
void renderBreakoutGame (BreakoutGame myGame, font myFont){

    renderPaddle(&myGame);
    renderBall (&myGame,255,255,255);
    renderBricks(&myGame);
    renderSides (&myGame);
    renderPlayerScore (&myGame, myFont);

    SDL_RenderPresent(myGame.display.g_pRenderer); // update the screen with any rendering performed since the previous cal

    SDL_SetRenderDrawColor(myGame.display.g_pRenderer,0,0,0,255); // black background
    SDL_RenderClear(myGame.display.g_pRenderer);
}

/*****************************************************************************************
PURPOSE : Check if the player has won or lost and call displayEndWindow procedure if True
INPUT : gameIsRunning, gameObject, fontObject
OUTPUT : gameIsRunning
*****************************************************************************************/

void checkVictoryConditions (int *gameIsRunning, BreakoutGame *myGame, font myFont){
    if (myGame->life <=0){
        displayEndWindow (myGame, myFont, 0);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }

    if (myGame->score == BRICK_NUMBER){
        displayEndWindow (myGame, myFont, 1);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }
}

/********************************************************************
PURPOSE : Reset ball position and direction
INPUT : gameObject
OUTPUT : ball.px, ball.py, ball.sx, ball.sy
*********************************************************************/
void resetBall (BreakoutGame *myGame){
    myGame->ball.px= SCREEN_WIDTH/2 + (rand () % 200);
    myGame->ball.py= SCREEN_HEIGHT/3 + (rand () % 200);
    myGame->ball.sx= (rand () % 2 + 4) + cos (rand () % 90);
    myGame->ball.sy= (rand () % 1 + 2) + cos (rand () % 90);

    if ((rand () % 2)==1)
    {
        myGame->ball.sx*=-1;
    }
}

/********************************************************************
PURPOSE : Check if the ball hits a racket
INPUT : gameObject
OUTPUT : return True or False
*********************************************************************/
enum BOOL CheckCollisionBallPaddles (BreakoutGame myGame){
    //Racket
   if ((myGame.ball.py+myGame.ball.radius)>=myGame.paddle.py &&
                        myGame.ball.px>=myGame.paddle.px &&
                        myGame.ball.px<=(myGame.paddle.px+PADDLE_WIDTH)){
                            fprintf(stdout,"collision on Racket\n");
                            return True;
                        }
    return False;
};

/****************************************************************************
PURPOSE : Check if the ball hits a window's border and adjust ball direction
INPUT : gameObject
OUTPUT : ball.sy, ball.sx, life
****************************************************************************/
void handleCollisionBallWalls (BreakoutGame *myGame){
    //check if ball hit right side
    if (myGame->ball.px >=SCREEN_WIDTH-BALL_RADIUS-SIDE_WIDTH)
    {
        if (myGame->ball.sx>0)
        {
        myGame->ball.sx=-myGame->ball.sx*BOUNCE_SPEED;
        myGame->ball.sy*=1.15;
        }
    }
    //check if ball hit left side
    if (myGame->ball.px <=SIDE_WIDTH+BALL_RADIUS)
    {
        if (myGame->ball.sx<0)
        {
                myGame->ball.sx=-myGame->ball.sx*BOUNCE_SPEED;
                myGame->ball.sy*=1.15;
        }
    }
    //check if ball hit top
    if (myGame->ball.py <= BALL_RADIUS)
    {
        myGame->ball.sy=-myGame->ball.sy*BOUNCE_SPEED;
    }
    //check if ball hit bottom
    if (myGame->ball.py >= SCREEN_HEIGHT-BALL_RADIUS)
    {
        myGame->life -=1;
        resetBall (myGame);
    }
}

/********************************************************************
PURPOSE : Check if the ball hits a brick and adjust ball direction
INPUT : gameObject
OUTPUT : ball.sx or ball.sy
********************************************************************/
void handleCollisionBallBrick (BreakoutGame *myGame){

    int i;
    for (i=0 ; i<BRICK_NUMBER; i++)
    {
        if (
            myGame->bricks[i].state==1 &&
            myGame->ball.px-BALL_RADIUS<=myGame->bricks[i].x+BRICK_WIDTH &&
            myGame->ball.px+BALL_RADIUS>=myGame->bricks[i].x &&
            myGame->ball.py+BALL_RADIUS>=myGame->bricks[i].y &&
            myGame->ball.py-BALL_RADIUS<=myGame->bricks[i].y+BRICK_HEIGHT
            )
        {

            if (myGame->ball.py>=myGame->bricks[i].y+BRICK_HEIGHT) // hit bottom
            {
                myGame->bricks[i].state=0;
                myGame->score+=1;
                fprintf(stdout,"hit bottom\n");
                if (myGame->ball.sy<0) // in case ball hits 2 bricks at the same time
                {
                   myGame->ball.sy*=-1;
                }
            }
            else if (myGame->ball.py<=myGame->bricks[i].y) // hit top
            {
                myGame->bricks[i].state=0;
                myGame->score+=1;
                fprintf(stdout,"hit top\n");
                if (myGame->ball.sy>0) // in case ball hits 2 bricks at the same time
                {
                    myGame->ball.sy*=-1;
                }
            }
            else if (myGame->ball.px>=myGame->bricks[i].x+BRICK_WIDTH) // hit right
            {
                myGame->bricks[i].state=0;
                myGame->score+=1;
                fprintf(stdout,"hit right\n");
                if (myGame->ball.sx<0) // in case ball hits 2 bricks at the same time
                {
                    myGame->ball.sx*=-1;
                }
            }
            else if (myGame->ball.px<=myGame->bricks[i].x) // hit left
            {
                myGame->bricks[i].state=0;
                myGame->score+=1;
                fprintf(stdout,"hit left\n");
                if (myGame->ball.sx>0) // in case ball hits 2 bricks at the same time
                {
                    myGame->ball.sx*=-1;
                }
            }
        }
    }
}

/*****************************************************************
PURPOSE : Check if the ball hits paddle and adjust ball direction
INPUT : gameObject
OUTPUT : ball.sx, ball.sy
*****************************************************************/
void handleCollisionBallPaddle (BreakoutGame *myGame){
    if (CheckCollisionBallPaddles (*myGame)== True)
    {
        if (myGame->ball.sy>0)
        {
            myGame->ball.sx*=1.15;
            myGame->ball.sy=-myGame->ball.sy*BOUNCE_SPEED;
        }
    }
}

/*****************************************
PURPOSE : increment ball position
INPUT : gameObject
OUTPUT : ball.px, ball.py
*****************************************/
void ballMovement(BreakoutGame *myGame){

    //ball speed cap
    if (myGame->ball.sx>BALL_RADIUS-3)
    {
        myGame->ball.sx=BALL_RADIUS-3;
    }

    if (myGame->ball.sy>BALL_RADIUS-3)
    {
        myGame->ball.sy=BALL_RADIUS-3;
    }

    if (myGame->ball.sy<(BALL_RADIUS-3)*-1)
    {
        myGame->ball.sy=(BALL_RADIUS-3)*-1;
    }

    if (myGame->ball.sx<(BALL_RADIUS-3)*-1)
    {
        myGame->ball.sx=(BALL_RADIUS-3)*-1;
    }

    if ( myGame->ballIsMoving==1)
    {
        //ball movement
        myGame->ball.px+=myGame->ball.sx;
        myGame->ball.py+=myGame->ball.sy;
    }
}

/********************************************************************
PURPOSE :
FrameRate management
Capped at 60fps(1 frame/16ms => (1/60fps)*1000 =16.67ms)
INPUT : FrameLimit = SDL_GetTicks() + 16;
*********************************************************************/
void delay(unsigned int frameLimit){

    unsigned int ticks = SDL_GetTicks();
/*
    int preFPS = ticks - frameLimit + 16;
    float FPS = 1/((float)preFPS/1000);

    fprintf(stdout,"FPS: %f\n", FPS);*/
    if (frameLimit < ticks)
    {
        return;
    }
    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }
    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

/********************************************************************
PURPOSE : create and display a window displaying the result
INPUT : gameObject, fontObject,
OUTPUT : window with 1 texture containing the result.
*********************************************************************/
void displayEndWindow (BreakoutGame *myGame, font myFont, int winner){
SDL_Color fontColor={255,255,255};

    SDL_Rect Window1; //Rectangle used to display character chain
    Window1.x=MAIN_TITLE_X;//start point (x)
    Window1.y=MAIN_TITLE_Y;// start point (y)
    Window1.w=MAIN_TITLE_W; //Width
    Window1.h=MAIN_TITLE_H; //Height

    if (myGame->display.g_pTextureText3==NULL)
    {
        if (winner==0)
        {
            myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "You lost!", fontColor);
        }
        else
        {
            myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "You won!", fontColor);
        }

        if(myGame->display.g_pSurface)
         {
                     myGame->display.g_pTextureText3 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                     SDL_FreeSurface(myGame->display.g_pSurface);
        }
        else
        {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
    }

    if(myGame->display.g_pTextureText3)
    {
            //  copy a portion of the texture to the current rendering target
            SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTextureText3,NULL,&Window1);
            SDL_RenderPresent(myGame->display.g_pRenderer);
    }
}

/********************************************************************
PURPOSE :
Destroy the rendering context, the texture and the surface
Free RGB surface
INPUT: GameObject
*********************************************************************/
void destroyGame(BreakoutGame *myGame){

    //Destroy render
    if(myGame->display.g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->display.g_pRenderer);

    //Destroy surface
    if(myGame->display.g_pSurface!=NULL)
        SDL_FreeSurface(myGame->display.g_pSurface);

    //Destroy texture
    if(myGame->display.g_pTextureText1!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureText1);

    if(myGame->display.g_pTextureText2!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureText2);

    if(myGame->display.g_pTextureText3!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureText3);

    if(myGame->display.g_pTexturePaddle!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTexturePaddle);

    if(myGame->display.g_pTextureBrick!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureBrick);

    if(myGame->display.g_pTextureSide!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTextureSide);

    //Destroy window
    if(myGame->display.g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->display.g_pWindow);

}

/********************************************************************
PURPOSE : Free the memory used by font
INPUT : fontObject
*********************************************************************/
void releaseFont (font *myFont){
    if (myFont->g_font)
    {
        TTF_CloseFont (myFont->g_font);
    }
}
