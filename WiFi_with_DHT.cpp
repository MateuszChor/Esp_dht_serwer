// it's nesessary to include this library if we don't use Arduino IDE to upload the program on microcontroller
//#include <Arduino.h>

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 4

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// type of sensor DHT11 OR DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// "nettwork name"
const char* ssid  ="wifi name";
const char* password ="wifi password";

const char* host  ="server ip";

void setup()
{

  Serial.begin(115200);
  Serial.println("DHT22 Output!");
  dht.begin();

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // Start connecting to wifi nettwork

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop()
{

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if(isnan(temperature) || isnan(humidity)){
    Serial.println("Failed to read DHT22"); 
  }else{
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("temperature: ");
    Serial.print(temperature);
    Serial.print(" *C  ");

    lcd.setCursor(0, 0);
    lcd.print("Wilgoc : ");
    lcd.setCursor(9, 0);
    lcd.print(humidity);
    lcd.setCursor(0,1);
    lcd.print("Temp   :");
    lcd.setCursor(9, 1);
    lcd.print(temperature);
    
    delay(3000);
  }

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

  

    // This will send the request to the server 
                      // apache server address with 8080 port to php script example GET http//127.0.0.1:8080/connect.php
                      // put ip of the apache serwer in serwerip
  client.print(String("GET http://serwerip:80/IOT_project/connect.php?") +
                           ("&temperature=") + temperature +
                           ("&humidity=") + humidity +
                           " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
     
  }

  Serial.println();
  Serial.println("closing connection");
}
