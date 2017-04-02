/*    Roast Class
 *   
 *    Class manages roasting while active
 *  
 */

#include "Arduino.h"
#include "Roast.h"
#include "Configuration.h"
#include <max6675.h>


// Set pins and initialize thermocouples (CLK, CS, SO)
MAX6675 chamber_thermo(THERMO_CHAMBER_CLK, THERMO_CHAMBER_CS, THERMO_CHAMBER_SO);
MAX6675 exhaust_thermo(THERMO_EXHAUST_CLK, THERMO_EXHAUST_CS, THERMO_EXHAUST_SO);
MAX6675 drum_thermo(THERMO_DRUM_CLK, THERMO_DRUM_CS, THERMO_DRUM_SO);

Roast::Roast(int test){
    // first we will get fire sensor readings
    // this will calibrate our off threshold since outside lighting will alter the reading
    flameSensor = 0;
    lastCheck = 0;
    lastCheckFlame = 0;
    proValve = 0;
    proPercent = 0;
    flameCalibration = false;
    // Booleans for relay states
    ignitorState =      false;
    gasValveState =     false;
    coolingFanState =   false;
    exhaustFanState =   false;
    drumState =         false;
    fireState =         false;
}

void Roast::setProValve(int percent){
    float value;
    if(percent > 100){ percent = 100;}
    if(percent < 0){ percent = 0;}
    if(percent == 0){
      value = 0;
    }else{
      // input is percentage so we will convert it to 0-255 byte
      int minOffset = 255 - PWM_MIN;
      value = (((float) percent / 100) * minOffset) + PWM_MIN;
    }
    proPercent = percent;
    proValve = (byte) value;
    analogWrite(PRO_VALVE,   proValve);
}

void Roast::toggleRelay(int relay, boolean state){
    int pin;
    // match relay value with constants and toggle state
    switch(relay){
        case(RELAY_DRUM):
            drumState =       state;
            break;
        case(RELAY_COOLING):
            coolingFanState = state;
            break;
        case(RELAY_EXHAUST):
            exhaustFanState = state;
            break;
        case(RELAY_GAS):
            gasValveState =   state;
            break;
        case(RELAY_IGNITOR):
            ignitorState =    state;
            ignitorStart = millis(); // save start time
            break;
    }
    if(state){
        digitalWrite(relay,   LOW);
    }else{
        digitalWrite(relay,   HIGH);
    }
}

// Update Flamer Sensor readings
void Roast::updateFlameSensors(){
    int flameTotal = 0;
    if(millis() > (lastCheckFlame + FLAME_DELAY)){
        // loop of reads for smoothing
        for(int i=0; i<FLAME_READS; i++){
            flameTotal += analogRead(FLAME_SENSOR);
        }
        // convert to byte 0-255 also rounds value
        flameSensor = byte(flameTotal / FLAME_READS);
        if(!flameCalibration){
            offFlame = flameSensor;
            flameCalibration = true;
        }
        // flame status
        if(flameSensor > (offFlame + FLAME_THRESHOLD)){ flameStatus = true; }else{ flameStatus = false; }
        if(flameStatus){ // if true fire is on
          fireState = true;
        }else{
          fireState = false;
        }
        lastCheckFlame = millis();
        Serial.print("flame: ");
        Serial.println(flameSensor);
    }
}

void Roast::updateThermos(){
    if(millis() > lastCheck + TEMP_DELAY){
        // max6675 library only updates on readC(), readF() only converts the current value, so we have to call both
        chamber_thermo.readCelsius();
        drum_thermo.readCelsius();
        exhaust_thermo.readCelsius();
        // convert to F because we are uncivilized
        chamberTemp = chamber_thermo.readFarenheit();
        drumTemp = drum_thermo.readFarenheit();
        exhaustTemp = exhaust_thermo.readFarenheit();
        // update last check for temp delay
        lastCheck = millis();
    }    
}

// If gas is on and fire if off run ignitor
// if it fails to ignite shut off main gas valve
void Roast::safetyCheck(){
    // if ignitor is running check time
    if(ignitorState && !fireState){ 
        if(millis() > (ignitorStart + IGNITION_TIME)){ // shut off gas
            toggleRelay(RELAY_GAS, false);
            toggleRelay(RELAY_IGNITOR, false);
        }
    // ignitor on, fire on
    }else if(ignitorState && fireState){
          toggleRelay(RELAY_IGNITOR, false);
    // ignitor off, gas on, fire off
    }else if(!ignitorState && gasValveState && proValve > 0 && !fireState){
          toggleRelay(RELAY_IGNITOR, true);
    }
}

void Roast::loop_(){
    // update temps  
    updateThermos();
    if(!FLAME_SENSOR_BYPASS){
      // update flame sensors
      updateFlameSensors();r      // safety check
      safetyCheck();
    }
}




 
