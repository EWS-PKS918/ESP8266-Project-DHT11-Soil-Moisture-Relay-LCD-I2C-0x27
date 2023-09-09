# ESP8266-Project-DHT11-Soil-Moisture-Relay-LCD-I2C-0x27

// Components required using this code

 - LCD ( Any Type, I2C, OLED or not )
 - DHT11/DHT22
 - Soil Moisture Sensor
 - Relay (5VDC - 30VDC)
 - GREEN LED
 - YELLOW LED
 - Water Pump
 - 2x Polarized Capacitor ( 16V 27uF)


NOTICE :
    - If you want to use custom LCD like OLED and Normal LCD, you need to configure the code a little bit
    - If you want to use DHT22 instead of DHT11, you might need to tweak the code a little bit and integrate it to the code

DHT22 Code :

```
#include <DHT.h>
#define DHTPIN 2  // Define the pin where the DHT22 is connected change with your own pin
#define DHTTYPE DHT22  // Define the type of DHT sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // Delay between sensor readings

  float humidity = dht.readHumidity();  // Read humidity
  float temperature = dht.readTemperature();  // Read temperature in Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("% Temperature: "));
  Serial.println(temperature);
}
```

LCD Code for people that is using other than 0x27 :

  OLED :

  - **I2C Interface**
```
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display(); // Display splashscreen
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();   // Clear the buffer
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Hello, World!"));

  display.display(); // Show the content on the display
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clear the buffer
}
```
 - **SPI Interface :**
```
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET, 10, 9, 8, 7, 6);

void setup() {
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display(); // Display splashscreen
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();   // Clear the buffer
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Hello, World!"));

  display.display(); // Show the content on the display
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clear the buffer
}
```
- **UART Interface :**
```
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

SoftwareSerial displaySerial(2, 3); // RX, TX

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &displaySerial, OLED_RESET);

void setup() {
  displaySerial.begin(9600);
  
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display(); // Display splashscreen
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();   // Clear the buffer
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Hello, World!"));

  display.display(); // Show the content on the display
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clear the buffer
}
```

- **LCD I2C**:
```
 #include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x20  // Change this to the correct address of your LCD
#define LCD_COLS    16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Hello, World!");
}

void loop() {
  // Your code here
}


```
 - **Normal LCD (Not I2C) :**
```
  #include <LiquidCrystal.h>

// Define the pins for RS, Enable, and data lines (D4-D7)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Hello, World!");
}

void loop() {
  // Your code here
}
```
