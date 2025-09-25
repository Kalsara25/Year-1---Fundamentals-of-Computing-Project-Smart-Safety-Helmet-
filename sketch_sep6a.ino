#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// 🟢 Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// 🟢 WiFi credentials
#define WIFI_SSID "Dialog 4G"
#define WIFI_PASSWORD "4MJNYYNK2ER"

// 🟢 Firebase project config
#define API_KEY "AIzaSyDgjXrb_6NlLUgIda7BxB-X77eqYlXCguo"
#define DATABASE_URL "https://gas-safty-monitor-default-rtdb.firebaseio.com"  
#define USER_EMAIL "malishakalsara4@gmail.com"
#define USER_PASSWORD "Your_Firebase_User_Password"

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXD2 16   // GPS TX -> ESP32 RX
#define TXD2 17   // GPS RX -> ESP32 TX

// Create GPS object
HardwareSerial SerialGPS(2);
TinyGPSPlus gps;

// --- Pin Definitions ---
#define IR_PIN 14
#define RED_LED 18
#define GREEN_LED 19
#define BUZZER 23
#define DHTPIN 33
#define DHTTYPE DHT22
#define MQ135_PIN 34
#define PULSE_PIN 35

// --- Objects ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
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
  SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);

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

  // 🟢 Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // 🟢 Firebase Config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // --- Read Sensors ---
  int irValue = digitalRead(IR_PIN);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int GasValue = analogRead(MQ135_PIN);

  // Pulse sensor
  sensorValue = analogRead(PULSE_PIN);
  baseline = (baseline * 15 + sensorValue) / 16;

  if (sensorValue > baseline + 100 && !beatDetected) {
    beatDetected = true;
    unsigned long now = millis();
    int delta = now - lastBeat;
    lastBeat = now;

    if (delta > 300 && delta < 2000) {
      int instantBPM = 60000 / delta;
      bpmArray[bpmIndex] = instantBPM;
      bpmIndex = (bpmIndex + 1) % BPM_BUFFER;

      int sum = 0;
      for (int i = 0; i < BPM_BUFFER; i++) sum += bpmArray[i];
      bpm = sum / BPM_BUFFER;
    }
  }
  if (sensorValue < baseline + 50) {
    beatDetected = false;
  }

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" ");

  lcd.setCursor(8, 0);
  lcd.print("G:");
  lcd.print(GasValue);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("BPM:");
  lcd.print(bpm);
  lcd.print("   ");

  // GPS Reading
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        Serial.println("LOCATION:");
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }

  // Helmet Check
  if (irValue == LOW) {
    digitalWrite(GREEN_LED, HIGH);
    Serial.println("Helmet Worn by the worker");

    Serial.print("Temperature: "); Serial.println(temp);
    Serial.print("Gas Value: "); Serial.println(GasValue);
    Serial.print("BPM: "); Serial.println(bpm);

    // Upload to Firebase
    if (Firebase.ready()) {
      Firebase.RTDB.setFloat(&fbdo, "helmets/15/temperature", temp);
      Firebase.RTDB.setFloat(&fbdo, "helmets/15/humidity", hum);
      Firebase.RTDB.setInt(&fbdo, "helmets/15/gas", GasValue);
      Firebase.RTDB.setInt(&fbdo, "helmets/15/bpm", bpm);
      Firebase.RTDB.setBool(&fbdo, "helmets/15/helmetWorn", true);
    }

    if (temp > 33 || GasValue > 2400 || bpm > 140) {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BUZZER, HIGH);
      Serial.println("⚠️ WARNING! Unsafe Environment");
    } else {
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, LOW);
      Serial.println("✅ Safe Environment");
    }
  } else {
    digitalWrite(GREEN_LED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please Wear");
    lcd.setCursor(0, 1);
    lcd.print("Safety Helmet");
    Serial.println("Helmet is NOT Worn by the worker");
    delay(1000);
  }

  delay(200); // Sampling delay
}
