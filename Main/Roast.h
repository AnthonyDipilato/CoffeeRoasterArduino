/*    Roast Class
 *   
 *    Class manages brewing while active
 *  
 */

#ifndef Brew_h
#define Brew_h

#include "Arduino.h"


class Roast
{
    public:
        Roast(int test);
        void loop_();
        void setProValve(int percent);
        void toggleRelay(int relay, boolean state);
        void updateThermos();
        void updateFlameSensor();
        void safetyCheck();
        
        // we don't need decimal percision for temps, so going with int
        unsigned int chamberTemp;
        unsigned int drumTemp;
        unsigned int exhaustTemp;

        int proPercent;
        int lastFlameState;
        boolean flameStatus;

        byte rState;
        byte proValve;
        

        // Booleans for relay states
        boolean ignitorState;
        boolean gasValveState;
        boolean coolingFanState;
        boolean drumState;
        boolean exhaustFanState;
 
        unsigned long lastCheck;
        unsigned long lastCheckFlame;
        unsigned long ignitorStart;
        
    private:
        int none;
};



#endif

