//this forked from from Trash80 HE280AccelerometerInterface https://github.com/trash80/HE280AccelerometerInterface
// by tunkerhammer 1/9/2017 

#include <Wire.h>

#define DEBUG 1 // Comment out this line to not echo results to serial

//------------------------------------------------------------------------------------------------------
//this setup is for Teensy 3/3.2
//const uint8_t click_pin = 2; // "INT" interrupt pin from HE280 - Blue on my model
//const uint8_t reset_pin = 3; // Used to re-send the initialization code to the HE280's accelerometer. Z Probe Mod pin.
//const uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's Z probe in. Duet set to Type: 5.
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
// this setup is for a Adafruit Feather 32u4 Basic Proto or Adafruit Feather M0 Basic Proto
const uint8_t click_pin = 11; // "INT" interrupt pin from HE280 - Blue on my model
const uint8_t reset_pin = 12; // Used to re-send the initialization code to the HE280's accelerometer. Z Probe Mod pin.
const uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's Z probe in. Duet set to Type: 5.
//------------------------------------------------------------------------------------------------------

#define Z_PROBE_SENSITIVITY  14 // Seemecnc's default is 20, 15 is a good middle ground, 0-126 7 bit value

#include "accelerometer.h"
int debug_probe_disabled_switch = 0; //this is used to only send one proble disable for debug
int delay_for_probe = 0; // this is used to have only one delay once in probe movement



void setup() {
  pinMode(click_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  
  #ifdef USE_RESET_PIN
  pinMode(reset_pin, INPUT_PULLUP);
  #endif
  
  while(!accelerometer_init()) {
    debug_println("Failed Accel Init");
    accelerometer_status();
    digitalWrite(trigger_pin,HIGH); // Keep HIGH so that travel stops
    delay(50);
  }
  digitalWrite(trigger_pin,LOW);
}


void loop() {
    //debug_println (xx);
    if(digitalRead(reset_pin)) {
        if (delay_for_probe == 0){  //only need to delay enableing probe once
          debug_println("enable probe");
          delay(60); //delay used to give the printer time to start to move before enableing probe
          delay_for_probe = 1;         
        }

        if(!digitalRead(click_pin)) {
            digitalWrite(trigger_pin,HIGH);
            delay(100);
            digitalWrite(trigger_pin,LOW);
            debug_print("Accel hit: ");
            debug_println(accelerometer_read(0x31)); 
            debug_probe_disabled_switch = 0;
            delay_for_probe = 0;     
        }
    }
        
    if(!digitalRead(reset_pin)) {
        if (debug_probe_disabled_switch == 0){
          debug_probe_disabled_switch =1;
          debug_println("disable probe");
        }
        //digitalWrite(trigger_pin,LOW);
    }


}
