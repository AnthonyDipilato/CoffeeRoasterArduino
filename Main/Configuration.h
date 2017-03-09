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
#define FLAME_SENSOR_L        A0
#define FLAME_SENSOR_R        A1
// Thermos
// Drum
#define THERMO_DRUM_CLK       31
#define THERMO_DRUM_CS        32
#define THERMO_DRUM_SO        33
// Exhaust
#define THERMO_EXHAUST_CLK    34
#define THERMO_EXHAUST_CS     35
#define THERMO_EXHAUST_SO     36
// Chamber
#define THERMO_CHAMBER_CLK    37
#define THERMO_CHAMBER_CS     38
#define THERMO_CHAMBER_SO     39
// Proportional Valve
#define PRO_VALVE             11
#define PWM_MIN               40       // Minimum gas setting before flame goes out, used to minimize the chance of flame out.

// Flame Safety Settings
#define IGNITION_TIME         5000     // How long to run ignitor to ignite flame (ms)
#define FLAME_READS           10       // Flame sensor number of reads for smoothing
#define FLAME_THRESHOLD       255       // Lower threshold above off value to detect flame
// MAX6675 is limited by read speed so we will use a delay
#define TEMP_DELAY            500
#define FLAME_DELAY           500
// Debounce delay for checking buttons
#define DEBOUNCE_DELAY        50
// Command Addresses
// Relays


#endif

