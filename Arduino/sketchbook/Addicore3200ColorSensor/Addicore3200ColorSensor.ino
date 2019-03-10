/*
	Demonstrates the TCS230/TCS3200 Color Sensor Recognition Module
	using Energia and the RGB LED on the MSP432 LaunchPad.
	Tested: MSP432 EMT using Energia V16

	Don't expect a whole lot from this sketch (or the TCS3200 variants
	for that matter).  It can recognize primary colors and simple mixes.
	This sketch cannot output white, black, grey, etc.  The sensor output
	changes, sometimes dramatically, with changing incident light, material
	reflectivity, material orientation, size, distance, your individual
	sensor, etc.

	The sensor works by reading light from photodiodes and gives an output
	for clear, red, green, and blue.  The output is in the form of a
	square wave with color a function of the wavelength (see datasheet).
	The shorter the pulse, the higher the incidence of that color. The
	sensor has different responsiveness to different wavelengths and
	this must be adjusted for.

	This sketch uses pulseIn() to measure the frequency for each color and
	then adjusts for frequency responsiveness with an empirical factor, Rx.
	Factors were developed using colored "card" paper.  Empirically, it
	also appeared that subtracting the clear reading out from each color
	helped.  The colors are then sorted to determine which colors are the
	primary contributors and adjusted  so they looked reasonable on a scale
	of 0 to 255.  It is implied by this algorithm that the colors in the
	calibration sample are mostly made of two colors with one dominant color.

	Try adjusting the values Rc, Rg, Rb, and Rg first if you are not getting
	good results.  After that, try adjusting the factors/algorithm in the
	scaling section.

	This sketch was also tried on the MSP430F5529 and it "worked" but gave
	poor results, probably due to pulseIn() inaccuracies in the F5529
	implementation.  A better approach would be to use a timer and
	count interrupts.

	Pin Connections
	---------------
	Sensor    LaunchPad   Comment
	VCC       3V3
	GND       GND
	OUT       2  22         Output is square wave
	S2        3  24         Photodiode type selection
	S3        4  26         Photodiode type selection
	OE        11 53         Output enable (active low)
	S1        12 51         Output frequency scaling selection
	S0        13 49         Output frequency scaling selection

	Selectable options
	------------------
	S0    S1    Output Frequency Scaling
	L     L     Power down
	L     H     2%
	H     L     20%
	H     H     100%

	Note: According to the datasheet, scaled-down outputs may be used where
	a slower frequency counter is available. The pulseIn() functions seems
	to work OK on the MSP432 with frequency scaling set at 20%.

	s2    S3    Photodiode type selected
	L     L     Red
	L     H     Blue
	H     L     Clear (no filter)
	H     H     Green

	Frank Milburn 7 August 2015
	Released into the public domain
*/

// pin declarations
int outPin = 22;
int S2 = 24;
int S3 = 26;
int OEPin = 53;
int S1 = 51;
int S0 = 49;
int RED_LED = 13;
int GREEN_LED = 11;
int BLUE_LED = 12;

// constants - adjust if getting poor readings
const int Rc = 100;                      // Clear Relative Responsivity
const int Rr = 99;                       // Red Relative Responsivity
const int Rg = 65;                       // Green Relative Responsivity
const int Rb = 70;                       // Blue Relative Responsivity

void setup()
{
	Serial.begin(115200);
	pinMode(outPin, INPUT);
	pinMode(OEPin, OUTPUT);
	digitalWrite(OEPin, LOW);              // Enable frequency scaling
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	digitalWrite(S0, HIGH);                // set frequency scaling of sensor
	digitalWrite(S1, LOW);                 // set frequency scaling of sensor
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(RED_LED, OUTPUT);              // R on RGB LED of MSP432
	pinMode(GREEN_LED, OUTPUT);            // G on RGB LED of MSP432
	pinMode(BLUE_LED, OUTPUT);             // B on RGB LED of MSP432
}

void loop()
{
	//------------------ Read raw values and correct them ----------------------
	digitalWrite(S2, HIGH);                // get ready to read clear
	digitalWrite(S3, LOW);
	int cPulse = pulseIn(outPin, LOW);     // read clear
	Serial.print("C pulse = ");
	Serial.println(cPulse);
	int Ac = cPulse * Rc;                  // adjust reading for responsiveness

	digitalWrite(S2, LOW);                 // get ready to read red
	digitalWrite(S3, LOW);
	int rPulse = pulseIn(outPin, LOW);     // read red
	Serial.print("R pulse = ");
	Serial.println(rPulse);
	int Ar = rPulse * Rr;                  // adjust reading for responsiveness
	int Cr = Ar - Ac;                      // correct for clear reading

	digitalWrite(S2, HIGH);                // get ready to read green
	digitalWrite(S3, HIGH);
	int gPulse = pulseIn(outPin, LOW);     // read green
	Serial.print("G pulse = ");
	Serial.println(gPulse);
	int Ag = gPulse * Rg;                  // adjust reading for responsiveness
	int Cg = Ag - Ac;                      // correct for clear reading

	digitalWrite(S2, LOW);                 // get ready to read blue
	digitalWrite(S3, HIGH);
	int bPulse = pulseIn(outPin, LOW);     // read blue
	Serial.print("B pulse = ");
	Serial.println(bPulse);
	int Ab = bPulse * Rb;                  // adjust reading for responsiveness
	int Cb = Ab - Ac;                      // correct for clear reading

	//-------------- Find relative order of colors and scale to 255 ------------------
	//-------------- Algorithym is empirical                        ------------------
	int r;
	int g;
	int b;
	if (Cr < Cg && Cg < Cb)                //   RGB is color order
	{
		r = 255;
		g = 128 * Cr / Cg;
		b = 16 * Cr / Cb;
	}
	else if (Cr < Cb && Cb < Cg)           //   RBG is color order
	{
		r = 255;
		b = 128 * Cr / Cb;
		g = 16 * Cr / Cg;
	}
	else if (Cg < Cr && Cr < Cb)           //   GRB is color order
	{
		g = 255;
		r = 128 * Cg / Cr;
		b = 16 * Cg / Cb;
	}
	else if (Cg < Cb && Cb < Cr)           //   GBR is color order
	{
		g = 255;
		b = 128 * Cg / Cb;
		r = 16 * Cg / Cr;
	}
	else if (Cb < Cr && Cr < Cg)           //   BRG is color order
	{
		b = 255;
		r = 128 * Cb / Cr;
		g = 16 * Cb / Cg;
	}
	else // (Cb < Cg && Cg < Cr)           //   BGR is color order
	{
		b = 255;
		g = 128 * Cb / Cg;
		r = 32 * Cb / Cr;
	}
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	//----------------------------------- Output ------------------------------------
	Serial.println("");
	Serial.print("r = "); Serial.println(r);
	Serial.print("g = "); Serial.println(g);
	Serial.print("b = "); Serial.println(b);
	Serial.println("");
	analogWrite(RED_LED, 255 - r);
	analogWrite(GREEN_LED, 255 - g);
	analogWrite(BLUE_LED, 255 - b);
	delay(1000);
}
