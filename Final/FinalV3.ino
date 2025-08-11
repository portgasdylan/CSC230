#include <M5Core2.h>                 
#include <WiFi.h>               
#include "secrets.h"                 
#include "ThingSpeak.h"             
#include <HTTPClient.h>                // For sending HTTP requests to Google Apps Script

// Firebase
#include <Firebase_ESP_Client.h>       // Firebase client library
#include <addons/RTDBHelper.h>         // Helper functions for Firebase Realtime Database
#include <addons/TokenHelper.h>        // Helper functions for Firebase authentication tokens

// -------------------- FIREBASE CONFIG --------------------
FirebaseConfig fbConfig;               // Holds Firebase configuration (API key, database URL, etc.)
FirebaseAuth fbAuth;                   // Holds authentication credentials
FirebaseData fbdo;                     // Firebase data object for reading/writing data


// External push buttons
const int BUTTON_PIN1 = 33;            // External button 1 pin
const int BUTTON_PIN2 = 26;            // External button 2 pin
bool lastButtonState1 = HIGH;          // Tracks last state of button 1 (for debounce)
bool lastButtonState2 = HIGH;          // Tracks last state of button 2 (for debounce)

// Screen toggle state
bool screenToggled = false;            // Tracks whether the screen is on/off (currently unused)

// LED pins
const int LED1 = 27;                   // LED 1 pin
const int LED2 = 19;                   // LED 2 pin
const int LED3 = 25;                   // LED 3 pin


char ssid[] = SECRET_SSID;          
char pass[] = SECRET_PASS;         
WiFiClient client;              
unsigned long myChannelNumber = SECRET_CH_ID;        // ThingSpeak channel ID (from secrets.h)
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;    // ThingSpeak write API key (from secrets.h)

// Misc counters
int tallyCount = 0;                    // Tracks total tally count (currently unused)
int keyIndex = 0;                      // Not currently used 


uint32_t colors[] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE }; 
int numColors = sizeof(colors) / sizeof(colors[0]);                     // Number of colors
int currentIndex = 0;                                                     // Current color index
unsigned long lastChangeTime = 0;                                         // Last time color was changed
bool colorSent = false;                                                   // Has the color been sent to ThingSpeak yet?

// -------------------- PROFESSOR & PHRASE SETUP --------------------
const char* professors[] = { "ProfA", "ProfB", "ProfC" };  // List of professors
const size_t NUM_PROFS = sizeof(professors) / sizeof(professors[0]); // Number of professors

const char* phrases[] = { "phrase1", "phrase2" };           // List of phrases
const size_t NUM_PHRASES = sizeof(phrases) / sizeof(phrases[0]);     // Number of phrases

int currentProf = 0;                                       // Currently selected professor index
int phraseTally[NUM_PROFS][NUM_PHRASES] = {0};              // 2D array storing tallies for each professor & phrase

// Google App Script endpoint
const char* GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbwvKDwWmLfmNyGZ6dOZGpa-fWbUgHcv1a8oPj6BKUHbP8K4uI_J-jUy-nDmizIu8NRS/exec";


void updateColor();                // Updates M5Core2 screen color & professor label
void sendColorToThingSpeak();      // Sends current color to ThingSpeak
void sendTallyToGoogle(const char* prof, const char* phrase, int count);  // Sends tally to Google Sheets
void sendTallyToFirebase(const char* prof, const char* phrase, int count); // Sends tally to Firebase


void setup() {
  M5.begin();                     
  Serial.begin(115200);             

  // LED setup
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Turn LEDs off initially
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  // Turn LEDs on one by one with delay
  delay(500); digitalWrite(LED1, HIGH);
  delay(500); digitalWrite(LED2, HIGH);
  delay(500); digitalWrite(LED3, HIGH);

  // Button setup
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);

  // Display Wi-Fi connection message
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Connecting WiFi...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected.");
  M5.Lcd.println("WiFi connected.");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Firebase setup
  fbConfig.api_key = FIREBASE_API_KEY;
  fbConfig.database_url = FIREBASE_DATABASE_URL;
  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);

  // Show first color on screen
  updateColor();
}


void loop(){
  M5.update();

  // Reconnect Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.begin(ssid, pass);
    delay(1000);
    return; // Skip loop until reconnected
  }

  // --- External Button 1 ---
  bool b1 = digitalRead(BUTTON_PIN1) == LOW; // Button pressed?
  if (b1 && lastButtonState1 == HIGH) {
    int c = ++phraseTally[currentProf][0]; // Increment tally
    sendTallyToGoogle(professors[currentProf], phrases[0], c); // Send to Google Sheets
    sendTallyToFirebase(professors[currentProf], phrases[0], c); // Send to Firebase
    Serial.printf("Tally %s:%s = %d\n", professors[currentProf], phrases[0], c);
    delay(200); // Debounce delay
  }
  lastButtonState1 = !b1;

  // --- External Button 2 ---
  bool b2 = digitalRead(BUTTON_PIN2) == LOW; // Button pressed?
  if (b2 && lastButtonState2 == HIGH) {
    int c = ++phraseTally[currentProf][1]; // Increment tally
    sendTallyToGoogle(professors[currentProf], phrases[1], c); // Send to Google Sheets
    sendTallyToFirebase(professors[currentProf], phrases[1], c); // Send to Firebase
    Serial.printf("Tally %s:%s = %d\n", professors[currentProf], phrases[1], c);
    delay(200); // Debounce delay
  }
  lastButtonState2 = !b2;

  // --- Built-in M5Core2 buttons ---
  if (M5.BtnA.wasPressed()) { currentProf = 0; updateColor(); } // Select ProfA
  if (M5.BtnB.wasPressed()) { currentProf = 1; updateColor(); } // Select ProfB
  if (M5.BtnC.wasPressed()) { currentProf = 2; updateColor(); } // Select ProfC

  // If color has been displayed for >5 seconds, send it to ThingSpeak
  if (!colorSent && millis() - lastChangeTime > 5000) {
    sendColorToThingSpeak();
    colorSent = true;
  }
}



// Updates screen color & professor label
void updateColor() {
  uint32_t color = colors[currentIndex];
  M5.Lcd.fillScreen(color);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK, color);  // Set contrasting text color
  M5.Lcd.printf("Color: #%06X", color);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.printf("Prof: %s", professors[currentProf]);
  lastChangeTime = millis();
  colorSent = false; // Reset send status
}

// Sends current color to ThingSpeak
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

// Sends tally data to Google Sheets
void sendTallyToGoogle(const char* prof, const char* phrase, int count) {
  HTTPClient http;
  String url = String(GOOGLE_SCRIPT_URL)
               + "?prof="   + prof
               + "&phrase=" + phrase
               + "&count="  + count;
  http.begin(url);
  int code = http.GET();
  if (code == 200) {
    Serial.println("→ Google Sheets OK");
  } else {
    Serial.printf("GS error %d\n", code);
  }
  http.end();
}

// Sends tally data to Firebase
void sendTallyToFirebase(const char* prof, const char* phrase, int count) {
  String path = String("/tallies/") + prof + "/" + phrase;
  if (Firebase.RTDB.setInt(&fbdo, path, count)) {
    Serial.println("→ Firebase OK");
  } else {
    Serial.print("FB error: ");
    Serial.println(fbdo.errorReason());
  }
}
