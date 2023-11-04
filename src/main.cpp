#include <Arduino.h>
#include "SoftwareSerial.h"
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

#define ESP_AT_BAUD 115200
#define SERIAL_BAUD 9600

SoftwareSerial ESP_Serial(9, 10); // TX, RX

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(LCDRS, LCDENABLE, LCDD0, LCDD1, LCDD2, LCDD3);
RTC_DS1307 rtc;

bool startWiFi() {
    ESP_Serial.flush();
    ESP_Serial.println("AT");
    delay(2000);

    if (ESP_Serial.find("OK")) {
        Serial.println("Comunicacao com modulo ESP8266: OK");
    } else {
        Serial.println("Erro no modulo ESP8266");
    }

    Serial.println("Conectando wi-fi...");
    Serial.println("AT+CWMODE=1");
    ESP_Serial.println("AT+CWMODE=1");
    delay(2000);
    ESP_Serial.flush();

    Serial.println("Desabilitando multilpas conexões");
    Serial.println("AT+CIPMUX=0");
    ESP_Serial.println("AT+CIPMUX=0");
    delay(2000);
    ESP_Serial.flush();

    String wifiConnectString = "AT+CWJAP=\"ssid\",\"password\"";
    Serial.println(wifiConnectString);
    ESP_Serial.println(wifiConnectString);
    delay(5000);

    if (ESP_Serial.find("OK")) {
        Serial.println("Conectado com sucesso!");
        return true;
    } else {
        Serial.println("Falha na conexao!");
        return false;
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    ESP_Serial.begin(ESP_AT_BAUD);

    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Inicializando...");

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    dht.begin();

    startWiFi();

    lcd.clear();
}

float temperature() {
    return dht.readTemperature();
}

float humidity() {
    return dht.readHumidity();
}

void updateSensors(uint32_t unixtime, float temperature, float humidity) {
    String host = "";

    String s_unixtime = String(unixtime);
    String s_temperature = String(temperature);
    String s_humidity = String(humidity);

    String get = "GET /update?time=%unixtime%&temperature=%temperature%&humidity=%humidity% HTTP/1.1\r\nHost: weather.hevertonfreitas.com.br\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0\r\n\r\n";
    get.replace("%unixtime%", s_unixtime);
    get.replace("%temperature%", s_temperature);
    get.replace("%humidity%", s_humidity);

    String startConnection = "AT+CIPSTART=\"TCP\",\"" + host + "\",80";
    Serial.println(startConnection);
    ESP_Serial.println(startConnection);
    delay(1000);

    String dataCommand = "AT+CIPSEND=" + String(get.length());
    Serial.println(dataCommand);
    ESP_Serial.println(dataCommand);
    delay(1000);

    Serial.println(get);
    ESP_Serial.print(get);
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

    if (now.minute() % 5 == 0 && now.second() == 0) {
        updateSensors(now.unixtime(), t, h);
    }
}