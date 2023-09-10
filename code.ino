
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET LED_BUILTIN  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SPI.h>

const char* ssid = "devolo-4cc"; //enter your wifi network name
const char* password = "SWESRFMRILBEEUZJ"; //enter your wifi password

//"BPBKCEAYREAEZMGZ"

//const char* ssid     = "USH 2015 signal";
//const char* password = "7112zoel";

const char* resource = "/trigger/test/with/key/bkaP1dZ_-uQK9KgTx0oL3O"; //enter IFTTT key
const char* server = "maker.ifttt.com";

String TimeShowFormatted(long ms) { //formats milliseconds to hh:mm:ss
  long days1 = 0;
  long hours1 = 0;
  long mins = 0;
  long secs = 0;
  String secs_o = ":";
  String mins_o = ":";
  String hours_o = ":";
  secs = ms / 1000;                // set the seconds remaining
  mins = secs / 60;                //convert seconds to minutes
  hours1 = mins / 60;              //convert minutes to hours
  days1 = hours1 / 24;             //convert hours to days
  secs = secs - (mins * 60);       //subtract the coverted seconds to minutes in order to display 59 secs max
  mins = mins - (hours1 * 60);     //subtract the coverted minutes to hours in order to display 59 minutes max
  hours1 = hours1 - (days1 * 24);  //subtract the coverted hours to days in order to display 23 hours max

  if (secs < 10) {
    secs_o = ":0";
  }
  if (mins < 10) {
    mins_o = ":0";
  }
  if (hours1 < 10) {
    hours_o = ":0";
  }

  String finalString = (String(hours1) + mins_o + String(mins) + secs_o + String(secs));
  Serial.print("finalString :");
  Serial.println(finalString);
  return (String(hours1) + mins_o + String(mins) + secs_o + String(secs));
}


void oledWriteWords(String text) {//write one line of big text on OLED
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(text);
  display.display();
}

void oledWrite4lines(String text, String text2, String text3, String text4) { //write 4 lines of text on the OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.println(text2);
  display.println(text3);
  display.println(text4);
  display.display();
  Serial.print(text);
  Serial.println(text2);
  Serial.print(text3);
  Serial.println(text4);
}

void makeIFTTTRequest(String bedElapsed, String sleepElapsed, String unsleepElapsed) { //upload data to google sheets via IFTTT
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
    oledWrite4lines("Sheets", "fail", " ", " ");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);


  String jsonObject = "{\"value1\":\"" + bedElapsed + "\",\"value2\":\"" + sleepElapsed + "\",\"value3\":\"" + unsleepElapsed + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
  Serial.println(jsonObject);


  int timeout = 5 * 10;  // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}


void initWifi() { //connect to wifi
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4;  // 10 seconds
  while (WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
    oledWrite4lines("WIFI", "FAIL", " ", " ");
  }

  Serial.print("WiFi connected in: ");
  Serial.print(millis());
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connected to internet");
}

//pins for a ESP-8266 Node MCU
int D5 = 14;
int D6 = 12;
const int pirPin = D5;
const int pir2Pin = D6;

// definiing some variables
int bedState,pirState,pir2State;

// maximum length of time the sleep timer can be triggered after the most recent trigger:
unsigned long timeLimit = 2400000; 

long start1, finished, currentClock, bedStart;
String bedElapsed,elapsed,unsleepElapsed;
bool inBed = false;
bool done = false;
bool sent = false;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(pir2Pin, INPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Don't proceed, loop forever
  }
  initWifi();
}

void loop() {

  bedState = analogRead(A0); // read potentiometer value
  pirState = digitalRead(pirPin); // read pir sensor values
  pir2State = digitalRead(pir2Pin);
  currentClock = millis() - start1; //update sleep timer duration

  
  if (bedState > 1000 && done == false) { // start In-Bed
    bedStart = millis();
    start1 = millis();
    Serial.println("In bed");
    oledWriteWords("In bed");
    delay(1000);
    inBed = true;
    display.clearDisplay();
    display.display();
    done = true;
    sent = false;

  }
  else if (bedState > 621 && bedState < 999){ // ready to reset
    done = false;
    delay(400);
    bedState = analogRead(A0);
    Serial.println("Reset");
    oledWrite4lines("bed", "ready", "to", "reset");
    delay(700);
    display.clearDisplay();
    display.display();
    
  }

  else if (pirState == HIGH && pir2State == HIGH && currentClock < timeLimit && inBed == true)  { // sleep timer triggered, if before time limit and in bed
 
    
    start1 = millis();
    Serial.println("Sleeping");
    oledWriteWords("Sleepin");
    digitalWrite(LED_BUILTIN, LOW);
    delay(350);
    display.clearDisplay();
    display.display();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(4300);
  }

  else if (bedState < 620)  { // Send-Datato Google Spreadsheet
    finished = millis();
    elapsed = TimeShowFormatted((finished - start1) - 600000 ); // time from last sleep indicator, minus 10 mins 
    bedElapsed = TimeShowFormatted(finished - bedStart); // time in bed
    unsleepElapsed = TimeShowFormatted((finished - bedStart) - ((finished - start1) - 600000 ));
    inBed = false;
    if (sent == false) {
      makeIFTTTRequest(elapsed, bedElapsed, unsleepElapsed);
      //makeIFTTTRequest("elapsed", "bedElapsed", "unsleepElapsed");
  
      sent = true;
    }
    delay(1000);
    while (bedState < 620) {
      oledWrite4lines("sleep:", elapsed, "bed:", bedElapsed);
      delay(3000);
      oledWrite4lines("time to", "fall", "asleep:", unsleepElapsed);
      delay(3000);
      bedState = analogRead(A0);
    }

    start1 = millis();
    done = false;

  }


}
