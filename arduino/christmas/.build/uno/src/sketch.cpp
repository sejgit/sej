#include <Arduino.h>

void setup();
void loop();
void calibrate();
#line 1 "src/sketch.ino"
/*
 christmas.ino
 written dec2014 to control string of lights using a bjt
*/

const int ONOFF = 2; // ON/OFF switch
const int DELAYPOT = 0; //the speed potentiometer
const int LED1 = 9; //red led
const int LED2 = 10; //green led
const int LIGHTS = 12;  //the christmas lights
const int BAUD = 9600; //serial port baud
const int INDICATOR = 13; //calibration indicator

int brightness = 0; //starting brightness
int fadeAmount = 1; //how many points to fade the LEDs by

boolean mode = false;
boolean onoffSW = false;

unsigned long prevMillisLED = 0; //will store last time LED was updated
unsigned long prevMillisLight = 0; //will store last time Light was updated
unsigned long curMillis = 0;
unsigned long lightSpeed = 1000; //interval at which to blink (milliseconds)
unsigned long ledSpeed = 20;

int sensorMin = 1023; //minimum sensor value
int sensorMax = 10; //maximum sensor value
int sensorValue = 10; //the sensor value


void setup()
{
  pinMode(ONOFF, INPUT);
  pinMode(LIGHTS, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(INDICATOR, OUTPUT);
  pinMode(A0, INPUT);

  Serial.begin(BAUD);
}

void loop()
{
  curMillis = millis();
  onoffSW = digitalRead(ONOFF);
  
  //  delay(10);
  if (onoffSW) {
    digitalWrite(INDICATOR, LOW); //signal the end of the calibration period
    sensorValue = analogRead(DELAYPOT); //read the sensor
    lightSpeed = map(sensorValue, sensorMin, sensorMax, 0, 5000);   //apply the calibration to the sensor reading
    lightSpeed = constrain(lightSpeed, 0, 5000); //in case the sensor value is outside the range seen during calibration

    if (curMillis - prevMillisLED > ledSpeed) {
      prevMillisLED = curMillis;   
      analogWrite(LED1, brightness);
      analogWrite(LED2, 255-brightness);
      brightness = brightness + fadeAmount;

      if (brightness == 0 || brightness == 255) {
	fadeAmount = -fadeAmount;
      }
    }

    if (curMillis - prevMillisLight > lightSpeed) {
      prevMillisLight = curMillis;   
      mode = !mode;
      digitalWrite(LIGHTS, mode);
      Serial.print(mode);
      Serial.print("\t");
      Serial.print(brightness);
      Serial.print("\t");
      Serial.print(sensorMin);
      Serial.print("\t");
      Serial.print(sensorMax);
      Serial.print("\t");
      Serial.println(sensorValue);
      Serial.print("\t");
      Serial.println(lightSpeed);
    }
  } else {
    digitalWrite(INDICATOR, HIGH);
    analogWrite(LED1, 0);
    analogWrite(LED2, 0);
    digitalWrite(LIGHTS, LOW);
    calibrate(); 
  }

}


void calibrate() {

  sensorValue = analogRead(A0);

  // record the maximum sensor value
  if (sensorValue > sensorMax) {
    sensorMax = sensorValue;
  }

  // record the minimum sensor value
  if (sensorValue < sensorMin) {
    sensorMin = sensorValue;
  }
  Serial.print(sensorMin, DEC);
  Serial.print("\t");
  Serial.print(sensorMax, DEC);
  Serial.print("\t");
  Serial.println(sensorValue, DEC);

  delay(20);
}





