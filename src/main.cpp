#include <Arduino.h>
#include "SoftwareSerial.h"
#include "ESP_AT_Lib.h"
#include "SPI.h"
#include "Wire.h"
#include "LiquidCrystal.h"
#include "RTClib.h"
#include "DHT.h"

#define LCDRS 12
#define LCDENABLE 11
#define LCDD0 5
#define LCDD1 4
#define LCDD2 3
#define LCDD3 2

#define DHTPIN A0
#define DHTTYPE DHT11

#define SSID ""
#define PASSWORD ""

#define ESP_AT_BAUD 115200
#define SERIAL_BAUD 9600

SoftwareSerial ESP_Serial(9, 10); // TX, RX
ESP8266 wifi(&ESP_Serial);

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(LCDRS, LCDENABLE, LCDD0, LCDD1, LCDD2, LCDD3);
RTC_DS1307 rtc;

void setup() {
    Serial.begin(SERIAL_BAUD);
    ESP_Serial.begin(ESP_AT_BAUD);

    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("inicializando...");

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    dht.begin();

    if (wifi.setOprToStation()) {
        Serial.println("Set STA Mode OK");
    } else {
        Serial.println("Set STA Mode failed");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.println("Connect to WiFi OK");
        Serial.print("IP: ");
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.println("Connect to WiFi failed");
    }

    lcd.clear();
}

float temperature() {
    return dht.readTemperature();
}

float humidity() {
    return dht.readHumidity();
}

void loop() {
    DateTime now = rtc.now();

    lcd.setCursor(0, 0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.print(" ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);

    float t = temperature();
    float h = humidity();

    if (!isnan(t) && !isnan(h)) {
        lcd.setCursor(0, 1);
        lcd.print("T:");
        lcd.print(t);
        lcd.print(" ");

        lcd.print("U:");
        lcd.print(h);
    }

    delay(1000);
}