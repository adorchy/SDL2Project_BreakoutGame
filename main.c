
#include "Breakout.h"

int main(int argc, char *argv[])
{


    BreakoutGame myGame;
    int gameIsRunning;
    int introIsRunning;
    font myFont;
    unsigned int frameLimit = SDL_GetTicks() + 16;

    //Init myGame
    initBreakoutGame (&myGame);
    initFont(&myFont);
    //Init SDL
    if(initSDL("BreakOut",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame)){
            gameIsRunning=1;
            introIsRunning=1;
            srand (time(NULL)); // initialize random seed
    }else{
            fprintf(stdout,"Unable to initialize SDL (%s)\n",SDL_GetError());
            return EXIT_FAILURE;
    }

    loadIntroTextures (&myGame, myFont);

    while (introIsRunning){
        handleIntroEvents(&introIsRunning,&gameIsRunning,&myGame);
        displayIntroWindow(&myGame);
    }

    destroyIntroTextures (&myGame);
    loadGameTextures (&myGame);

   while (gameIsRunning) {
            handleGameEvents(&gameIsRunning,&myGame);
            playerPaddleMove (&myGame);
            handleCollisionBallWalls (&myGame);
            handleCollisionBallBrick (&myGame);
            ballMovement(&myGame);
            renderBreakoutGame (myGame, myFont);
            checkVictoryConditions (&gameIsRunning, &myGame, myFont);

            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16; //60 fps cap

    }

        // free pointer
        destroyGame(&myGame);
        releaseFont (&myFont);

        //free memory
        TTF_Quit();
        SDL_Quit();


    return EXIT_SUCCESS;
}













