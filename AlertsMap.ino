
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Adafruit_NeoPixel.h>
#define PIN 4         // led pin
#define NUMPIXELS 26  // leds number 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define BTN_PIN 5
#include "GyverButton.h"
GButton butt1(BTN_PIN);
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Replace with your Wi-FI network credentials
const char* ssid = "name";
const char* password = "pass";

int lamp;
String apidata;
int brightness = 1;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.clear();
  Serial.println();
  Serial.println();
  Serial.println();

  //Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}

void loop() {
  butt1.tick();

  // wait for WiFi connection
  if ((millis() - lastTime) > timerDelay) {
    if ((WiFi.status() == WL_CONNECTED)) {

      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

      // Ignore SSL certificate validation
      client->setInsecure();

      //create an HTTPClient instance
      HTTPClient https;

      //Initializing an HTTPS communication using the secure client
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://api.alerts.in.ua/v1/iot/active_air_raid_alerts_by_oblast.json?token=#token#")) {  // Replace #token# with your token provided by site support 
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            apidata = https.getString();
            Serial.println(apidata);
            Serial.println(apidata.length());
            setLed(apidata);
          }

        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }

    Serial.println();
    Serial.println("Waiting 30 sec before the next round...");
    lastTime = millis();
  }
  if (butt1.isStep()) {
    brightness = brightness + 20;
    if (brightness > 255) {
      brightness = 1;
    }
    setLed(apidata);
  }
  // Serial.println(brightness);
}

void setLed(String apidata) {
  //pixels.clear();
  for (int i = 0; i < apidata.length(); i++) {
    //Serial.print(i);
    //Serial.println(apidata[i]);
    lamp = getRegion(i);
    //Serial.println(lamp);
    switch (apidata[i]) {
      case 'A':
        pixels.setPixelColor(lamp, pixels.Color(brightness, 0, 0));
        //    Serial.print("Alert");
        break;
      case 'P':
        pixels.setPixelColor(lamp, pixels.Color(brightness, brightness, 0));
        //    Serial.print("partially Alert");
        break;
      case 'N':
        pixels.setPixelColor(lamp, pixels.Color(0, brightness, 0));
        //    Serial.print("No Alert");
        break;
      default:
        Serial.print("No Data");
    }
    pixels.show();
  }
  
}

int getRegion(int symbol) {
  switch (symbol) {
    case 1:
      //      Serial.println("Автономна Республіка Крим");
      lamp = 0;
      break;
    case 2:
      //      Serial.println("Волинська область");
      lamp = 19;
      break;
    case 3:
      //      Serial.println("Вінницька область");
      lamp = 16;
      break;
    case 4:
      //      Serial.println("Дніпропетровська область");
      lamp = 5;
      break;
    case 5:
      //      Serial.println("Донецька область");
      lamp = 2;
      break;
    case 6:
      //      Serial.println("Житомирська область");
      lamp = 17;
      break;
    case 7:
      //      Serial.println("Закарпатська область");
      lamp = 21;
      break;
    case 8:
      //      Serial.println("Запорізька область");
      lamp = 1;
      break;
    case 9:
      //      Serial.println("Івано-Франківська область");
      lamp = 22;
      break;
    case 10:
      //      Serial.println("м. Київ");
      lamp = 13;
      break;
    case 11:
      //      Serial.println("Київська область");
      lamp = 14;
      break;
    case 12:
      //      Serial.println("Кіровоградська область");
      lamp = 9;
      break;
    case 13:
      //      Serial.println("Луганська область");
      lamp = 3;
      break;
    case 14:
      //      Serial.println("Львівська область");
      lamp = 20;
      break;
    case 15:
      //      Serial.println("Миколаївська область");
      lamp = 8;
      break;
    case 16:
      //      Serial.println("Одеська область");
      lamp = 7;
      break;
    case 17:
      //      Serial.println("Полтавська область");
      lamp = 10;
      break;
    case 18:
      //      Serial.println("Рівненська область");
      lamp = 18;
      break;
    case 19:
      //      Serial.println("м. Севастополь");
      lamp = 30; //not in use, in my case 
      break;
    case 20:
      //      Serial.println("Сумська область");
      lamp = 11;
      break;
    case 21:
      //      Serial.println("Тернопільська область");
      lamp = 24;
      break;
    case 22:
      //      Serial.println("Харківська область");
      lamp = 4;
      break;
    case 23:
      //      Serial.println("Херсонська область");
      lamp = 6;
      break;
    case 24:
      //      Serial.println("Хмельницька область");
      lamp = 25;
      break;
    case 25:
      //      Serial.println("Черкаська область");
      lamp = 15;
      break;
    case 26:
      //      Serial.println("Чернівецька область");
      lamp = 23;
      break;
    case 27:
      //Serial.println("Чернігівська область");
      lamp = 12;
      break;

  }
  return lamp;
}
