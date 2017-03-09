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
        void updateFlameSensors();
        void safetyCheck();
        
        // we don't need decimal percision for temps, so going with int
        unsigned int chamberTemp;
        unsigned int drumTemp;
        unsigned int exhaustTemp;

        int proPercent;

        // flame sensors
        byte flameR;
        byte flameL;
        byte offFlameR;
        byte offFlameL;
        byte rState;
        byte proValve;
        
        // flame variables
        boolean fireState;
        boolean flameStatusL;
        boolean flameStatusR;
        boolean flameCalibration;

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

