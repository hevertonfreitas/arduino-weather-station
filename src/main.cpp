#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
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

void setup() {
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
    float t = temperature();
    float h = humidity();

    if (!isnan(t) && !isnan(h)) {
        lcd.setCursor(0, 0);
        lcd.print("C: ");
        lcd.print(t);
        lcd.print(" graus");

        lcd.setCursor(0, 1);
        lcd.print("UR: ");
        lcd.print(h);
        lcd.print("%");
    }
}