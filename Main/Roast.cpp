/*    Roast Class
 *   
 *    Class manages roasting while active
 *  
 */

#include "Arduino.h"
#include "Roast.h"
#include "Configuration.h"
#include "Adafruit_MAX31855.h"


// Set pins and initialize thermocouples (CLK, CS, SO)
Adafruit_MAX31855 exhaust_thermo(THERMO_EXHAUST_CLK, THERMO_EXHAUST_CS, THERMO_EXHAUST_SO);
Adafruit_MAX31855 drum_thermo(THERMO_DRUM_CLK, THERMO_DRUM_CS, THERMO_DRUM_SO);
Adafruit_MAX31855 chamber_thermo(THERMO_CHAMBER_CLK, THERMO_CHAMBER_CS, THERMO_CHAMBER_SO);

Roast::Roast(int test){
    // first we will get fire sensor readings
    // this will calibrate our off threshold since outside lighting will alter the reading
    lastCheck = 0;
    lastCheckFlame = 0;
    proValve = 0;
    proPercent = 0;
    // Booleans for relay states
    ignitorState =      false;
    gasValveState =     false;
    coolingFanState =   false;
    exhaustFanState =   false;
    drumState =         false;
    flameStatus = false;
}

void Roast::setProValve(int percent){
    float value;
    if(percent > 100){ percent = 100;}
    if(percent < 0){ percent = 0;}
    if(percent == 0){
      value = 0;
    }else{
      percent = ( (100 - PWM_MIN) * ((float) percent/100)) + PWM_MIN;
      // input is percentage so we will convert it to 0-255 byte;
      value = ((float) percent / 100) * 255;
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


void Roast::updateFlameSensor(){
  int flameReading = digitalRead(FLAME_SENSOR);
  if (flameReading != lastFlameState) {
    // reset the debouncing timer
    lastCheckFlame = millis();
  }
  if ((millis() - lastCheckFlame) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (flameReading == 1) {
      flameStatus = false;
    }else{
      flameStatus = true;
    }
    //Serial.print("Flame: ");
    //Serial.println(flameStatus);
  }
  lastFlameState = flameReading;
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
    if(ignitorState && !flameStatus){ 
        if(millis() > (ignitorStart + IGNITION_TIME)){ // shut off gas
            //Serial.println("Safety Shutoff.");
            toggleRelay(RELAY_GAS, false);
            toggleRelay(RELAY_IGNITOR, false);
        }
    // ignitor on, fire on
    }else if(ignitorState && flameStatus){
          toggleRelay(RELAY_IGNITOR, false);
    // ignitor off, gas on, fire off
    }else if(!ignitorState && gasValveState && proValve > 0 && !flameStatus
    ){
          toggleRelay(RELAY_IGNITOR, true);
    }
}

void Roast::loop_(){
    // update temps  
    updateThermos();
    updateFlameSensor();
    if(!FLAME_SENSOR_BYPASS){
      safetyCheck();
    }
}




 
