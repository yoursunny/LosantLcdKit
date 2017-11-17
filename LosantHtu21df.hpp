#ifndef LOSANT_HTU21DF_HPP
#define LOSANT_HTU21DF_HPP

#include <memory>

class Adafruit_HTU21DF;
class LosantDevice;

/**
 * \brief report HTU21DF readings to Losant
 */
class LosantHtu21df
{
public:
  /**
   * \param tempVar Losant device state variable name for celsius temperature
   * \param humidVar Losant device state variable name for humidity
   * \param reportInterval interval between reports, in millis
   */
  LosantHtu21df(LosantDevice& losantDevice, const char* celsiusVar, const char* humidVar, int reportInterval = 15000);

  ~LosantHtu21df();

  void
  loop();

private:
  LosantDevice& m_losantDevice;
  std::unique_ptr<Adafruit_HTU21DF> m_sensor;
  const char* const m_celsiusVar;
  const char* const m_humidVar;
  const int m_reportInterval;
  unsigned long m_lastReport;
};

#endif // LOSANT_TEMPERATURE_HPP
