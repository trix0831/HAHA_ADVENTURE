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
    if (boat.getPosX() + boat.width == gate.getPosX()){
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
        return 0;
    }
}

#endif /* checkState_h */
