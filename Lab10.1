
#include <M5Core2.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// This is the wifi information so that I can get the the M5 connected to the WIFI.
#include "secrets.h"
//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;

int intValue;
float floatValue;

bool signupOK = false;


void setup() {
  // this is going to begin the serial at 115200
  Serial.begin(115200);
  // this is going to begin the M5
  M5.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // this is going to delay
    delay(300);
  };
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  /* Assign the api key (required) */
  config.api_key = FIREBASE_AUTH;
  /* Assign the RIDB URL (required) */
  config.database_url = FIREBASE_HOST;


  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons / TokenHelper.h
  // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    bool gotInt = false;
    bool gotFloat = false;

    // Try get int from Firebase
    if (Firebase.RTDB.getInt(&fbdo, "/test/int")) {
      if (fbdo.dataType() == "int") {
        intValue = fbdo.intData();
        gotInt = true;
      } else {
        Serial.println("Data at /test/int is NOT an int!");
      }
    } else {
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // Try get float from Firebase
    if (Firebase.RTDB.getFloat(&fbdo, "/test/float")) {
      Serial.print("Data type received: ");
      Serial.println(fbdo.dataType());

      if (fbdo.dataType() == "float") {
        floatValue = fbdo.floatData();
        gotFloat = true;
      } else {
        Serial.println("Data at /test/float is NOT a float!");
      }
    } else {
      Serial.print("Failed to get /test/float - Reason: ");
      Serial.println(fbdo.errorReason());
    }

    // Clear screen once before printing anything
    M5.Lcd.fillScreen(OLIVE);

    // Print int value if available
    if (gotInt) {
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(55, 50);
      M5.Lcd.print("Int: ");
      M5.Lcd.print(intValue);
    } else {
      M5.Lcd.setCursor(55, 50);
      M5.Lcd.setTextColor(TFT_RED);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("Int: ERR");
    }

    // Print float value if available
    if (gotFloat) {
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(30, 100);
      M5.Lcd.print("Float: ");
      M5.Lcd.print(floatValue);
    } else {
      M5.Lcd.setCursor(30, 100);
      M5.Lcd.setTextColor(TFT_RED);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("Float: ERR");
    }

    // Optional: print to Serial for debug
    if (gotInt) Serial.println("Int value: " + String(intValue));
    if (gotFloat) Serial.println("Float value: " + String(floatValue));
  }
}
