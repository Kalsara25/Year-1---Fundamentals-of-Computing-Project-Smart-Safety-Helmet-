#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXD2 16   // GPS TX -> ESP32 RX (GPIO16)
#define TXD2 17   // GPS RX -> ESP32 TX (GPIO17) [optional]

// Create GPS object
HardwareSerial SerialGPS(2);
TinyGPSPlus gps;


// --- Pin Definitions ---
#define IR_PIN 14          // IR sensor digital output
#define RED_LED 18        // Red LED
#define GREEN_LED 19       // Green LED
#define BUZZER 23         // Buzzer
#define DHTPIN 33          // DHT22 data pin
#define DHTTYPE DHT22         // Sensor type
#define MQ135_PIN 34   
#define PULSE_PIN 35

// --- Objects ---
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your LCD I2C address
DHT dht(DHTPIN, DHTTYPE);

int sensorValue = 0;
int baseline = 0;
bool beatDetected = false;

unsigned long lastBeat = 0;
int bpm = 0;

// Moving average buffer for smoothing
#define BPM_BUFFER 5
int bpmArray[BPM_BUFFER] = {0,0,0,0,0};
int bpmIndex = 0;

void setup() {

  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  pinMode(IR_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();


  dht.begin();

  
 
}
void loop() {
  
 Serial.begin(115200);

// --- Read Sensors ---
  int irValue = digitalRead(IR_PIN);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int GasValue = analogRead(MQ135_PIN);

 // Read raw sensor value
  sensorValue = analogRead(PULSE_PIN);

  // Update dynamic baseline (moving average)
  baseline = (baseline * 15 + sensorValue) / 16;

  // Peak detection
  if (sensorValue > baseline + 100 && !beatDetected) {
    beatDetected = true;
    unsigned long now = millis();
    int delta = now - lastBeat;
    lastBeat = now;

    if (delta > 300 && delta < 2000) {  // valid BPM range
      int instantBPM = 60000 / delta;

      // Add to moving average buffer
      bpmArray[bpmIndex] = instantBPM;
      bpmIndex = (bpmIndex + 1) % BPM_BUFFER;

      // Calculate smoothed BPM
      int sum = 0;
      for (int i = 0; i < BPM_BUFFER; i++) sum += bpmArray[i];
      bpm = sum / BPM_BUFFER;
    }
  }

  if (sensorValue < baseline + 50) {
    beatDetected = false;
  }

 
 

  delay(100); // 50Hz sampling

   lcd.setCursor(0, 0);
   lcd.print("T:");
   lcd.print(temp);
   lcd.print(" ");

   lcd.setCursor(8, 0);
   lcd.print("G:");
   lcd.print(GasValue );
   lcd.print(" ");

   lcd.setCursor(0, 1);
   lcd.print("BPM:");
   lcd.print(bpm);
   lcd.print("          ");

{
    while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        Serial.println();
        Serial.println("LOCATION     ");
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.println();
  }
}
}
}
  

  if (irValue == LOW) {
    digitalWrite(GREEN_LED, HIGH);
    Serial.println();
    Serial.println("THE Helmet Worn by the worker  ");

    

        Serial.print("Temprature-  ");
        Serial.print(temp);
        Serial.println();

        Serial.print("Gas Value-  ");
        Serial.print(GasValue);
        Serial.println();

        Serial.print("BPM-  ");
        Serial.print(bpm);
        Serial.println();

    if (temp > 353|| GasValue > 3000 || bpm > 160){
     
     digitalWrite(RED_LED, HIGH);
     digitalWrite(GREEN_LED, LOW);
     digitalWrite(BUZZER, HIGH);
     Serial.println("WARNING! Unsafe Environment");
     
   } else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    Serial.println("Safe Environment");
   
   }

  } else {
    delay(1000);
    lcd.clear();
     
    digitalWrite(GREEN_LED, LOW);
    Serial.println();
    Serial.println("Helmet is not Worn by the worker");
    lcd.setCursor(0, 0);
    lcd.print("Please_wear_the  ");
    
    lcd.setCursor(0, 1);
    lcd.print("Safety_Helmet   ");
    delay (1000);
  
  }

}