#include <M5Core2.h>

/*
Dylan Maxwell
6/2/2025
Lab 3.1: Working with Displays and Touch Buttons
dylmaxwe@uat.edu

*/



  void setup() {
    M5.begin();
  //Color screen red
  M5.Lcd.fillScreen(RED);

  //Delay 1 sec
  delay(1000);

  //Color screen white
  M5.Lcd.fillScreen(WHITE);

  //Delay 1 sec
  delay(1000);

  //Color screen blue
  M5.Lcd.fillScreen(BLUE);

  //Delay 1 sec
  delay(1000);

  //Color screen black
  M5.Lcd.fillScreen(BLACK);

  //Set cursor to 20,20
  M5.Lcd.setCursor(20, 20);

  //Set text size to 2
  M5.Lcd.setTextSize(2);

  //Write Name
  M5.Lcd.print("Dylan Maxwell");
}

void loop() {
  // nothing repeats so don't use
}
