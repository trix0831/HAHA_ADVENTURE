#pragma once
#include "LTexture.h"
#include "Collision.h"
#include <iostream>
#include <vector>

extern const int BOAT_WIDTH = 128;
extern const int BOAT_HEIGHT = 64;
extern const int SPEED_LIMIT = 12;

//The dimensions of the level
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

const int boatFrame = 3;

extern int paddleTimes;
extern bool paddleNotEnd;


//Maximum axis velocity of the dot
static const double BOAT_VEL = 2;
static const double RESISTANCE = 0.075;

class Boat
{
public:
    //The dimensions of the dot
    static const int width = BOAT_WIDTH;
    static const int height = BOAT_HEIGHT;


    //Initializes the variables
    Boat();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    //Moves the dot
    void move();

    //Shows the dot on the screen relative to the camera
    void render(int camX, int camY, int ani);

    void changeBoatType();

    void initializeCurrentBoat();

    void defaultBoat();

    void bounce();

    bool getBoatType() {
        return boatType;
    }

    bool changePos(int x, int y);

    //Position accessors
    double getPosX();
    double getPosY();

    std::vector<SDL_Rect>& getColliders();

private:
    //The X and Y offsets of the dot
    double mPosX, mPosY;

    //The velocity of the dot
    double mVelX, mVelY;

    //up = 0, down = 1
    bool boatType, boatDir;

    //index: boatType, boatDir, animation
    SDL_Rect currentBoat[2][2][3];

    std::vector<SDL_Rect> mColliders;

    void shiftColliders();

    LTexture gBoatTexture;
};

std::vector<SDL_Rect>& Boat::getColliders() {
    return mColliders;
}

Boat::Boat()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;


    if (!gBoatTexture.loadFromFile("image/BOAT_ALL.bmp"))
    {
        printf("Failed to load dot texture!\n");
    }

    boatType = false;
    boatDir = false;

    initializeCurrentBoat();
}

void Boat::handleEvent(SDL_Event& e)
{
    //If a key was pressed
//    std::cout << "posX = "  << mPosX << std::endl;
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE) {
        this->changeBoatType();
        std::cout << "boat type changed to " << (int)boatType << std::endl;
    }

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_p) {
        mVelX = 0;
        mVelY = 0;
        std::cout << "stop boat" << std::endl;
        std::cout << "boat speed : (" << mVelX << ", " << mVelY << ")" << std::endl;
    }

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            if (mVelY > -SPEED_LIMIT)
            {
                mVelY -= BOAT_VEL;
                if (mVelX < SPEED_LIMIT) {
                    mVelX += BOAT_VEL;
                }
                boatDir = 0;
                std::cout << "up" << std::endl;
                //                std::cout << "boat speed : (" << mVelX << ", " << mVelY << ")" << std::endl;
            }
            break;
        case SDLK_RIGHT:
            if (mVelY < SPEED_LIMIT)
            {
                mVelY += BOAT_VEL;
                if (mVelX < SPEED_LIMIT) {
                    mVelX += BOAT_VEL;
                }
                boatDir = 1;
                std::cout << "down" << std::endl;
                //                std::cout << "boat speed : (" << mVelX << ", " << mVelY << ")" << std::endl;
            }
            break;
        case SDLK_DOWN:
            if (mVelX > -SPEED_LIMIT)
            {
                mVelX -= BOAT_VEL;
                std::cout << "left" << std::endl;
                //                std::cout << "boat speed : (" << mVelX << ", " << mVelY << ")" << std::endl;
            }
            break;
            //        case SDLK_RIGHT:
            //            if (mVelX < SPEED_LIMIT)
            //            {
            //                mVelX += BOAT_VEL;
            //                std::cout << "right" << std::endl;
            //                std::cout << "boat speed : (" << mVelX << ", " << mVelY << ")" << std::endl;
            //            }
            //            break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //        //adjust the velocity
        /*switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY += BOAT_VEL; break;
        case SDLK_DOWN: mVelY -= BOAT_VEL; break;
        case SDLK_LEFT: mVelX += BOAT_VEL; break;
        case SDLK_RIGHT: mVelX -= BOAT_VEL; break;
        }*/
    }
}

void Boat::move()
{
    //Move the dot left or right

    mPosX += mVelX;
    shiftColliders();

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + width > LEVEL_WIDTH))
    {
        //Move back
        mPosX -= mVelX;
        mVelX = 0;
        shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
    shiftColliders();

    //If the dot went too far up or down
    if ((mPosY < 0) || (mPosY + height > LEVEL_HEIGHT))
    {
        //Move back
        mPosY -= mVelY;
        mVelY = 0;
        shiftColliders();
    }

    if (mVelX > 0) {
        mVelX -= RESISTANCE;
        shiftColliders();
    }
    else if (mVelX < 0) {
        mVelX += RESISTANCE;
        shiftColliders();
    }
    if (mVelY > 0) {
        mVelY -= RESISTANCE;
        shiftColliders();
    }
    else if (mVelY < 0) {
        mVelY += RESISTANCE;
        shiftColliders();
    }

}

void Boat::render(int camX, int camY, int ani)
{
    //Show the dot relative to the camera
    gBoatTexture.render(mPosX - camX, mPosY - camY, &currentBoat[boatType][boatDir][ani]);
}

void Boat::shiftColliders()
{
    //The row offset
    int r = 0;

    //Go through the dot's collision boxes
    for (int set = 0; set < mColliders.size(); ++set)
    {
        //Center the collision box
        mColliders[set].x = mPosX + (BOAT_WIDTH - mColliders[set].w) / 2;

        //Set the collision box at its row offset
        mColliders[set].y = mPosY + r;

        //Move the row offset down the height of the collision box
        r += mColliders[set].h;
    }
}

void Boat::changeBoatType() {
    boatType = !boatType;
}

void Boat::bounce() {
    mVelX = -mVelX;
    mVelY = -mVelY;
}

void Boat::initializeCurrentBoat() {

    //type = 0, dir = up
    currentBoat[0][0][0].x = 3 * BOAT_WIDTH;
    currentBoat[0][0][0].y = 0;
    currentBoat[0][0][0].w = BOAT_WIDTH;
    currentBoat[0][0][0].h = BOAT_HEIGHT;

    currentBoat[0][0][1].x = 3 * BOAT_WIDTH;
    currentBoat[0][0][1].y = BOAT_HEIGHT;
    currentBoat[0][0][1].w = BOAT_WIDTH;
    currentBoat[0][0][1].h = BOAT_HEIGHT;

    currentBoat[0][0][2].x = 3 * BOAT_WIDTH;
    currentBoat[0][0][2].y = 2 * BOAT_HEIGHT;
    currentBoat[0][0][2].w = BOAT_WIDTH;
    currentBoat[0][0][2].h = BOAT_HEIGHT;

    //type = 0, dir = down
    currentBoat[0][1][0].x = BOAT_WIDTH;
    currentBoat[0][1][0].y = 0;
    currentBoat[0][1][0].w = BOAT_WIDTH;
    currentBoat[0][1][0].h = BOAT_HEIGHT;

    currentBoat[0][1][1].x = BOAT_WIDTH;
    currentBoat[0][1][1].y = BOAT_HEIGHT;
    currentBoat[0][1][1].w = BOAT_WIDTH;
    currentBoat[0][1][1].h = BOAT_HEIGHT;

    currentBoat[0][1][2].x = BOAT_WIDTH;
    currentBoat[0][1][2].y = 2 * BOAT_HEIGHT;
    currentBoat[0][1][2].w = BOAT_WIDTH;
    currentBoat[0][1][2].h = BOAT_HEIGHT;

    //type = 1, dir = up
    currentBoat[1][0][0].x = 2 * BOAT_WIDTH;
    currentBoat[1][0][0].y = 0;
    currentBoat[1][0][0].w = BOAT_WIDTH;
    currentBoat[1][0][0].h = BOAT_HEIGHT;

    currentBoat[1][0][1].x = 2 * BOAT_WIDTH;
    currentBoat[1][0][1].y = BOAT_HEIGHT;
    currentBoat[1][0][1].w = BOAT_WIDTH;
    currentBoat[1][0][1].h = BOAT_HEIGHT;

    currentBoat[1][0][2].x = 2 * BOAT_WIDTH;
    currentBoat[1][0][2].y = 2 * BOAT_HEIGHT;
    currentBoat[1][0][2].w = BOAT_WIDTH;
    currentBoat[1][0][2].h = BOAT_HEIGHT;

    //type = 1, dir = down
    currentBoat[1][1][0].x = 0;
    currentBoat[1][1][0].y = 0;
    currentBoat[1][1][0].w = BOAT_WIDTH;
    currentBoat[1][1][0].h = BOAT_HEIGHT;

    currentBoat[1][1][1].x = 0;
    currentBoat[1][1][1].y = BOAT_HEIGHT;
    currentBoat[1][1][1].w = BOAT_WIDTH;
    currentBoat[1][1][1].h = BOAT_HEIGHT;

    currentBoat[1][1][2].x = 0;
    currentBoat[1][1][2].y = 2 * BOAT_HEIGHT;
    currentBoat[1][1][2].w = BOAT_WIDTH;
    currentBoat[1][1][2].h = BOAT_HEIGHT;

    mColliders.resize(1);
    mColliders[0].w = BOAT_WIDTH;
    mColliders[0].h = BOAT_HEIGHT;

}


void defaultBoat() {

};

double Boat::getPosX()
{
    return mPosX;
}

double Boat::getPosY()
{
    return mPosY;
}

bool Boat::changePos(int x,int y) {
    mPosX = x;
    mPosY = y;
    return true;
}