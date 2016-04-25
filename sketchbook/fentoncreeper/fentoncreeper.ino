// fenton creeper robot
// arduino uno based four wheel drive robot
// tsj sej 2016 04 25


char* Instructions[]={
  "Keyboard Controls:",
  "binary status B0000   left [on/off dir(+/-)] right [on/off dir(+/-)]",
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
  "  ";
  " (left motors)",
  " 1 -Motor 1 Left",
  " 2 -Motor 1 Stop",
  " 3 -Motor 1 Right",
  " (right motors)",
  " 4 -Motor 2 Left",
  " 5 -Motor 2 Stop",
  " 6 -Motor 2 Right",
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
  Serial.write();
  for (int i = 0; i < 21; i++){
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

void loop(int speed = 0, int dif = 0, int status = B000 ) {

  // Initialize the Serial interface:

  if (Serial.available() > 0) {

    int inByte = Serial.read();
    newstatus = status;

    switch (inByte) {

      case 'y': // speed++
	speed += incr;
        break;

      case 'n': // speed--
        speed -= incr;
        break;

      case '.': // speed=0
        if ( (status & B1100) != 0) {
          newstatus = status & B0011;
	}
	else {
	  speed = 0;
	  dif = 0;
	}
        break;

      case ',': // dif=0
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
    status = drive(speed, dif, newstatus);
  }
}


int drive(int left, int right, int speed, int left, int right, int dif ) {
  if (speed > 255) speed = 255;
  if (speed < 0 ) speed = 0;
  
  if ((speed + dif) > 255) left = 255 - dif;
  if ((speed + dif) < 0)
  string out = left right speed;

  Serial.println(out);
  Serial.println("   "); // Creates a blank line printed on the serial monitor

  }


