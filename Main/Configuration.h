/*    Configuration
 *   
 *    Settings and pin configurations
 *  
 */

#ifndef Configuration_h
#define Configuration_h


// Relays
#define RELAY_DRUM            2
#define RELAY_COOLING         3
#define RELAY_EXHAUST         4
#define RELAY_GAS             5
#define RELAY_IGNITOR         6
// Flame Sensors
#define FLAME_SENSOR        A1
// Thermos
// Drum
#define THERMO_DRUM_CLK       31
#define THERMO_DRUM_CS        33
#define THERMO_DRUM_SO        35
// Exhaust
#define THERMO_EXHAUST_CLK    37
#define THERMO_EXHAUST_CS     39
#define THERMO_EXHAUST_SO     41
// Chamber
#define THERMO_CHAMBER_CLK    43
#define THERMO_CHAMBER_CS     45
#define THERMO_CHAMBER_SO     47
// Proportional Valve
#define PRO_VALVE             11
// Minimum gas setting before flame goes out, used to minimize the chance of flame out.
// !80% PWM is lowest so a value of 
 #define PWM_MIN               200     

// Flame Safety Settings
#define FLAME_SENSOR_BYPASS   true
#define IGNITION_TIME         5000     // How long to run ignitor to ignite flame (ms)
#define FLAME_READS           10       // Flame sensor number of reads for smoothing
#define FLAME_THRESHOLD       10       // Lower threshold above off value to detect flame
// MAX318 is limited by read speed so we will use a delay
#define TEMP_DELAY            500
#define FLAME_DELAY           500
// Debounce delay for checking buttons
#define DEBOUNCE_DELAY        50
// Command Addresses
// Relays


#endif

