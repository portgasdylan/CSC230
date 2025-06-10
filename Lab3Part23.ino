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
  //When Button A is released
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        //Color Screen White
        M5.Lcd.fillScreen(WHITE);
        //Change Text Size
        M5.Lcd.setTextSize(3);
        //Use setCursor
        M5.Lcd.setCursor(50,100);
        //Set Text Color to Red
        M5.Lcd.setTextColor(RED);
        //Print myName on Screen
        M5.Lcd.print(myName);
        //Random Color Combo
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        randNum = random(1,5);
        //If randNum is 1, show Blue screen with Red & White Text
        if(randNum == 1){
          M5.Lcd.fillScreen(BLUE);
          M5.Lcd.setTextColor(RED, WHITE);
        //If randNum is 2, show White screen with Red & Blue Text
        } else if (randNum == 2){
          M5.Lcd.fillScreen(WHITE);
          M5.Lcd.setTextColor(RED, BLUE);
        //If randNum is 3, show Red screen with Blue & Green Text
        } else if (randNum == 3){
          M5.Lcd.fillScreen(RED);
          M5.Lcd.setTextColor(BLUE, GREEN);
        //If randNum is 4, show Green screen with Blue & White Text
        } else if (randNum == 4) {
          M5.Lcd.fillScreen(GREEN);
          M5.Lcd.setTextColor(BLUE, WHITE);
        }
        //Change Text Size
        M5.Lcd.setTextSize(3);
        //Use setCursor
        M5.Lcd.setCursor(55,100);
        //Print Random on Screen
        M5.Lcd.print("Random!!");
        //Specific Color Combo
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
        //Change Text Size
        M5.Lcd.setTextSize(3);
        //Use setCursor
        M5.Lcd.setCursor(50,100);
        //Change Text Color
        M5.Lcd.setTextColor(BLACK, RED);
        //Color Screen Olive
        M5.Lcd.fillScreen(OLIVE);
        //Print myName
        M5.Lcd.print(myName);
    }
}
