/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * Version update Nov, 2014
 * Mitsubishi HVAC protocol added by Vincent Cruvellier.
 *  delaymicrosecond() modified to avoid limit.
 * Version update Dec, 2014
 * Panasonic HVAC protocol added by Mathieu Thomas. 
 * Version update Dec, 2015
 * Mitsubishi HVAC protocol enhancement by Vincent Cruvellier.
 *   added    void sendHvacMitsubishiFD(
      HvacMode                  HVAC_Mode,           // Example HVAC_HOT  HvacMitsubishiMode
      int                       HVAC_Temp,           // Example 21  (°c)
      HvacFanMode               HVAC_FanMode,        // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
      HvacVanneMode             HVAC_VanneMode,      // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
      HvacAreaMode              HVAC_AreaMode,       // Example AREA_AUTO
      HvacWideVanneMode         HVAC_WideMode,       // Example WIDE_MIDDLE
      int                       HVAC_PLASMA,          // Example true to Turn ON PLASMA Function
      int                       HVAC_CLEAN_MODE,      // Example false 
      int                       HVAC_ISEE,            // Example False
      int                       OFF                   // Example false to Turn ON HVAC / true to request to turn off
    );
 * sendHvacMitsubishiFD Not tested, i don't have this material -- if someone can confirm then please push a note.
 */

#include "IRremote2.h"

IRsend irsend;

const int buttonPin = 12;
int switchedOn = 0;

void setup()
{
  Serial.begin(9600);
  //IR Demo send a cmd To Mitsubishi HVAC
 Serial.println("IR Demo send a cmd To HVAC Mitsubishi or Panasonic");
 Serial.println("Please any data to the Serial Interface in order to start the Demo");
 Serial.println("");
 
 pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == 0 and switchedOn == 0) {
    irsend.sendHvacMitsubishi(HVAC_HOT, 22, FAN_SPEED_AUTO, VANNE_AUTO_MOVE, false);
    switchedOn = 1;
    Serial.println("Command 'HVAC_HOT, 22, FAN_SPEED_AUTO, VANNE_AUTO_MOVE' sent.\n Waiting 30 Seconds to send OFF.");
    delay(30000);
  }
  if (digitalRead(buttonPin) == 0 and switchedOn == 1) {
    irsend.sendHvacMitsubishi(HVAC_HOT, 21, FAN_SPEED_AUTO, VANNE_AUTO_MOVE, true);
    Serial.println("Turn OFF\nCommand sent. End Of Sketch");
    delay(15000); // Let HVAC sleeping
  } 
}
