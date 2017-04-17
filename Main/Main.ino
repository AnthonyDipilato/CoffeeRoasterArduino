/*    Coffee Roaster
 *    Written By Anthony DiPilato
 *    
 *    This project is the code for the coffee roaster that I built.
 *    You can see the build log and details for the project at [url]
 *    
 *    The arduino acts as slave to the raspberry pi.
 *    Inputs:
 *      RaspberyPi commands via usb 
 *      3 thermocouples (MAX31855)
 *      2 infrared flame sensors
 *    
 *    Outputs: 
 *      5 relays
 *      PWM pin for proportial gas valve.
 *    
 *    Relays:
 *      Normally closed solenoid valve for gas line
 *      Ignitor (relay to 110V SSR)
 *      Cooling fan (relay to 110V SSR)
 *      Exhaust Fan (relay to 110V SSR)
 *      Drum Motor (relay to 110V SSR)
 *    
 *    
 *    USE AT YOUR ON RISK.
 *    I am posting this project for EDUCATIONAL USE ONLY.
 *    This project involves electricity, propane, and fire.
 *    I will not be held liable damages and/or injuries resulting from the use of this code or from copying this project.
 *    
 *    Required libraries
 *    Adafruit Max31855 library
 *    
 */
#include "Configuration.h"
#include "Roast.h"


byte proValve = 0;
char inData[8];
int index = 0;

// Setup Roast Class
Roast roast(0);

void setup() {
    // Set pwm frequency, default settings cause valve to hum act unpredictably. 
    TCCR1B = (TCCR1B & 0b11111000) | 0x02; // timer 1 acts on pins 11 and 12
    // Serial output for debugging
    Serial.begin(9600);
    // Pin mode for relays
    pinMode(RELAY_DRUM,     OUTPUT); 
    pinMode(RELAY_COOLING,  OUTPUT); 
    pinMode(RELAY_EXHAUST,  OUTPUT); 
    pinMode(RELAY_GAS,      OUTPUT);
    pinMode(RELAY_IGNITOR,  OUTPUT);
    pinMode(PRO_VALVE,      OUTPUT);
    pinMode(H_BRIDGE_1,     OUTPUT);
    pinMode(H_BRIDGE_2,     OUTPUT);
    // Flame sensors
    pinMode(FLAME_SENSOR, INPUT);
    // Initialize relays (HIGH is off)
    digitalWrite(RELAY_DRUM,    HIGH);
    digitalWrite(RELAY_COOLING, HIGH);
    digitalWrite(RELAY_EXHAUST, HIGH);
    digitalWrite(RELAY_GAS,     HIGH);
    digitalWrite(RELAY_IGNITOR, HIGH);
    // Too lazy to solder a TIP120 to control the valve 
    // when I could just use a $5 H-Bridge that's preassembled.
    // The L298N also has a built in 5v regulator that we can use to power the arduino.
    // H_BRIDGE_1 and H_BRIDGE_2 control "direction" and PRO_VALVE pin for pwm
    // Valve is rated 10v but not problems with 12v
    digitalWrite(H_BRIDGE_1,    LOW);
    digitalWrite(H_BRIDGE_2,    HIGH);
    // Proportional valve uses analog (PWM)
    // TODO change pwm frequency
    analogWrite(PRO_VALVE, 0);
}


int getRelay(int address){
    int relay;
    switch (address) {
        case 5:
            relay = RELAY_DRUM;
            break;
        case 6:
            relay = RELAY_COOLING;
            break;
        case 7:
            relay = RELAY_EXHAUST;
            break;
        case 8:
            relay = RELAY_GAS;
            break;
        case 9:
            relay = RELAY_IGNITOR;
            break;
        default:
            relay = -1;
            break;
    }
    return relay;
}

int getAddress(int relay){
    int address;
    switch(relay) {
        case RELAY_DRUM:
            address = 5;
            break;
        case RELAY_COOLING:
            address = 6;
            break;
        case RELAY_EXHAUST:
            address = 7;
            break;
        case RELAY_GAS:
            address = 8;
            break;
        case RELAY_IGNITOR:
            address = 9;
            break;
        default:
            address = -1;
            break;
    }
    return address;
}

/* Commands
 *    0 - Status
 *    1 - Relay on
 *    2 - Relay off
 *    3 - Set proportional valve
 */
void readCommand(){
    // input format is comma seperated command,value
    char* items = strtok(inData,","); // split by comma
    int command = atoi(items);  // get command and convert to integer
    items = strtok(NULL,",");   // call strtok again for next token value
    int value = atoi(items);    // get value and convert to integer
    int relay;
    // redirect to command action
    switch(command){
        case(0): // Status
          {
            if(value == 0){
              // loop through addresses to output all
              for(int i=1; i<=11; i++){
                getStatus(i);
              }
            }else{
              getStatus(value);
            }
            break;
          }
        case(1): // Toggle relay on
          relay = getRelay(value);
          if (relay == -1){break;}
          roast.toggleRelay(relay, true);
          break;
        case(2): // Toggle relay off
          relay = getRelay(value);
          if (relay == -1){break;}
          roast.toggleRelay(relay, false);
          break;
        case(3): // Set Proportional valve
          roast.setProValve(value);
          break;
    }
    // if not status command return command for confirmation
    if(command != 0){
        // output command for confirmation
        Serial.print(command);
        Serial.print(",");
        Serial.println(value);
    }
}



/* Output Status
 *  Addresses
 *  0 - all
 *  1 - thermo drum (int)
 *  2 - thermo chamber (int)
 *  3 - thermo exhaust (int)
 *  4 - flame status (boolean)
 *  5 - drum relay (boolean)
 *  6 - cooling fan relay (boolean)
 *  7 - exhaust fan relay (boolean)
 *  8 - gas valve relay (boolean)
 *  9 - ignitor relay (boolean)
 *  10 - proportional valve value (int)
 */
void getStatus(int item){  
    // statuses are updated in loop for this function only outputs current
    // output format 0,(item),(value)
    char output[16];
    int value = 0;
    switch(item){
        case(1): 
            value = (int) roast.drumTemp;
            break;
        case(2):
            value = (int) roast.chamberTemp;
            break;
        case(3):
            value = (int) roast.exhaustTemp;
            break;
        case(4):
            value = (int) roast.flameStatus;
            break;
        case(5):
            value = (int) roast.drumState;
            break;
        case(6):
            value = (int) roast.coolingFanState;
            break;
        case(7):
            value = (int) roast.exhaustFanState;
            break;
        case(8):
            value = (int) roast.gasValveState;
            break;
        case(9):
            value = (int) roast.ignitorState;
            break;
        case(10):
            value = (int) roast.proPercent;
            break;
        case(11):
            value = (int) roast.flameStatus;
            break;
    }
    sprintf(output,"0,%d,%d",item,value);
    Serial.println(output);
}

void checkSerial(){
    // Reading incoming commands
    while(Serial.available() > 0){
        // commands are comma separated integers command,value followed by newline (\n)
        char inChar = (char)Serial.read();
        if(inChar != '\n'){
            inData[index] = inChar;
            index++;
        }else{
            readCommand();
            memset(&inData,0,8);
            index = 0;
        }   
    }
}

void loop() {
  // Roast Loop
  roast.loop_();
  // Check for serial input
  checkSerial();
}
