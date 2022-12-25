//Using SDL, SDL_image, standard IO, vectors, and strings
#include<SDL.h>
#include<SDL_image.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include "boat.h"
#include "LTexture.h"
#include "Fish.h"
#include "Gate.h"
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
bool ishard = false;

std::queue< std::vector<SDL_Rect> > Colliders;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


//Scene textures
LTexture gBGTexture;
LTexture STTexture;
LTexture SELTexture;

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
        }
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

void restart(Boat &player1, Fish &fish1, Fish &fish2, Gate &gate1) {
    player1.changePos(0, SCREEN_HEIGHT / 2-BOAT_HEIGHT/2);
    fish1.newPOS(1300, 100);
    fish2.newPOS(1900, 100);
    gate1.newPOS(2300, 0);

    std::cout << "restart" << std::endl;
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

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //random seed
    time_t t;
    srand((unsigned)time(&t));
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //start page
        if (!loadStartPage()) {
            printf("Failed to load start!\n");
        }
        else {
            STTexture.render(0, 0);
            SDL_RenderPresent(gRenderer);

            SDL_Event a;
            bool start = false;

            while(!start){
                while (SDL_PollEvent(&a) != 0)
                {
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

            while (!selected) {
                while (SDL_PollEvent(&b) != 0)
                {
                    if (b.type == SDL_KEYDOWN && b.key.repeat == 0 && b.key.keysym.sym == SDLK_h) {
                        ishard = true;
                        selected = true;
                    }

                    if (b.type == SDL_KEYDOWN && b.key.repeat == 0 && b.key.keysym.sym == SDLK_e) {
                        ishard = false;
                        selected = true;
                    }
                }
            }
        }

        //SDL_DestroyWindow(gWindow);
        //gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Current animation frame
            int frame = 0;
            int count = 0;

            //The dot that will be moving around on the screen
//            Fish fish1(0,0);

            //generating random fish
//            while (curx < LEVEL_WIDTH){
//                Fish tmpfish(curx+rand()%200, rand()%LEVEL_HEIGHT);
//                Colliders.push(tmpfish.getColliders());
//            }


            Boat player1;
            Fish fish1(1300, 140, "image/pufferfish(1).bmp");
            fish1.shiftColliders();
            //Gate gate1(1800, 0, "image/gate.bmp")
            Fish fish2(1900,80,"image/pufferfish(1).bmp");
            
            Gate gate1(2200, 0, "image/andGate.png");
            gate1.shiftColliders();
            //            fishVector.push_back(fish2);


                        //The camera area
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

            //While application is running
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

                    //Handle input for the dot
                    player1.handleEvent(e);

                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                        paddleNotEnd = true;
                    }

                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_r ) {
                        restart(player1, fish1, fish2, gate1);
                    }

                    if (player1.getPosX() > LEVEL_WIDTH - BOAT_WIDTH-10) {
                        restart(player1, fish1, fish2, gate1);
                    }
                }

                //Move the dot
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

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render background
                gBGTexture.render(0, 0, &camera);

                //Render player
                player1.render(camera.x, camera.y, frame);
                fish1.render(camera.x, camera.y);
                if (ishard)
                fish2.render(camera.x, camera.y);
                gate1.render(camera.x, camera.y);
                                //Render fish
                //                for (int i = 0; i<fishVector.size(); ++i){
                //                    Fish &curfish = fishVector[i];
                //                    if (curfish.getPosX() - player1.getPosX() < 3*(SCREEN_WIDTH/4)+60 && curfish.checkAppeared() == 0){
                //                        curfish.switchAppeared();
                //                    }else if (player1.getPosX() - curfish.getPosX() > (SCREEN_WIDTH/4)+60){
                //                        curfish.switchAppeared();
                //                        curfish.newPOS(player1.getPosX()+SCREEN_WIDTH, rand()%SCREEN_HEIGHT);
                //                    }
                //                    if (checkCollision(player1.getColliders(), curfish.getColliders()) == 1){
                //                        player1.bounce();
                //                    }
                //                }
                if (checkCollision(player1.getColliders(), fish1.getColliders()) == 1) {
                    player1.bounce();
                }
                if (checkCollision(player1.getColliders(), fish2.getColliders()) == 1 && ishard) {
                    player1.bounce();
                }
                if (checkCollision(player1.getColliders(), gate1.getColliders()) == 1) {
                    player1.bounce();
                }
                if (checkState(player1, gate1) == 1){
                    player1.bounce();
                }
                if (player1.getPosX() - fish1.getPosX() > 60 + SCREEN_WIDTH / 4) {
                    fish1.newPOS(player1.getPosX() + SCREEN_WIDTH, rand() % 240);
                    fish1.shiftColliders();
                }
                if (player1.getPosX() - fish2.getPosX() > 60 + SCREEN_WIDTH / 4 and ishard) {
                    fish2.newPOS(player1.getPosX() + SCREEN_WIDTH, rand() % 240);
                    fish2.shiftColliders();
                }
                if (player1.getPosX() - gate1.getPosX() > 60 + SCREEN_WIDTH / 4) {
                    gate1.newPOS(player1.getPosX() + SCREEN_WIDTH, 0);
                    gate1.shiftColliders();
                }

                //                if (int(player1.getPosX())%2000 > 3000  && fish1.checkAppeared() == 0){
                //                    fish1.newPOS(player1.getPosX()+SCREEN_WIDTH, rand()%SCREEN_HEIGHT);
                //                    fish1.shiftColliders();
                //                    fish1.switchAppeared();
                //                }else if(int(player1.getPosX())%2000 > 30) {
                //                    fish1.switchAppeared();
                //                }
                //
                //                if (int(player1.getPosX())%2000 > 3000 && fish2.checkAppeared() == 0){
                //                    fish2.newPOS(player1.getPosX()+SCREEN_WIDTH, rand()%LEVEL_HEIGHT);
                //                    fish2.shiftColliders();
                //                    fish2.switchAppeared();
                //                }else if(int(player1.getPosX())%2000 > 1030) {
                //                    fish2.switchAppeared();
                //                }


                std::cout << fish1.getPosX() << std::endl;
                //                std::cout << player1.getPosX() << std::endl;


                //                fish1.render();

                                //Update screen
                SDL_RenderPresent(gRenderer);

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
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
