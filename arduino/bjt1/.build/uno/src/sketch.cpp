#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
/*
 bjt1
 
 This sketch shows how to fade two LEDs on pin 9 and 10 
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

int led1 = 9;           // the pin that the LED is attached to
int led2 = 10;
int bjt1 = 12;		// the pin for the first bjt1
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
boolean bjtmode = false;

// the setup routine runs once when you press reset:
void setup()  { 
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(bjt1, OUTPUT);
} 

// the loop routine runs over and over again forever:
void loop()  { 
  // set the brightness of pin 9 pin 10
  analogWrite(led1, brightness);    
  analogWrite(led2, 255-brightness);
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount; 
    digitalWrite(bjt1, bjtmode);
    bjtmode = !bjtmode;
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}

