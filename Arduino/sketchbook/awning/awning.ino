/*********
          Awning control & Living room temperature

          init   SeJ 03 09 2019 specifics to my application
          update SeJ 03 24 2019 change to relay
*********/

#include <../../../../../../../../../Projects/keys/sej/sej.h>
#include <ESP8266WiFi.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);// Pass our oneWire reference to Dallas Temperature.
DallasTemperature DS18B20(&oneWire);
char temperatureCString[7];
char temperatureFString[7];
char outstr[7];
float tempC;
float tempF;
float tempOld;

// Web Server on port 80
WiFiServer server(80);

const char* tempresource = "/rest/vars/set/2/69/";
const char* heartbeatresource = "/rest/vars/set/2/70/";
const char* awningcontrolresource = "/rest/vars/set/2/71/";
float heartbeat = 0; // heartbeat to ISY

// time
unsigned long tempMillis = 0;
unsigned long getisyMillis = 0;
unsigned long resetwifiMillis = 0;
unsigned long awningcontrolMillis = 0;
long tempInterval = 30000;
long getisyInterval = 120000;
long resetwifiInterval = 60000;
long awningcontrolInterval = 30000;
int control = true;
int awningpushtime = 2000;

// pins
int pbpower = 0; // Pin D2 GPIO 0
int pbbutton = 4; // Pin D3 GPIO 4
int relaypower = 2; // Pin D4 GPIO 2

/*
 * Setup
 */
void setup(){
	Serial.begin(115200);
	delay(10);

  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

  initWifi();
  getTemperature();
  tempOld = 0;
  control = true;

  // prepare GPIO4
  pinMode(pbpower, OUTPUT);
  digitalWrite(pbpower, HIGH);
  delay(1);
  pinMode(pbbutton, OUTPUT);
  digitalWrite(pbbutton, HIGH);
  delay(1);
  pinMode(relaypower, OUTPUT);
  digitalWrite(relaypower, LOW);
}

void loop(){
  int val;
  unsigned long currentMillis = millis();
/*
 * Temperature retrieve from DS18B20
 */
	if(currentMillis - tempMillis > tempInterval) {
    tempMillis = currentMillis;
  getTemperature();
	}

/*
 * ISY Temperature update
 */
  if(currentMillis - getisyMillis > getisyInterval){
    getisyMillis = currentMillis;
    if (tempOld != tempF){
      makeHTTPRequest(tempresource,tempF);
      Serial.print("Updating ISY with ");
      Serial.println(temperatureFString);
    }
  }

  /*
   * Awning Control only so often
   */
  if(currentMillis - awningcontrolMillis > awningcontrolInterval) {
    control = true;
  }

/*
 * Heartbeat to ISY & WIFI maintain
 */
  if(currentMillis - resetwifiMillis > resetwifiInterval) {
    resetwifiMillis = currentMillis;
    if(heartbeat == 0){
      heartbeat = 1;
    }
    else {
      heartbeat = 0;
    }
    makeHTTPRequest(heartbeatresource,heartbeat);
    if(WiFi.status() != WL_CONNECTED) {
      initWifi();
    }
}

if(WiFi.status() == WL_CONNECTED) {
  // Listenning for new clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");

    if (true) {
      // Read the first line of the request
      String s = client.readStringUntil('\r');
      Serial.println(s);
      client.flush();

      // Match the request
      if (s.indexOf("/awning/0") != -1) {
        val = true;
        Serial.print("request awning status: ");
        Serial.println(( (control) ? "true" : "false"));
      } else if (s.indexOf("/awning/1") != -1) {
        val = true;
        Serial.println("request awning control");
        if(control == true){
          digitalWrite(relaypower, HIGH);
          digitalWrite(pbpower, LOW);
          digitalWrite(pbbutton, LOW);
          Serial.println("**awning button pushed**");
          delay(awningpushtime);
          digitalWrite(pbbutton, HIGH);
          digitalWrite(pbpower, HIGH);
          digitalWrite(relaypower, LOW);
          Serial.println("**awning button released**");
          awningcontrolMillis = currentMillis;
          control = false;
        }
      }

      /*
       * Respond to HTTP GET
       */
      if(val){
        // Prepare the response
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nAwning control is now ";
        s += (control) ? "true" : "false";
        s += " --- Temperature: ";
        s += temperatureCString;
        s += "C  ";
        s += temperatureFString;
        s += "F  ---";
        s += "</html>\n";

        // Send the response to the client
        delay(1);
        client.print(s);
        val = false;
      }

      delay(1);
      client.flush();
      client.stop();
      Serial.println("Client disconnected");

    }
  }
}
}


// Subroutines:

// Establish a Wi-Fi connection with your router
void initWifi() {
	Serial.println("");
	Serial.print("Connecting to: ");
	Serial.print(ssid);
	WiFi.begin(ssid, password);

	int timeout = 25 * 4; // 25 seconds
	while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
		delay(250);
		Serial.print(".");
	}
	Serial.println("");

	if(WiFi.status() != WL_CONNECTED) {
		 Serial.println("WiFi Failed to connect");
	}
	else {
	Serial.print("WiFi connected in: ");
	Serial.print(millis());
	Serial.print(", IP address: ");
	Serial.println(WiFi.localIP());

	// Starting the web server
	server.begin();
	Serial.println("Web server running....");
	}
}

/*
 * GetTemperature from DS18B20
 */
void getTemperature() {
	do {
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    tempF = DS18B20.getTempFByIndex(0);
    dtostrf(tempF, 3, 2, temperatureFString);
    delay(100);
	} while (tempC == 85.0 || tempC == (-127.0));
	Serial.print("Jenkins Lake Temp:  ");
	Serial.print("Temperature in Celsius: ");
	Serial.print(temperatureCString);
	Serial.print("   Temperature in Fahrenheit: ");
	Serial.println(temperatureFString);
}


/*
 * Make an HTTP request to ISY
 */
void makeHTTPRequest(const char* resource, float data) {
	Serial.print("Connecting to ");
	Serial.print(isy);

	WiFiClient client;
	int retries = 5;
	while(!!!client.connect(isy, isyport) && (retries-- > 0)) {
		Serial.print(".");
	}
	Serial.println();
	if(!!!client.connected()) {
		 Serial.println("Failed to connect, going back to sleep");
	}

	Serial.print("Request resource: ");
	Serial.println(resource);
	dtostrf(data, 3, 2, outstr);
	client.print(String("GET ") + resource + outstr +
							 " HTTP/1.1\r\n" +
							 "Host: " + isy + "\r\n" +
							 "Connection: close\r\n" +
							 "Content-Type: application/x-www-form-urlencoded\r\n" +
							 "Authorization: Basic " + hash + "\r\n\r\n");

	Serial.println("request sent");
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		if (line == "\r") {
			Serial.println("headers received");
			break;
		}
	}
	String line = client.readStringUntil('\n');
	if (line.startsWith("<?xml version=\"1.0\" encoding=\"UTF-8\"?><RestResponse succeeded=\"true\"><status>200</status></RestResponse>")) {
		Serial.println("write to ISY successful!");
		tempOld != tempF;
	} else {
		Serial.println("write to ISY has failed");
	}
	Serial.println("reply was:");
	Serial.println("==========");
	Serial.println(line);
	Serial.println("==========");
	Serial.println("closing connection");
	client.stop();
}
