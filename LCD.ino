#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Losant.h>
#include <Streaming.h>
#include "credentials.hpp"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

WiFiClientSecure losantSocket;
LosantDevice losantDevice(LOSANT_DEVICE_ID);

void
connect()
{
  if (WiFi.status() == WL_CONNECTED && losantDevice.connected()) {
    return;
  }

  Serial << "Connecting to WiFi ";
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial << WiFi.status();
  }
  Serial << endl << "Connected to WiFi " << WiFi.localIP() << endl;

  Serial << "Connecting to Losant ";
  losantDevice.connectSecure(losantSocket, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);
  while (!losantDevice.connected()) {
    delay(1000);
    Serial << '.';
  }
  Serial << endl << "Connected to Losant" << endl;
}

void
handleRefreshCommand(JsonObject& payload)
{
  lcd.clear();
  lcd << payload["L1"].asString();
  lcd.setCursor(0, 1);
  lcd << payload["L2"].asString();
}

void
handleCommand(LosantCommand* cmd)
{
  Serial << "Received command " << cmd->name << ' ';
  JsonObject& payload = *cmd->payload;
  payload.printTo(Serial);
  Serial << endl;

  if (strcmp(cmd->name, "refresh") == 0) {
    handleRefreshCommand(payload);
  }
  else {
    Serial << "Unknown command" << endl;
  }
}

void
setup()
{
  Wire.pins(12, 14); // D5-SCL, D6-SDA

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.disconnect();
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

  losantDevice.onCommand(&handleCommand);

  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
}

void
loop()
{
  connect();
  losantDevice.loop();
  delay(100);
}
