/*
    Write your notice here :
*/

/* Physical Pin Connection
RELAY_INPUT :     D3 / Your own pin connection
SOIL_SENSOR :     A0 / Your own pin connection
LCD_I2C_SDA :     D2 / Your own pin connection
LCD_I2C_SCL :     D1 / Your own pin connection
DHT_SENSOR  :     D4 / Your own pin connection
LED         :     D6 / Your own pin connection
*/


/* =================================================================================================================================================== */
#define BLYNK_TEMPLATE_ID ""                      // Change this with your Tmeplate ID
#define BLYNK_TEMPLATE_NAME ""                    // Change this with you template name
#define BLYNK_AUTH_TOKEN ""                       // Change this with you auth token

#include <Wire.h>                                 // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>                    // Include LCD library for I2C communication
#include <ESP8266WiFi.h>                          // Include ESP8266WiFi library
#include <BlynkSimpleEsp8266.h>                   // Include Blynk library for ESP8266
#include <DHT.h>                                  // Include DHT library for DHT11 sensor

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);               // Replace with your own type of LCD's I2C address

String newHostname = "";       // Change the hostname with you desire

char auth[] = "";  // Enter your Blynk Auth token
char ssid1[] = "";                             // Enter your primary WIFI SSID
char pass1[] = "";                             // Enter your primary WIFI Password
char ssid2[] = "";            // Fallback WIFI SSID | leave blank if you're not using it
char pass2[] = "";            // Fallback WIFI Password | leave blank if you're not using it

DHT dht(D4, DHT11);                                // Initialize DHT sensor on pin D4 as DHT11 Temperature Sensor
BlynkTimer timer;

// Define component pins
#define RELAY_PIN_1             D3     // Define Relay pin as D3
#define soilMoisturePin         A0     // Define Soil Moisture Sensor pin as A0
#define LED_PIN                 D6     // Define LED pin as D6
#define ERR_LED_PIN             D5     // Define Error LED pin as D5
int relay1State = LOW;

void setup() {
  WiFi.hostname(newHostname.c_str());  // Set ESP8266 hostname

  pinMode(LED_PIN, OUTPUT);             // Set GREEN LED as OUTPUT
  pinMode(ERR_LED_PIN, OUTPUT);         // Set YELLOW LED as OUTPUT
  Wire.begin();                         // Initialize the I2C communication
  Serial.begin(9600);                   // Initialize the Serial Monitor
  lcd.init();                           // Initialize LCD display
  lcd.backlight();                      // Turn on backlight

  pinMode(RELAY_PIN_1, OUTPUT);         // Set Relay pin as OUTPUT
  
  dht.begin();                          // Initialize DHT sensor
  lcd.setCursor(0, 0);
  lcd.print("  Starting  Up  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print("-");
    delay(500);
  }
  lcd.clear();
  
  // Call the function to read soil moisture and update Blynk
  timer.setInterval(70L, soilMoistureSensor);

  // Call the function to read temperature and humidity and update Blynk
  timer.setInterval(100L, DHT11sensor);

  // Connect to Wi-Fi networks
  connectToWiFi();

  // Begin Blynk with authentication and server details
  Blynk.begin(auth, ssid1, pass1, "blynk.cloud", 80);
}

void loop() {
  // Run Blynk operations
  Blynk.run();

  // Run timed functions
  timer.run();
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    digitalWrite(ERR_LED_PIN, HIGH);
    delay(500 / 2);
    digitalWrite(ERR_LED_PIN, LOW);
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
}

void soilMoistureSensor() {
  int value = analogRead(soilMoisturePin);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Blynk.virtualWrite(V3, value);
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);
  lcd.print(" ");
  if (value < 30){
    digitalWrite(RELAY_PIN_1, HIGH);
    digitalWrite(ERR_LED_PIN, HIGH);
  }else{
        digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(ERR_LED_PIN, LOW);
  }
}

void connectToWiFi() {
  int attempts = 0;

  // First, attempt to connect to the primary WiFi network
  WiFi.begin(ssid1, pass1);
  Serial.print("\nConnecting to " + String(ssid1) + "...");

  for (int i = 1; i <= 10; i++) {
    delay(1000);
    attempts++;
    lcd.setCursor(0, 0);
    lcd.print("Connecting to WiFi");
    lcd.setCursor(7, 1);
    lcd.print(i);
    delay(500);
    Serial.print(".");
  }

  // If not connected to the primary WiFi network, try the fallback network
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to " + String(ssid1));
    WiFi.begin(ssid2, pass2);
    Serial.print("\nConnecting to " + String(ssid2) + "...");
    attempts = 0;
    for (int i = 1; i <= 10; i++) {
      delay(1000);
      attempts++;
      lcd.setCursor(0, 0);
      lcd.print("Connecting to WiFi");
      lcd.setCursor(7, 1);
      lcd.print(i);
      delay(500);
      Serial.print(".");
    }
  }

  // Display the connection status
  if (WiFi.status() == WL_CONNECTED) {
    if (WiFi.SSID() == ssid1) {
      Serial.println("\nConnected to " + String(ssid1) + " \n");
      digitalWrite(LED_PIN, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Connected To");
      lcd.setCursor(0, 1);
      lcd.print(ssid1);
      delay(1500);
      lcd.clear();
    } else if (WiFi.SSID() == ssid2) {
      Serial.println("\nConnected to " + String(ssid2) + " \n");
      digitalWrite(LED_PIN, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Connected To");
      lcd.setCursor(0, 1);
      lcd.print(ssid2);
      delay(1500);
      lcd.clear();
    }
  } else {
    Serial.println("\nFailed to connect to any network\n");
    digitalWrite(ERR_LED_PIN, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Unable To");
    lcd.setCursor(0, 1);
    lcd.print("Connect to WiFi");
    delay(1500);
    lcd.clear();
  }
}