

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "max6675.h"
#include "DHT.h"

int systemon =13;
int warning = 9;
int fire = 11;
int sensing = 10;



#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
int relay = 7;


DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int thermoDO = 4; //pins for thermocouple
int thermoCS = 5;
int thermoCLK = 6;



MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
SoftwareSerial bt(0,1); // rx tx

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

pinMode(A0, INPUT);//zone1 lm35
pinMode(A1, INPUT); //zone2 lm35
pinMode(A2, INPUT);//zone3 lm35

pinMode(warning, OUTPUT);
pinMode(fire, OUTPUT);
pinMode(systemon, OUTPUT);
digitalWrite(systemon, HIGH);

pinMode(sensing, OUTPUT);
digitalWrite(sensing, HIGH);
dht.begin();

pinMode(relay, OUTPUT);
bt.begin(115200);

Wire.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("STARTING THE SYSTEM");
  display.display(); 
  delay(1000);
}

void loop() {
  display.clearDisplay();
digitalWrite(systemon, HIGH);

float currentTemp = 0;
   for (int f = 0; f < 25; f++) {
   
      currentTemp = thermocouple.readCelsius() + currentTemp;
    }
  currentTemp = currentTemp/25; // averaging thermocouple value by 25 for precise results.

  int zone1 = analogRead(A0);
  int zone2 = analogRead(A1);
  int zone3 = analogRead(A2);
  
  zone1 = zone1*500/1024;
  zone2 = zone2*500/1024;
  zone3 = zone3*500/1024;

float hum = dht.readHumidity();


  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  // Display static text
  
  display.print("Zone 1 :");
  display.println(zone1);

    display.print("Zone 2 :");
  display.println(zone2);
  
    display.print("Zone 3 :");
  display.println(zone3);

  display.print("Thermocouple : ");
  display.println(currentTemp);

  display.print("Humidity: ");
  display.println(hum);
    
  // Display static text
  
  Serial.print("Zone 1 :");
  Serial.println(zone1);

    Serial.print("Zone 2 :");
  Serial.print(zone2);
  
    Serial.print("Zone 3 :");
  Serial.print(zone3);

  Serial.print("Thermocouple : ");
  Serial.println(currentTemp);

  Serial.print("Humidity: ");
  Serial.println(hum);


  if((zone1>70)||(zone2>70)||(zone3>70)||(currentTemp>70))
  {
    display.clearDisplay();
    Serial.println("There's a Fire");
    display.println("There's a Fire");
    digitalWrite(fire, HIGH);
    digitalWrite(relay, HIGH);
    
    delay(5000);
    digitalWrite(relay, LOW);
    digitalWrite(fire, LOW);
    }

      if(((zone1>40)&&(zone1<40))||((zone2>40)&&(zone2<70))||((zone3>40)&&(zone3<70))||((currentTemp>40)&&(currentTemp<70)))
  {
    display.clearDisplay();
    Serial.println("WARNING, TEMPERATURE IS RISING");
    display.println("WARNING, TEMPERATURE IS RISING");
    digitalWrite(warning, HIGH);
    
    delay(5000);
    digitalWrite(warning, LOW);
    }

    if(hum<30)
    {
      display.println("Low Humidity, chances of fire");
      delay(1000);
    }
    
  display.display();   
delay(1000);

}
