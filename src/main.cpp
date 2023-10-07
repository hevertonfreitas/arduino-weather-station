#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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
        lcd.print("Temperatura: ");
        lcd.print(t);

        lcd.setCursor(0, 1);
        lcd.print("Umidade: ");
        lcd.print(h);
    }
}