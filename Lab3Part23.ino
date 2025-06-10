#include <M5Core2.h>

//Dylan Maxwell
//6.4.25
//dylmaxwe@uat.edu

String myName = "Dylan Maxwell";
long randNum = 0;

void setup() {
  M5.begin();
  //Color Screen White
  M5.Lcd.fillScreen(WHITE);
  //Set text color to red
  M5.Lcd.setTextColor(RED);
  //Change text size
  M5.Lcd.setTextSize(3);
  //Use setCursor
  M5.Lcd.setCursor(50,100);
  //Put myName on screen
  M5.Lcd.print(myName);


}

void loop() {
  M5.update();
  //Reset to Original Colors
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        M5.Lcd.fillScreen(WHITE);
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(50,100);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.print(myName);
        //Random Color Combo
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        randNum = random(1,5);
        if(randNum == 1){
          M5.Lcd.fillScreen(BLUE);
          M5.Lcd.setTextColor(RED, WHITE);
        } else if (randNum == 2){
          M5.Lcd.fillScreen(WHITE);
          M5.Lcd.setTextColor(RED, BLUE);
        } else if (randNum == 3){
          M5.Lcd.fillScreen(RED);
          M5.Lcd.setTextColor(BLUE, GREEN);
        } else if (randNum == 4) {
          M5.Lcd.fillScreen(GREEN);
          M5.Lcd.setTextColor(BLUE, WHITE);
        }
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(55,100);
        M5.Lcd.print("Random!!");
        //Specific Color Combo
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(50,100);
        M5.Lcd.setTextColor(BLACK, RED);
        M5.Lcd.fillScreen(OLIVE);
        M5.Lcd.print(myName);
    }
}
