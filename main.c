
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

    while (introIsRunning){
        handleIntroEvents(&introIsRunning,&gameIsRunning,&myGame);
        introWindow(&myGame, myFont);
    }

    if(myGame.display.g_pTextureText1!=NULL)
        SDL_DestroyTexture(myGame.display.g_pTextureText1);

    if(myGame.display.g_pTextureText2!=NULL)
        SDL_DestroyTexture(myGame.display.g_pTextureText2);

   while (gameIsRunning) {
            handleGameEvents(&gameIsRunning,&myGame);
            playerPaddleMove (&myGame);
            ballMovementAndScore(&myGame);
            renderBreakoutGame (myGame, myFont);
            checkVictoryConditions (&gameIsRunning, &myGame, myFont);

            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16; //60 fps cap

    }

        // free pointer
        destroy(&myGame);
        releaseFont (&myFont);

        //free memory
        TTF_Quit();
        SDL_Quit();


    return EXIT_SUCCESS;
}


/*
void cutBitmapTexture(game *myGame,gameState state){

        SDL_Rect rectangleDest;
        SDL_Rect rectangleSource;


        myGame->g_surface = IMG_Load("./assets/animate-alpha.png");//Chargement de l'image png

        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    rectangleSource.x=128*(int)((SDL_GetTicks()/100)%6);
                    rectangleSource.y=0;
                    rectangleSource.w=128;//1 image = w:128 et h:82
                    rectangleSource.h=82;

                    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                    //Définition du rectangle dest pour dessiner Bitmap
                    rectangleDest.x=0;//debut x
                    rectangleDest.y=0;//debut y
                    rectangleDest.w=rectangleSource.w; //Largeur
                    rectangleDest.h=rectangleSource.h; //Hauteur

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);
                    SDL_RenderClear(myGame->g_pRenderer);

                    if(state.right)
                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest); // Copie du sprite grâce au SDL_Renderer
                    if(state.left)
                        SDL_RenderCopyEx(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest,0,0,SDL_FLIP_HORIZONTAL);

                    //SDL_FLIP_NONE
                    //SDL_FLIP_HORIZONTAL
                    //SDL_FLIP_VERTICAL



                    SDL_RenderPresent(myGame->g_pRenderer); // Affichage

                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }


    destroyTexture(myGame);

}
*/










