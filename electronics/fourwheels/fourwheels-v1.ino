/*
Running four wheels with transisters
Thomas & Dad
2016 04 07
*/
 
 
int fd = 3;  /* front driver */
int fp = 9;  /* front passenger */ 
int rd = 10;  /* rear driver */
int rp = 11;  /* rear passenger */
int fd_corr=0; /* correction factors */
int fp_corr=0;
int rd_corr=0;
int rp_corr=0;
int cur_speed=150;
int new_speed=0;
int old_speed=new_speed;
const int ledPin=13;
int ledState=LOW;
long previousMillis=0;
long interval=1000 - (3 * cur_speed);

void setup() 
{ 
  pinMode(ledPin, OUTPUT);
  pinMode(fd, OUTPUT);
  pinMode(fp, OUTPUT);
  pinMode(rd, OUTPUT);
  pinMode(rp, OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Speed 0 to 255");
  SpeedChange();
} 
 
 
void loop() 
{ 
  Blink();
  if (Serial.available())
  {
    new_speed = Serial.parseInt();
    if (new_speed >= 0 && new_speed <= 255)
    {
      cur_speed = new_speed;
      interval = 1000 - (3 * cur_speed);
    }
  }
  if (cur_speed != old_speed)
  {
    old_speed = cur_speed;
    SpeedChange();
    Serial.print("Speed = '");
    Serial.print(cur_speed);
    Serial.println("'");
      }
}

void SpeedChange()
{
  analogWrite(fd, cur_speed+fd_corr);
  analogWrite(fp, cur_speed+fp_corr);
  analogWrite(rd, cur_speed+rd_corr);
  analogWrite(rp, cur_speed+rd_corr);
}

void Blink()
{
  unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
 
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    digitalWrite(ledPin, ledState);
    }
}

