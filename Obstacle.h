//
//  Obstacle.h
//  Dec16BOAT
//
//  Created by hubert on 2022/12/25.
//

#ifndef Obstacle_h
#define Obstacle_h

#include <vector>
#include "LTexture.h"
#include <string>

class Obstacle
{
public:
    //The dimensions of the obstacle
//    static const int width = FISH_WIDTH;
//    static const int height = FISH_HEIGHT;


    //Initializes the variables
    Obstacle(double x, double y, int width, int height,std::string imgPath);

    //Takes key presses and adjusts the dot's velocity
    // void handleEvent(SDL_Event& e);

    //Moves the dot
    // void move();

    //Shows the dot on the screen relative to the camera
    void render(int camX, int camY);

    // void changeBoatType();

    
    void newPOS(int x, int y);
    // void defaultBoat();

    //Position accessors
    double getPosX();
    double getPosY();
    void shiftColliders();
    bool checkAppeared();
    void switchAppeared();
    virtual void initCollider() {};
    std::vector<SDL_Rect>& getColliders();
    
protected:
    std::vector<SDL_Rect> mColliders;
    
    
private:
    //The X and Y offsets of the dot
    double mPosX, mPosY;
    bool appeared;
    int width, height;
    //The velocity of the dot
    double mVelX, mVelY;

    //up = 0, down = 1

    //index: boatType, boatDir, animation

    
    LTexture gObsTexture;
};

bool Obstacle::checkAppeared(){
    return appeared;
}

void Obstacle::switchAppeared(){
    appeared = !appeared;
}

void Obstacle::newPOS(int x, int y){
    mPosX = x;
    mPosY = y;
}

Obstacle::Obstacle(double x, double y, int width, int height, std::string imgPath)
{
    appeared = 0;

    if (!gObsTexture.loadFromFile(imgPath))
    {
        printf("Failed to load dot texture!\n");
    }

    // boatType = false;
    // boatDir = false;
    //Initialize the offsets
    mPosX = x;
    mPosY = y;
    width = width;
    height = height;

    //Create the necessary SDL_Rects
    
}


void Obstacle::render(int camX, int camY)
{
    //Show the dot relative to the camera
    gObsTexture.render(mPosX - camX, mPosY - camY);
}

std::vector<SDL_Rect>& Obstacle::getColliders()
{
    return mColliders;
}

void Obstacle::shiftColliders() {
    //The row offset
    int r = 0;

    //Go through the dot's collision boxes
    for ( int set = 0; set < mColliders.size(); ++set )
    {
        //Center the collision box
        mColliders[ set ].x = mPosX + ( width - mColliders[ set ].w ) / 2;

        //Set the collision box at its row offset
        mColliders[ set ].y = mPosY + r;

        //Move the row offset down the height of the collision box
        r += mColliders[ set ].h;
    }

}

double Obstacle::getPosX()
{
    return mPosX;
}

double Obstacle::getPosY()
{
    return mPosY;
}

#endif /* Obstacle_h */
