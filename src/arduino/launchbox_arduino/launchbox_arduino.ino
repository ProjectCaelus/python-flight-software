/*
toggle - on/off/on switch
pulse - you push it and it opens, waits .05s, and closes

Nitrogen Fill (1 switch (toggle)))
Ethanol Drain (1 switch (toggle))
Ethanol Vent (1 button (pulse), 1 switch (toggle))
Ethanol Main Propellant Valve (1 switch (toggle))

Nitrous Oxide Fill
Nitrous Oxide Drain
Nitrous Oxide Vent
Nitrous Oxide Main Propellant Valve
*/

#include "constants.h"

enum pin_state = {
    DO_NOTHING,
    OPEN_VENT,
    CLOSE_VENT
}


const int MAX_PIN = 21;
int PIN_MAP[MAX_PIN];

// Pin counts
const int NUM_VALVES = 8;
const int NUM_BUTTONS = 2;

// Local variables
boolean aborted;

// Arrays that keep track of stuff
// States: 0 means do nothing, 1 is CLOSE_VENT, and 2 is OPEN_VENT (matches the constants)
// TODO: CHECK IF digitalRead CAN BE COMPARED TO A BOOLEAN
pin_state states[NUM_VALVES];
boolean pulsing[NUM_BUTTONS];
int vent_pins[] = {NITROGEN_FILL, ETHANOL_DRAIN, ETHANOL_VENT, ETHANOL_MPV, NO_FILL, NO_DRAIN, NO_VENT, NO_MPV};
int pulse_pins[] = {ETHANOL_VENT_PULSE, NO_VENT_PULSE};

void setup(){
    PIN_MAP[NITROGEN_FILL] = VALVE_NITROGEN_FILL;
    PIN_MAP[ETHANOL_DRAIN] = VALVE_ETHANOL_DRAIN;
    PIN_MAP[ETHANOL_VENT] = VALVE_ETHANOL_VENT;
    PIN_MAP[ETHANOL_VENT_PULSE] = VALVE_ETHANOL_VENT;
    PIN_MAP[ETHANOL_MPV] = VALVE_ETHANOL_MPV;
    PIN_MAP[NO_FILL] = VALVE_NO_FILL;
    PIN_MAP[NO_DRAIN] = VALVE_NO_DRAIN;
    PIN_MAP[NO_VENT] = VALVE_NO_VENT;
    PIN_MAP[NO_VENT_PULSE] = VALVE_NO_VENT;
    PIN_MAP[NO_MPV] = VALVE_NO_MPV;

    for(int i = 2; i <= 17; i++){
        pinMode(i, INPUT_PULLUP);
    }
    Serial.begin(9600);
    aborted = false;
    
    for(int i = 0; i < NUM_VALVES; i++){
        pin_states[i] = DO_NOTHING; 
    }
    for(int i = 0; i < NUM_BUTTONS; i++){
        pulsing[i] = false;
    }
}

void loop(){
    if(aborted){
        return;
    }
    for(int i = 0; i < NUM_VALVES; i++){
        pin_state current_state = checkToggleSwitch((i + 1) * 2); // (i + 1) * 2 maps from array index to pin number
        if(current_state != states[i]){
            states[i] = current_state;
            send_message(current_state, PIN_MAP[vent_pins[i]]);
        }
    }
    if(buttonRead(ABORT_PIN)){
        aborted = true;
        send_message(DATA, ABORT);
        for(int i = 0; i < NUM_VALVES; i++){
            send_message(OPEN_VENT, PIN_MAP[vent_pins[i]]);
        }
    }
    for(int i = 0; i < NUM_BUTTONS; i++){
        if(buttonRead(pulse_pins[i])){
            if(pulsing[i]){
                continue;
            }
            pulsing[i] = true;
            send_message(PULSE, PIN_MAP[pulse_pins[i]]);
        }
        else{
            pulsing[i] = false;
        }
    }
}

int buttonRead(int pin){
    return digitalRead(pin);
}

void send_message(int cmd, int data){
    if(!override){
        return;
    }
    Serial.write(cmd);
    Serial.write(data);
    delay(50);
}

/*
 * note: these SPDT switches all have their common pins connected to GND because arduino only has an INPUT_PULLUP option instead of an INPUT_PULLDOWN option
 * this means that a value of LOW indicates that the switch has been flicked in a certain direction, not HIGH
 * switchStart always maps to CLOSE_VENT, OFF maps to DO_NOTHING, switchStart + 1 maps to OPEN_VENT

 * @param switchStart: the first pin of a toggle switch 
 * @return CLOSE_VENT if the switch is closest to switchStart, DO_NOTHING if it's off, and OPEN_VENT if it's closest to switchStart + 1
 
 * for example, if this is the switch:
 *                 /
 *                /
 *               /
 *              /
 *             /
 *  ----------/-----------
 *  8 (ON)  (OFF)        9 (ON)
 *  CL_VNT  DO_NTHING    OPEN_VENT

 * then startSwitch is 8 and the method returns OPEN_VENT
 * note that in this scenario, OPEN_VENT is returned even though switchStart has a value of LOW
 * this reversal has to be done (i.e. we can't just check for HIGH) because the OFF state always has a state of HIGH, which means that 2 pins are always HIGH, and 
 * because of how SPDT works, the opposite pin is LOW
*/

int checkToggleSwitch(int switchStart) {
  if(digitalRead(switchStart) == LOW) {
    return OPEN_VENT;  
  }  
  else if(digitalRead(switchStart + 1) == LOW) {
    return CLOSE_VENT;
  }
  else {
    return DO_NOTHING;
  }
}
