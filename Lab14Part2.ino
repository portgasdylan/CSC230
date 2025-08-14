#include <M5Core2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Scytale";
const char* password = "Chaumurky26";

String openWeatherMapApiKey = "6f8905bf7cd52190742046fd529be8f3";

// Current city & country
String city = "Tempe";
String countryCode = "US";

String jsonBuffer;

String httpGETRequest(const char* serverName) {
  HTTPClient http;
  http.begin(serverName);
  int httpResponseCode = http.GET();

  String payload = "{}";
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}

void fetchAndDisplayWeather() {
  String serverPath =
    "https://api.openweathermap.org/data/2.5/weather?q=" +
    city + "," + countryCode +
    "&units=imperial&appid=" + openWeatherMapApiKey;

  jsonBuffer = httpGETRequest(serverPath.c_str());
  JSONVar obj = JSON.parse(jsonBuffer);
  if (JSON.typeof(obj) == "undefined") {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(20, 40);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.print("Error reading data");
    return;
  }

  String apiCity = (const char*)obj["name"];
  String apiCountry = (const char*)obj["sys"]["country"];
  double tempF = (double)obj["main"]["temp"];
  int humidity = (int)obj["main"]["humidity"];

  // Display on screen
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(20, 30);
  M5.Lcd.printf("%s, %s", apiCity.c_str(), apiCountry.c_str());

  M5.Lcd.setCursor(20, 90);
  M5.Lcd.printf("Temp: %.1f F", tempF);

  M5.Lcd.setCursor(20, 150);
  M5.Lcd.printf("Humidity: %d %%", humidity);
}

void setup() {
  M5.begin();
  Serial.begin(115200);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(20, 40);
  M5.Lcd.print("Connecting Wi-Fi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 40);
  M5.Lcd.print("Connected!");
}

void loop() {
  M5.update();

  // Button A = Tempe
  if (M5.BtnA.wasPressed()) {
    city = "Tempe";
    countryCode = "US";
    fetchAndDisplayWeather();
  }

  // Button B = Brooklyn, New York
  if (M5.BtnB.wasPressed()) {
    city = "Brooklyn";
    countryCode = "US";
    fetchAndDisplayWeather();
  }
}
