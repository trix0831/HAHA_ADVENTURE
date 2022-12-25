//
//  Gatw.h
//  Dec16BOAT
//
//  Created by hubert on 2022/12/25.
//

#ifndef Gatw_h
#define Gatw_h


#include "LTexture.h"
#include "Obstacle.h"
#include <iostream>
#include <vector>

const int GATE_WIDTH = 128;
const int GATE_HEIGHT = 50;

//The dimensions of the level
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

class Gate :  public Obstacle {

public:
    static const int width = GATE_WIDTH;
    static const int height = GATE_HEIGHT;

    Gate(double x, double y, std::string imgPath);

    void initCollider() {
        mColliders.resize(2);
        mColliders[0].x = getPosX();
        mColliders[0].w = GATE_WIDTH;
        mColliders[0].h = GATE_HEIGHT;
        mColliders[0].y = 0;

        mColliders[1].x = getPosX();
        mColliders[1].w = GATE_WIDTH;
        mColliders[1].h = GATE_HEIGHT;
        mColliders[1].y = 250;
    }

    void shiftColliders() {
        mColliders[0].x = getPosX();
        mColliders[0].y = getPosY();
        
        mColliders[1].x = getPosX();
        mColliders[1].y = 250;
    }

    void render(double camX, double camY) {
//        gObsTexture.render(getPosX() - camX, getPosY() - camY, &currentGate[state][number]);
        gObsTexture.render(getPosX() - camX, getPosY() - camY);

    }

    bool getNumber() {
        return number;
    }

    int getState() {
        return state;
    }

private:
    int state; //  0->AND , 1->OR , 2->XOR , 3->NAND , 4->NOER
    bool number;
    SDL_Rect currentGate[5][2];
};

Gate::Gate(double x, double y, std::string imgPath):
    Obstacle(x, y, width, height, imgPath)
{
    int cnt = 0;
    for (int i = 0; i<5; ++i){
        for (int j = 0; j<2; ++j){
            currentGate[i][j].x = cnt*GATE_WIDTH;
            currentGate[i][j].y = 0;
            currentGate[i][j].w = GATE_WIDTH;
            currentGate[i][j].h = GATE_HEIGHT;
            cnt ++;
        }
    }
    
    initCollider();
    shiftColliders();
}


#endif /* Gatw_h */
