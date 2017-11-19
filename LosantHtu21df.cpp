#include "LosantHtu21df.hpp"
#include <Adafruit_HTU21DF.h>
#include <Losant.h>
#include <Streaming.h>

LosantHtu21df::LosantHtu21df(LosantDevice& losantDevice, const char* celsiusVar, const char* humidVar, int reportInterval)
  : m_losantDevice(losantDevice)
  , m_sensor(new Adafruit_HTU21DF())
  , m_celsiusVar(celsiusVar)
  , m_humidVar(humidVar)
  , m_reportInterval(reportInterval)
  , m_lastReport(millis())
{
}

LosantHtu21df::~LosantHtu21df() = default;

void
LosantHtu21df::loop()
{
  if (!m_losantDevice.connected() || millis() - m_lastReport < m_reportInterval) {
    return;
  }

  float celsius = m_sensor->readTemperature();
  float humid = m_sensor->readHumidity();
  yield();

  Serial << "HTU21DF " << celsius << "C " << humid << "%" << endl;

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  if (m_celsiusVar != nullptr) {
    root[m_celsiusVar] = celsius;
  }
  if (m_humidVar != nullptr) {
    root[m_humidVar] = humid;
  }
  m_losantDevice.sendState(root);

  m_lastReport = millis();
}
