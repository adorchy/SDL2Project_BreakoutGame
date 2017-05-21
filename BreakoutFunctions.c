#include "Breakout.h"
// Copryright Arnaud d'Orchymont 2017

void initBreakoutGame (BreakoutGame *myGame){

//ball
 myGame->ball.px=SCREEN_WIDTH/3;
 myGame->ball.py=SCREEN_HEIGHT/2;
 myGame->ball.sx=4.0;
 myGame->ball.sy=2.50;
 myGame->ball.radius=BALL_RADIUS;

 //padle
 myGame->paddle.px=SCREEN_WIDTH/2-PADDLE_WIDTH/2;
 myGame->paddle.py=SCREEN_HEIGHT-PADDLE_HEIGHT;
 myGame->paddle.dx=SCREEN_WIDTH/2-PADDLE_WIDTH/2;

 //score
 myGame->timer=0.0;

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
OUTPUT: g_pWindow
*********************************************************************/
int initSDL(char *title, int xpos,int ypos,int width, int height,int flags,BreakoutGame *myGame){

    myGame->display.g_pWindow=NULL;
    myGame->display.g_pRenderer=NULL;
    myGame->display.g_pTexture1=NULL;
    myGame->display.g_pTexture2=NULL;
    myGame->display.g_pTexture3=NULL;
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

/********************************************************************
PURPOSE : create and display an introduction window
INPUT : gameObject, fontObject
OUTPUT : window with 2 textures containing text
*********************************************************************/
void introWindow(BreakoutGame *myGame, font myFont){

     SDL_Color fontColor={255,255,255};
    SDL_Rect IntroRect1; //Rectangle used to display character chain
    IntroRect1.x=MAIN_TEXT_X;//start point (x)
    IntroRect1.y=MAIN_TEXT_Y;// start point (y)
    IntroRect1.w=MAIN_TEXT_W; //Width
    IntroRect1.h=MAIN_TEXT_H; //Height

    if (myGame->display.g_pTexture1==NULL)
    {
        myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "WELCOME TO BREAKOUT!", fontColor);//Charge la police
        if(myGame->display.g_pSurface)
        {
                 myGame->display.g_pTexture1 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                 SDL_FreeSurface(myGame->display.g_pSurface);
        }
        else
        {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
    }
    else
    {
        //  copy a portion of the texture to the current rendering target
        SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTexture1,NULL,&IntroRect1);
        //SDL_DestroyTexture(myGame->displayGame.g_pTexture1);
    }

    SDL_Rect IntroRect2; //Rectangle to write character chain
    IntroRect2.x=START_X; //start point (x)
    IntroRect2.y=START_Y; // start point (y)
    IntroRect2.w=START_W; //Width
    IntroRect2.h=START_H; //Height

    if (myGame->display.g_pTexture2==NULL)
    {
        myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, "Press space to start", fontColor);//load font
        if(myGame->display.g_pSurface)
        {
             myGame->display.g_pTexture2 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
             SDL_FreeSurface(myGame->display.g_pSurface);
        }
        else
        {
        fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
    }
    else
    {
        //  copy a portion of the texture to the current rendering target
        SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTexture2,NULL,&IntroRect2);
        // SDL_DestroyTexture(myGame->displayGame.g_pTexture2);

    }

    SDL_RenderPresent(myGame->display.g_pRenderer);
    myGame->display.g_pSurface=NULL;
}

/********************************************************************************************
PURPOSE :
Events management (input=>keyboard)
Indicates in what direction player's paddle should go when player press Up or Down arrow key
INPUT : state variable, gameObject
OUTPUT : gameIsRunning or paddle1.dy
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

/***************************************************************************
PURPOSE :
Move player's paddle and give a smooth move to player's paddle
Paddle max speed: 7 pixels per 16 ms
INPUT : gameObject
OUTPUT : paddle1.py
****************************************************************************/
void playerPaddleMove (BreakoutGame *myGame){

    if (myGame->paddle.px>myGame->paddle.dx){
        if (myGame->paddle.px>0){ // if paddle not at
                myGame->paddle.px-=PADDLE_MAX_SPEED;
                }
    }

    if (myGame->paddle.px<myGame->paddle.dx){
        if (myGame->paddle.px<SCREEN_WIDTH-PADDLE_WIDTH){ // if paddle not
            myGame->paddle.px+=PADDLE_MAX_SPEED;
            }
    }
}

/********************************************************************
PURPOSE : render the paddles
INPUT : gameObject
OUTPUT : 2 rectangles
*********************************************************************/
void renderPaddle(BreakoutGame *myGame) {

       //Paddle
        SDL_Rect rectangle;
        rectangle.x=myGame->paddle.px;//debut x
        rectangle.y=myGame->paddle.py;//debut y
        rectangle.w=PADDLE_WIDTH; //Largeur
        rectangle.h=PADDLE_HEIGHT; //Hauteur

        //Draw in texture
        SDL_SetRenderDrawColor(myGame->display.g_pRenderer,255,0,0,255); // set the color used for drawing operations

        SDL_RenderFillRect(myGame->display.g_pRenderer, &rectangle); // fill a rectangle on the current rendering target with the drawing color
}

/********************************************************************
PURPOSE : render a boundary line middle of the screen (width)
INPUT : gameObject, color (RGB)
OUTPUT : chain of espaced rectangles
*********************************************************************/
void renderBorders(BreakoutGame *myGame, int colorR, int colorG, int colorB){
/*
        int y;
        SDL_SetRenderDrawColor(myGame->display.g_pRenderer,colorR,colorG,colorB,255);

        for (y=10; y<SCREEN_HEIGHT-10; y=y+15){

        SDL_Rect Rectangle;
        Rectangle.x=SCREEN_WIDTH/2-LINE_WIDTH/2;
        Rectangle.y=y;
        Rectangle.w=LINE_WIDTH;
        Rectangle.h=LINE_HEIGHT;

        SDL_RenderFillRect(myGame->display.g_pRenderer, &Rectangle);
        }*/
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
PURPOSE : display player's score
INPUT : gameObject, fontObject
OUTPUT : 1 texture containing player's score
*********************************************************************/
void renderPlayerScore (BreakoutGame *myGame, font myFont){
/*
        char playerScoreArr [0];
        sprintf (playerScoreArr, "%i", myGame->score.player);
        //fprintf(stdout,"score player:%c%c\n", playerScoreArr[0],playerScoreArr[1]);
        SDL_Color fontColor={255,255,255};
        myGame->display.g_pSurface=TTF_RenderText_Blended(myFont.g_font, playerScoreArr, fontColor);


        if(myGame->display.g_pSurface){


                // Rectangle used to display character chain
                SDL_Rect playerScoreRect;
                playerScoreRect.x=SCREEN_WIDTH/2-100;//start point (x)
                playerScoreRect.y=SCORE_Y;// start point (y)
                playerScoreRect.w=SCORE_W; //Width
                playerScoreRect.h=SCORE_H; //Height


                 myGame->display.g_pTexture = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                 SDL_FreeSurface(myGame->display.g_pSurface);


                 if(myGame->display.g_pTexture){
                        // copy a portion of the texture to the current rendering target
                        SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTexture,NULL,&playerScoreRect);
                        SDL_DestroyTexture(myGame->display.g_pTexture);
                 }
                 else{
                        fprintf(stdout,"Failed to create texture (%s)\n",SDL_GetError());
                }

                }
        else{
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }*/
}

/********************************************************************
PURPOSE :
Procedure calling all the render functions
Give a black background to the game
INPUT : gameObject, fontObject, color (RGB)
*********************************************************************/
void renderBreakoutGame (BreakoutGame myGame, font myFont){
    renderBall (&myGame,255,255,255);
    renderPaddle(&myGame);
    renderBorders (&myGame, 255, 255, 255);
    renderPlayerScore (&myGame, myFont);

    SDL_RenderPresent(myGame.display.g_pRenderer); // update the screen with any rendering performed since the previous cal

    SDL_SetRenderDrawColor(myGame.display.g_pRenderer,0,0,0,255); // black background
    SDL_RenderClear(myGame.display.g_pRenderer);
}

/********************************************************************
PURPOSE : Check if the ball hits a window's border
INPUT : gameObject
OUTPUT : Return Right or Left or Top or Bottom or None
*********************************************************************/
enum Collision CheckCollisionBallWalls (BreakoutGame myGame){
    //check if ball hit right side
    if (myGame.ball.px >=SCREEN_WIDTH-BALL_RADIUS){
        //fprintf(stdout,"score right side\n");
        return Right;
    }
    //check if ball hit left side
    if (myGame.ball.px <=BALL_RADIUS){
        //fprintf(stdout,"score left side\n");
        return Left;
    }
    //check if ball hit top
    if (myGame.ball.py <= BALL_RADIUS){
        //fprintf(stdout,"collision on top\n");
        return Top;
    }
    //check if ball hit bottom
    if (myGame.ball.py >= SCREEN_HEIGHT-BALL_RADIUS){
        //fprintf(stdout,"collision on bottom\n");
        return Bottom;
    }

    // if no collision
    return None;
};

/********************************************************************
PURPOSE : Check if the AI or player has won.
INPUT : state variable, gameObject, fontObject
OUTPUT : gameIsRunning
*********************************************************************/

void checkVictoryConditions (int *gameIsRunning, BreakoutGame *myGame, font myFont){
    /*if (myGame->score.AI >=SCORE_TO_WIN){
        endWindow (myGame, myFont, 0);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }

    if (myGame->score.player >=SCORE_TO_WIN){
        endWindow (myGame, myFont, 1);
        SDL_Delay(4000);
        *gameIsRunning=0;
    }*/
}

/********************************************************************
PURPOSE : Reset ball position and direction
INPUT : gameObject
OUTPUT : ball.px, ball.py, ball.sx, ball.sy
*********************************************************************/
void resetBall (BreakoutGame *myGame){
    myGame->ball.px= SCREEN_WIDTH/3 + (rand () % 200);
    myGame->ball.py= SCREEN_HEIGHT/3 + (rand () % 200);
    myGame->ball.sx= (rand () % 2 + 4) + cos (rand () % 90);
    myGame->ball.sy= (rand () % 1 + 2) + cos (rand () % 90);

    if ((rand () % 2)==1) {
        myGame->ball.sx*=-1;
    }

    if ((rand () % 2)==1) {
        myGame->ball.sy*=-1;
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

/************************************************************************
PURPOSE :
Change ball direction and position according to collision events
Increment player's score and AI's score according to collision events
INPUT : gameObject
OUTPUT : ball.px, ball.py and/or ball.sy and/or (score.player or score.AI)
*************************************************************************/
void ballMovementAndScore(BreakoutGame *myGame){
    // if ball hit right wall
    if (CheckCollisionBallWalls (*myGame)== Right){
            if (myGame->ball.sx>0){
                    myGame->ball.sx=-myGame->ball.sx*BOUNCE_SPEED;
                    myGame->ball.sy*=1.15;
            }


    }

    // if ball hit left wall
    if (CheckCollisionBallWalls (*myGame)== Left)
    {
        if (myGame->ball.sx<0)
        {
                myGame->ball.sx=-myGame->ball.sx*BOUNCE_SPEED;
                myGame->ball.sy*=1.15;
        }

    }

    // if ball hit Top
     if (CheckCollisionBallWalls (*myGame)== Top)
     {
         myGame->ball.sy=-myGame->ball.sy*BOUNCE_SPEED;
     }

    // if ball hit Bottom
    if (CheckCollisionBallWalls (*myGame)== Bottom)
    {
        //myGame->score.player+=1;
        resetBall (myGame);
    }

    // if ball hit a racket
    if (CheckCollisionBallPaddles (*myGame)== True)
    {
        if (myGame->ball.sy>0)
        {
            myGame->ball.sx*=1.15;
            myGame->ball.sy=-myGame->ball.sy*BOUNCE_SPEED;

        }

    }

    //ball speed cap
    if (myGame->ball.sx>BALL_RADIUS-2){
            myGame->ball.sx=BALL_RADIUS-2;
            }

    if (myGame->ball.sy<-BALL_RADIUS-2){
          myGame->ball.sy=-BALL_RADIUS-2;
          }

    //ball movement
    myGame->ball.px+=myGame->ball.sx;
    myGame->ball.py+=myGame->ball.sy;

}

/********************************************************************
PURPOSE :
FrameRate management
Capped at 60fps(1 frame/16ms => (1/60fps)*1000 =16.67ms)
INPUT : FrameLimit
OUTPUT : Delay=>frameLimit
*********************************************************************/
void delay(unsigned int frameLimit){

    unsigned int ticks = SDL_GetTicks();

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
PURPOSE : create and display a window displaying the winner
INPUT : gameObject, fontObject
OUTPUT : window with 1 texture containing the name of the winner.
*********************************************************************/
void endWindow (BreakoutGame *myGame, font myFont, int winner){
SDL_Color fontColor={255,255,255};

    SDL_Rect Window1; //Rectangle used to display character chain
    Window1.x=MAIN_TEXT_X;//start point (x)
    Window1.y=MAIN_TEXT_Y;// start point (y)
    Window1.w=MAIN_TEXT_W; //Width
    Window1.h=MAIN_TEXT_H; //Height

    if (myGame->display.g_pTexture3==NULL)
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
                     myGame->display.g_pTexture3 = SDL_CreateTextureFromSurface(myGame->display.g_pRenderer,myGame->display.g_pSurface);
                     SDL_FreeSurface(myGame->display.g_pSurface);
        }
        else
        {
            fprintf(stdout,"Failed to create surface (%s)\n",SDL_GetError());
        }
    }

    if(myGame->display.g_pTexture3)
    {
            //  copy a portion of the texture to the current rendering target
            SDL_RenderCopy(myGame->display.g_pRenderer,myGame->display.g_pTexture3,NULL,&Window1);
            SDL_RenderPresent(myGame->display.g_pRenderer);
    }
}

/********************************************************************
PURPOSE :
Destroy the rendering context, the texture and the surface
Free RGB surface
INPUT: GameObject
*********************************************************************/
void destroy(BreakoutGame *myGame){

    //Destroy render
    if(myGame->display.g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->display.g_pRenderer);

    //Destroy surface
    if(myGame->display.g_pSurface!=NULL)
        SDL_FreeSurface(myGame->display.g_pSurface);

    //Destroy texture
    if(myGame->display.g_pTexture1!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTexture1);

    if(myGame->display.g_pTexture2!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTexture2);

    if(myGame->display.g_pTexture3!=NULL)
        SDL_DestroyTexture(myGame->display.g_pTexture3);

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
