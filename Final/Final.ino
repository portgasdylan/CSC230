#include <M5Core2.h>
#include <WiFi.h>
#include "secrets.h" //this is another file
#include "ThingSpeak.h"
#include <HTTPClient.h>

//Button Setup
const int BUTTON_PIN = 33;
bool lastButtonState = HIGH;
bool screenToggled = false;

//LED pin definitions
const int LED1 = 27;
const int LED2 = 19;
const int LED3 = 25;

int tallyCount = 0;
const char* GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbwvKDwWmLfmNyGZ6dOZGpa-fWbUgHcv1a8oPj6BKUHbP8K4uI_J-jUy-nDmizIu8NRS/exec";
char ssid[] = SECRET_SSID;   // your network SSID (name)       this will all be found in secrets.h!
char pass[] = SECRET_PASS;   // your network password 
int keyIndex = 0;
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Define colors (can add more)
uint32_t colors[] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
int numColors = sizeof(colors) / sizeof(colors[0]);

int currentIndex = 0;
unsigned long lastChangeTime = 0;
bool colorSent = false;

void updateColor();               // Function declaration
void sendColorToThingSpeak();     // Function declaration

void sendTallyToGoogle(int count) {
  HTTPClient http;
  String url = String(GOOGLE_SCRIPT_URL) + "?count=" + String(count);
  
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    Serial.println("Tally sent to Google Sheets");
  } else {
    Serial.print("Failed to send tally. HTTP code: ");
    Serial.println(httpCode);
  }
  http.end();
}

void setup() {
  M5.begin();
  Serial.begin(115200);

  //Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //LED setup
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  //Turn LEDs off initially
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  //Turn LEDs one by one
  delay(500);
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED3, HIGH);

  
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Connecting WiFi...");

   WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
    Serial.println("\nConnected.");
    M5.Lcd.println("WiFi connected.");

    ThingSpeak.begin(client);
    updateColor();  // Show the first color
  }



void loop(){
  M5.update();
 if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.begin(ssid, pass);
    delay(5000);
    return;  // skip rest of loop until reconnected
  }

  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  //Button press detection
  if (buttonPressed && lastButtonState == HIGH) {
    currentIndex = (currentIndex + 1) % numColors;
    updateColor();

    tallyCount++;
    sendTallyToGoogle(tallyCount);
    Serial.println("Button pressed. Tally sent.");

    delay(250);
  }

  //update last button state
  lastButtonState = !buttonPressed;



  // Cycle through colors with Button A
  if (M5.BtnA.wasPressed()) {
    currentIndex = (currentIndex + 1) % numColors;
    updateColor();
  }

  if (M5.BtnA.wasPressed()) {
  tallyCount++;
  sendTallyToGoogle(tallyCount);
  updateColor();
}

  // If on a color for more than 5 seconds, send to ThingSpeak
  if (!colorSent && millis() - lastChangeTime > 5000) {
    sendColorToThingSpeak();
    colorSent = true;
  }
}

 


void updateColor() {
  uint32_t color = colors[currentIndex];
  M5.Lcd.fillScreen(color);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK, color);  // Contrast
  M5.Lcd.printf("Color: #%06X", color);
  lastChangeTime = millis();
  colorSent = false;
}

void sendColorToThingSpeak() {
  uint32_t color = colors[currentIndex];
  int x = ThingSpeak.writeField(myChannelNumber, 1, (long)color, myWriteAPIKey);

  if (x == 200) {
    Serial.print("Sent color to ThingSpeak: #");
    Serial.println(color, HEX);
  } else {
    Serial.print("Failed to send. HTTP error code: ");
    Serial.println(x);
  }
}


