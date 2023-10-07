#include <Arduino.h>
#include "Wire.h"
#include "SPI.h"
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

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(LCDRS, LCDENABLE, LCDD0, LCDD1, LCDD2, LCDD3);
RTC_DS1307 rtc;

void setup() {
    Serial.begin(9600);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    lcd.begin(16, 2);
    lcd.print("inicializando...");

    dht.begin();

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