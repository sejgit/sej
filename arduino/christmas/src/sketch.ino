/*
 christmas.ino
 written dec2014 to control string of lights using a bjt
*/

#define ONOFF = 6; // ON/OFF switch
#define POT = 7; //the speed potentiometer
#define LED1 = 9; // red led
#define LED2 = 10; // green led
#define LIGHTS = 12;  //the christmas lights
#define BAUD = 9600; //serial port baud

int lightSpeed = 1000; //start delay at 1s
int dimSpeed = 30; //led dimming speed
int brightness = 0; //starting brightness
int fadeAmount = 5; // how many points to fade the LEDs by
boolean mode = false;


void setup()
{
  pinMode(ONOFF, INPUT);
  pinMode(POT, INPUT);

  pinMode(LIGHTS, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(bjt1, OUTPUT);

  Serial.begin(BAUD);
}

void loop()
{
  while (digitalRead(ONOFF) {
      digitalWrite(LIGHTS, mode);
      Serial.println(mode);
      analogWrite(LED1, brightness);
      analogWrite(LED2, 255-brightness);
      brightness = brightness + fadeAmount;
      if (brightness == 0 || brightness == 255) {
	fadeAmount = -fadeAmount;
	mode = !mode;
	delay(lightSpeed);
      }
      delay(dimSpeed);
    }    
}
