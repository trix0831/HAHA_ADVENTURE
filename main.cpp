//Using SDL, SDL_image, standard IO, vectors, and strings
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include "boat.h"
#include "LTexture.h"
#include "Fish.h"
#include "Gate.h"
#include "LTimer.h"
#include "Collision.h"
#include "checkState.h"
#include <stdlib.h>
#include <time.h>


//The dimensions of the level
const int LEVEL_WIDTH = 8000 * 2;
const int LEVEL_HEIGHT = 300;

//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
//1280 300

int paddleTimes = 0;
bool paddleNotEnd = true;

//game mode
bool isHard = false;

LTimer timer;

std::queue< std::vector<SDL_Rect> > Colliders;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

bool gameEnded = false;
bool gameStarted = false;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


//Scene textures
LTexture gBGTexture;
LTexture STTexture;
LTexture SELTexture;
LTexture ENDTexture;

//Globally used font
TTF_Font* gFont = NULL;

//Scene textures
LTexture gTimeTextTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;

//std::vector<Fish> fishVector;
//std::vector<Gate> gateVector;




bool init()
{
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("HAHA ADENVENTURE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }

            //Initialize SDL_ttf
            if (TTF_Init() == -1)
            {
                printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
            }
        }
    }

    gFont = TTF_OpenFont("image/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //Set text color as black
        SDL_Color textColor = { 250, 240, 71, 0 };

        ////Load stop prompt texture
        //if (!gStartPromptTexture.loadFromRenderedText("Press S to Start or Stop the Timer", textColor))
        //{
        //    printf("Unable to render start/stop prompt texture!\n");
        //    success = false;
        //}

        ////Load pause prompt texture
        //if (!gPausePromptTexture.loadFromRenderedText("Press P to Pause or Unpause the Timer", textColor))
        //{
        //    printf("Unable to render pause/unpause prompt texture!\n");
        //    success = false;
        //}
    }

    return success;
}

bool loadStartPage() {
    //Loading success flag
    bool success = true;

    //Load background texture
    if (!STTexture.loadFromFile("image/initialPage.bmp"))
    {
        printf("Failed to load start texture!\n");
        success = false;
    }

    return success;
}

bool loadSelectionPage() {
    bool success = true;

    //Load background texture
    if (!SELTexture.loadFromFile("image/difficultySelection.bmp"))
    {
        printf("Failed to load selection texture!\n");
        success = false;
    }

    return success;
}

void loadEndPic() {
    //Load background texture
    if (!ENDTexture.loadFromFile("image/end.bmp"))
    {
        printf("Failed to load end texture!\n");
    }
}

void restart(Boat &player1, Fish &fish1, Fish &fish2, Gate &gate1, Gate &tmpgate, Fish &tmpfish) {
    player1.changePos(0, SCREEN_HEIGHT / 2-BOAT_HEIGHT/2);
    fish1.newPOS(1300, 100);
    fish1.shiftColliders();
    fish2.newPOS(1900, 100);
    fish2.shiftColliders();
    gate1.newPOS(2300, 0);
    gate1.shiftColliders();
    tmpgate.newPOS(13000, 0);
    tmpgate.shiftColliders();
    tmpfish.newPOS(13000, 0);
    tmpfish.shiftColliders();

    timer.stop();
    gameEnded = false;
    gameStarted = false;

//    std::cout << "restart" << std::endl;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load background texture
    if (!gBGTexture.loadFromFile("image/river7.png"))
    {
        printf("Failed to load background texture!\n");
        success = false;
    }

    return success;
}

void close()
{
    //Free loaded images
    gBGTexture.free();

    //Free loaded images
    gTimeTextTexture.free();
    gStartPromptTexture.free();
    gPausePromptTexture.free();

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //random seed
    time_t t;
    srand((unsigned)time(&t));

    bool quit = false;
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        loadEndPic();
        //start page
        if (!loadStartPage()) {
            printf("Failed to load start!\n");
        }
        else {
            STTexture.render(0, 0);
            SDL_RenderPresent(gRenderer);

            SDL_Event a;
            bool start = false;

            while(!start && !quit){
                while (SDL_PollEvent(&a) != 0)
                {
                    if (a.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    if (a.type == SDL_KEYDOWN && a.key.repeat == 0 && a.key.keysym.sym == SDLK_s) {
                        start = true;
                    }
                }
            }
        }

        if (!loadSelectionPage()) {
            printf("Failed to load selection!\n");
        }
        else {
            SELTexture.render(0, 0);
            SDL_RenderPresent(gRenderer);

            SDL_Event b;
            bool selected = false;

            while (!selected && !quit) {
                while (SDL_PollEvent(&b) != 0)
                {
                    if (b.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    if (b.type == SDL_KEYDOWN && b.key.repeat == 0 && b.key.keysym.sym == SDLK_h) {
                        isHard = true;
                        selected = true;
                    }

                    if (b.type == SDL_KEYDOWN && b.key.repeat == 0 && b.key.keysym.sym == SDLK_e) {
                        isHard = false;
                        selected = true;
                    }
                }
            }
        }

        SCREEN_WIDTH = 1024;
        SCREEN_HEIGHT = 300;

        SDL_SetWindowSize(gWindow, SCREEN_WIDTH,SCREEN_HEIGHT);

        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Main loop flag

            std::stringstream timeText;

            //Set text color as black
            SDL_Color textColor = { 240, 250, 71, 255 };

            //Event handler
            SDL_Event e;

            //Current animation frame
            int frame = 0;
            int count = 0;




            Boat player1;
            Fish fish1(1300, 140, "image/pufferfish(1).bmp");
            fish1.shiftColliders();
            //Gate gate1(1800, 0, "image/gate.bmp")
            Fish fish2(1900,80,"image/pufferfish(1).bmp");
            fish2.shiftColliders();
            
            Gate gate1(2200, 0, "image/MERGEgates (1).png");
            gate1.shiftColliders();
            
            Gate tmpgate(13200, 0, "image/MERGEgates (1).png");
            tmpgate.shiftColliders();
            
            Fish tmpfish(13000,140, "image/pufferfish(1).bmp");
            tmpfish.shiftColliders();
            
            //            fishVector.push_back(fish2);


                        //The camera area
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };



            //While application is running
            Uint32 scoreTime = 1;
            while (!quit)
            {


                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    if (e.type == SDL_KEYDOWN && !gameStarted && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                        timer.start();
                        gameStarted = true;
                    }


                    //Handle input for the dot
                    player1.handleEvent(e);

                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                        paddleNotEnd = true;
                    }

                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_r ) {
                        restart(player1, fish1, fish2, gate1, tmpgate, tmpfish);
                    }

                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_w) {
                        player1.changePos(LEVEL_WIDTH - BOAT_WIDTH - 5, SCREEN_HEIGHT / 2 - BOAT_HEIGHT / 2);

                    }

                    if (player1.getPosX() > LEVEL_WIDTH - BOAT_WIDTH-10 && gameEnded == false) {
                        //restart(player1, fish1, fish2, gate1, tmpgate, tmpfish);
                        std::cout << "hi" << std::endl;
                        scoreTime = timer.getTicks();
                        std::cout << scoreTime << std::endl;
                        timer.stop();
                        gameEnded = true;
                    }


                }



                //Move the dot
                if(gameStarted)
                player1.move();

                //Center the camera over the dot
                camera.x = (int)((player1.getPosX() + Boat::width / 2) - SCREEN_WIDTH / 4);
                camera.y = (int)((player1.getPosY() + Boat::height / 2) - SCREEN_HEIGHT / 4);

                //Keep the camera in bounds
                if (camera.x < 0)
                {
                    camera.x = 0;
                }
                if (camera.y < 0)
                {
                    camera.y = 0;
                }
                if (camera.x > LEVEL_WIDTH - camera.w)
                {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if (camera.y > LEVEL_HEIGHT - camera.h)
                {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }

                if (checkCollision(player1.getColliders(), fish1.getColliders()) == 1) {
                    player1.bounce();
                }
                if (checkCollision(player1.getColliders(), fish2.getColliders()) == 1 && isHard) {
                    player1.bounce();
                }
                if (checkCollision(player1.getColliders(), tmpfish.getColliders()) == 1) {
                    player1.bounce();
                }

                if (checkCollision(player1.getColliders(), tmpgate.getColliders()) == 1) {
                    player1.bounce();
                }

                if (checkState(player1, gate1) == 0){
                    player1.bounce();
                }
                if (checkCollision(player1.getColliders(), gate1.getColliders()) == 1) {
                    player1.bounce();
                }
                if (player1.getPosX() - fish1.getPosX() > 60 + SCREEN_WIDTH / 4) {
                    tmpfish.newPOS(fish1.getPosX(), fish1.getPosY());
                    tmpfish.shiftColliders();
                    fish1.newPOS(player1.getPosX() + SCREEN_WIDTH, rand() % 240);
                    fish1.shiftColliders();
                }
                if (player1.getPosX() - fish2.getPosX() > 60 + SCREEN_WIDTH / 4 and isHard) {
                    tmpfish.newPOS(fish2.getPosX(), fish2.getPosY());
                    tmpfish.shiftColliders();
                    fish2.newPOS(player1.getPosX() + SCREEN_WIDTH, rand() % 240);
                    fish2.shiftColliders();
                }
                if (player1.getPosX() - gate1.getPosX() > 60 + SCREEN_WIDTH / 4) {
                    tmpgate.assignGate(gate1.getState(), gate1.getNumber());
                    tmpgate.newPOS(gate1.getPosX(),gate1.getPosY());
                    tmpgate.shiftColliders();
                    gate1.newPOS(player1.getPosX() + SCREEN_WIDTH, 0);
                    do{
                        gate1.assignGate(rand()%5, rand()%2);
                    }while((gate1.getState() == 0 && gate1.getNumber() == 0) || (gate1.getState() == 4 && gate1.getNumber() == 1));
//                    std::cout << "state:"  <<gate1.getState() << " number:" << gate1.getNumber() << std::endl;
                    gate1.shiftColliders();
                }


                

                //Clear screen
                SDL_RenderClear(gRenderer);
                //Set text to be rendered
                if (gameEnded == 0) {
                    timeText.str("");
                    timeText << "Seconds since start time " << (timer.getTicks() / 1000.f);
                    //Render text
                    if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                    {
                        printf("Unable to render time texture!\n");
                    }
                }
                else {
                    timeText.str("");
                    timeText << "final time:  " << (scoreTime / 1000.f);
                    if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                    {
                        printf("Unable to render time texture!\n");
                    }
                    
                }
                //LEVEL_WIDTH - SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 90

                //Render textures
                                //Render background
                gBGTexture.render(0, 0, &camera);

                //Render player
                if(gameEnded)
                ENDTexture.render(362, 60);
                player1.render(camera.x, camera.y, frame);
                fish1.render(camera.x, camera.y);
                if (isHard) {
                    fish2.render(camera.x, camera.y);
                }
                gate1.render(camera.x, camera.y);
                tmpfish.render(camera.x, camera.y);
                tmpgate.render(camera.x, camera.y);
                gTimeTextTexture.render((SCREEN_WIDTH - gStartPromptTexture.getWidth()) / 2, 0);

                //Go to next frame
                if (paddleNotEnd)
                    count++;

                if (count == 10)
                {
                    frame++;
                    count = 0;
                }

                //Cycle animation
                if (frame == boatFrame)
                {
                    frame = 0;
                    if (paddleTimes == 2)
                    {
                        paddleNotEnd = false;
                        paddleTimes = 0;
                        frame = 1;
                    }
                    else
                    {
                        paddleTimes += 1;
                    }
                }

                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
