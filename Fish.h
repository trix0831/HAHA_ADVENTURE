#pragma once
#include "LTexture.h"
#include "Obstacle.h"
#include <iostream>
#include <vector>

const int FISH_WIDTH = 60;
const int FISH_HEIGHT = 60;

//The dimensions of the level
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

// const int boatFrame = 3;

// extern int paddleTimes;
// extern bool paddleNotEnd;


//Maximum axis velocity of the dot
// static const double BOAT_VEL = 0.5;

class Fish : public Obstacle
{
public:
	//The dimensions of the dot
	static const int width = FISH_WIDTH;
	static const int height = FISH_HEIGHT;


	//Initializes the variables
	Fish(double x, double y, std::string imgPath);
	void initCollider() {
		mColliders.resize(11);


		//type = 0, dir = up
		mColliders[0].w = 6 * 3;
		mColliders[0].h = 1 * 3;

		mColliders[1].w = 10 * 3;
		mColliders[1].h = 1 * 3;

		mColliders[2].w = 14 * 3;
		mColliders[2].h = 1 * 3;

		mColliders[3].w = 16 * 3;
		mColliders[3].h = 2 * 3;

		mColliders[4].w = 18 * 3;
		mColliders[4].h = 2 * 3;

		mColliders[5].w = 20 * 3;
		mColliders[5].h = 6 * 3;

		mColliders[6].w = 18 * 3;
		mColliders[6].h = 2 * 3;

		mColliders[7].w = 16 * 3;
		mColliders[7].h = 2 * 3;

		mColliders[8].w = 14 * 3;
		mColliders[8].h = 1 * 3;

		mColliders[9].w = 10 * 3;
		mColliders[9].h = 1 * 3;

		mColliders[10].w = 6 * 3;
		mColliders[10].h = 1 * 3;
	};

	void shiftColliders() {
		//The row offset
		int r = 0;

		//Go through the dot's collision boxes
		for (int set = 0; set < mColliders.size(); ++set)
		{
			//Center the collision box
			mColliders[set].x = getPosX() + (width - mColliders[set].w) / 2;

			//Set the collision box at its row offset
			mColliders[set].y = getPosY() + r;

			//Move the row offset down the height of the collision box
			r += mColliders[set].h;
		}

	}
	void render(double camX, double camY) {
		gObsTexture.render(getPosX() - camX, getPosY() - camY);
	}
};



Fish::Fish(double x, double y, std::string imgPath) :
	Obstacle(x, y, width, height, imgPath)
{

	// boatType = false;
	// boatDir = false;
	//Initialize the offset

	//Create the necessary SDL_Rects
	initCollider();


	//Initialize colliders relative to position
	shiftColliders();
}

