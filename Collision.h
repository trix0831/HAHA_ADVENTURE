//
//  Collision.h
//  Dec16BOAT
//
//  Created by hubert on 2022/12/24.
//

#ifndef Collision_h
#define Collision_h

#include "boat.h"
#include "Fish.h"

bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b );

bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b )
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Go through the A boxes
	for ( int Abox = 0; Abox < a.size(); Abox++ )
	{
		//Calculate the sides of rect A
		leftA = a[ Abox ].x;
		rightA = a[ Abox ].x + a[ Abox ].w;
		topA = a[ Abox ].y;
		bottomA = a[ Abox ].y + a[ Abox ].h;

		//Go through the B boxes
		for ( int Bbox = 0; Bbox < b.size(); Bbox++ )
		{
			//Calculate the sides of rect B
			leftB = b[ Bbox ].x;
			rightB = b[ Bbox ].x + b[ Bbox ].w;
			topB = b[ Bbox ].y;
			bottomB = b[ Bbox ].y + b[ Bbox ].h;

			//If no sides from A are outside of B
			if ( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
			{
				//A collision is detected
				return true;
			}
		}
	}

	//If neither set of collision boxes touched
	return false;
}

#endif /* Collision_h */
