#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <IOXhop_FirebaseESP32.h>
#include <IOXhop_FirebaseStream.h>
#include "secrets.h"

String fireStatus;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(10, 10);

WiFi.begin(WIFI_SSID, WIFI_PASS);
Serial.print("connecting ");
while(WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  delay(500);
}

Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.setString("color_status", "OFF");

}

void loop() {
  //get latest "color_status" value
  fireStatus = Firebase.getString("color_status");
  Serial.print("color_stauts: ");
  //Print the actual value
  Serial.println(fireStatus);

  //if says off
  if (fireStatus == "OFF") {
    //Paint screen black
    Serial.println("Background -> BLACK");
    M5.Lcd.fillScreen(BLACK);
  }
  //if says red, paint red
  else if (fireStatus == "RED") {
    Serial.println("Background -> RED");
    M5.Lcd.fillScreen(RED);
  }
  //if says green, paint green
  else if (fireStatus == "GREEN") {
  Serial.println("Background -> GREEN");
  M5.Lcd.fillScreen(GREEN);
  }
  //if says blue, paint blue
  else if (fireStatus == "BLUE") {
    Serial.println("Background -> BLUE");
    M5.Lcd.fillScreen(BLUE);
  }
  //if anything else
  else {
    Serial.println("Wrong Color");
  }

delay(1000);
}
