// fenton creeper robot
// arduino uno based four wheel drive robot
// tsj sej 2016 04 25


char* Instructions[]={
  "Keyboard Controls:",
  "binary status B0000   on/off[left/right] dir(+/-)[left/right]",
  " y - speed: more",
  " n - speed: less",
  " . - stop (speed & dif zero)",
  " , - dif to zero",
  " u - forward (current speed then increase)",
  " m - backward (current speed then increase)",
  " h - left hard (left / right drive)",
  " j - left soft (idle left side / drive right)",
  " k - right soft (idle right side / drive left)",
  " l - right hard (left / right drive)",
  "  "
};


// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Left Motors
static int dir1PinA = 2;
static int dir2PinA = 3;
static int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Right Motors
static int dir1PinB = 4;
static int dir2PinB = 5;
static int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

static int incr = 20; // speed change incr;


void setup() {  // Setup runs once per reset

  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);
  Serial.write(0);
  for (int i = 0; i < 13; i++){
     Serial.println(Instructions[i]);
     delay(10);
   }

  //Define L298N Dual H-Bridge Motor Controller Pins

  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);

}


void loop(){
  static int speed = 0;
  static int dif = 0;
  static int status = B000;
  static int newstatus = B000;

  status = drive(speed, dif, newstatus);
  newstatus = status;

  
  if (Serial.available() > 0) {
    int inByte = Serial.read();

    switch (inByte) {
      case 'y': // speed++
	speed += incr;
        break;

      case 'n': // speed--
        speed -= incr;
        break;

      case '.': // first disengage then set speed=0
        if ( (status & B1100) != 0) { // if left or right are on then disengage both
          newstatus = status & B0011;
	}
	else { // else turn speed off
	  speed = 0;
	  dif = 0;
	}
        break;

      case ',': // go straight by turning dif=0
        dif = 0;
        break;

      case 'u': // forward (current speed then increase)
        newstatus = B1111;
        if (newstatus == status) speed += incr;
        break;

      case 'm': // forward (current speed then increase)
        newstatus = B1100;
        if (newstatus == status) speed -= incr;
        break;


	// **** start here not done
      case 'h': // left hard (left / right drive)
        if (speed == 0) {
          speed += incr;
	  dif = 0;
	  newstatus = B1011;
	}
	else {
          newstatus = B1100;
	}
        break;

      case 'j': // left hard (left / right drive)
        if (speed == 0) {
          speed += incr;
	  dif = 0;
	  newstatus = B1011;
	}
	else {
          newstatus = B1100;
	}
        break;

      case 'k': // left hard (left / right drive)
        if (speed == 0) {
          speed += incr;
	  dif = 0;
	  newstatus = B1011;
	}
	else {
          newstatus = B1100;
	}
        break;

      case 'l': // left hard (left / right drive)
        if (speed == 0) {
          speed += incr;
	  dif = 0;
	  newstatus = B1011;
	}
	else {
          newstatus = B1100;
	}
        break;

      default:  // turn all the connections off if an unmapped key is pressed:
	speed = 0;
	dif = 0;
	newstatus = B0000;
    }
  }
}


int drive(int speed, int dif, int newstatus ) {

  if (speed > 255) speed = 255;
  if (speed < 0 ) speed = 0;
  if (dif > 255) dif = 255;

  if ((speed + dif) > 255) speed = 255 - dif;
  if ((speed + dif) < 0)

  analogWrite(speedPinA, 0);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinB, 0);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);



  //  string out = left right speed;
  Serial.println();
  Serial.println("   "); // Creates a blank line printed on the serial monitor

  }


