// fenton creeper robot
// arduino uno based four wheel drive robot
// tsj sej 2016 04 22

//Keyboard Controls:
// (left motors)
// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
// (right motors)
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// y - speed: more forward
// n - speed: more backward
// u - forward (current speed then increase)
// m - backward (current speed then increase)
// . - stop (speed zero)
// h - left hard (left / right drive)
// j - left soft (idle left side / drive right)
// k - right soft (idle right side / drive left)
// l - right hard (left / right drive)


// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Left Motors
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Right Motors
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

int speed = 0; // speed start
int dif = 0; // speed diferencial
int incr = 20; // speed change incr;
void setup() {  // Setup runs once per reset

  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);

  //Define L298N Dual H-Bridge Motor Controller Pins

  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);

}

void loop() {

  // Initialize the Serial interface:

  if (Serial.available() > 0) {
  int inByte = Serial.read();
  switch (inByte) {

    case 'y': // speed++
      speed += incr;
      status = drive(speed);
      break;

    case 'n': // speed--
      speed -= incr;
      status = drive(speed);
      Serial.println("speed++"); // Prints out “Motor 1 Forward” on the serial monitor
      Serial.println("   "); // Creates a blank line printed on the serial monitor
      break;


  }
  // binary status B0000   left [on/off dir(+/-)] right [on/off dir(+/-)] 
// y - speed: more forward
// n - speed: more backward
// u - forward (current speed then increase)
// m - backward (current speed then increase)
// . - stop (speed zero)
// h - left hard (left / right drive)
// j - left soft (idle left side / drive right)
// k - right soft (idle right side / drive left)
// l - right hard (left / right drive)



 
  
//______________Motor 1______________

case '1': // Motor 1 Forward
analogWrite(speedPinA, 255);//Sets speed variable via PWM
digitalWrite(dir1PinA, LOW);
digitalWrite(dir2PinA, HIGH);
Serial.println("Motor 1 Forward"); // Prints out “Motor 1 Forward” on the serial monitor
Serial.println("   "); // Creates a blank line printed on the serial monitor
break;

case '2': // Motor 1 Stop (Freespin)
analogWrite(speedPinA, 0);
digitalWrite(dir1PinA, LOW);
digitalWrite(dir2PinA, HIGH);
Serial.println("Motor 1 Stop");
Serial.println("   ");
break;

case '3': // Motor 1 Reverse
analogWrite(speedPinA, 255);
digitalWrite(dir1PinA, HIGH);
digitalWrite(dir2PinA, LOW);
Serial.println("Motor 1 Reverse");
Serial.println("   ");
break;

//______________Motor 2______________

case '4': // Motor 2 Forward
analogWrite(speedPinB, 255);
digitalWrite(dir1PinB, LOW);
digitalWrite(dir2PinB, HIGH);
Serial.println("Motor 2 Forward");
Serial.println("   ");
break;

case '5': // Motor 1 Stop (Freespin)
analogWrite(speedPinB, 0);
digitalWrite(dir1PinB, LOW);
digitalWrite(dir2PinB, HIGH);
Serial.println("Motor 2 Stop");
Serial.println("   ");
break;

case '6': // Motor 2 Reverse
analogWrite(speedPinB, 255);
digitalWrite(dir1PinB, HIGH);
digitalWrite(dir2PinB, LOW);
Serial.println("Motor 2 Reverse");
Serial.println("   ");
break;

default:
// turn all the connections off if an unmapped key is pressed:
for (int thisPin = 2; thisPin < 11; thisPin++) {
digitalWrite(thisPin, LOW);
}
  }
 }
}




int drive(int left, int right, int speed, int left, int right, int dif ) {
  string out = left right speed;
  Serial.println(out);
  Serial.println("   "); // Creates a blank line printed on the serial monitor

  }


