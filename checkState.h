//
//  checkState.h
//  Dec16BOAT
//
//  Created by hubert on 2022/12/25.
//

#ifndef checkState_h
#define checkState_h

#include "boat.h"
#include "Gate.h"

bool checkState(Boat& boat, Gate& gate){
    if ((gate.getPosX() - boat.getPosX() < BOAT_WIDTH + 1) &&  (boat.getPosY() >= 51)  && (boat.getPosY() < 250-BOAT_HEIGHT)){
        if (gate.getState() == 0){ //AND
            return (boat.getBoatType() & gate.getNumber());
        }else if (gate.getState() == 1){ //OR
            return (boat.getBoatType() | gate.getNumber());
        }else if (gate.getState() == 2){ //XOR
            return (boat.getBoatType() ^ gate.getNumber());
        }else if (gate.getState() == 3){ //NAND
            return !(boat.getBoatType() & gate.getNumber());
        }else if (gate.getState() == 4){ //NOR
            return !(boat.getBoatType() | gate.getNumber());
        }else{
            return 0;
        }
    }else{
        return 1;
    }
}

#endif /* checkState_h */
