// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/

#ifndef ADAFRUIT_MAX6675_H
#define ADAFRUIT_MAX6675_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"


#define SPI_TIMEOUT 1000
#define FILTER_LVL 0.10

/**************************************************************************/
/*!
    @brief  Class for communicating with thermocouple sensor
*/
/**************************************************************************/
class MAX6675 {
public:
  MAX6675(SPI_HandleTypeDef *hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin);
  void initMeasure(void);					// Need to call it after all sytem init (/* Initialize all configured peripherals */) !!!
  float readCelsius(bool filtered);
  float get_celcius(bool filtered) const;
  void set_filterLevel(uint8_t level);  	//0-99
  bool get_status(void) const;				// TRUE = connected

protected:
  float	_celcius;
  float	_cFiltered;
  bool 	_connected;
  float _filterLvl;
  SPI_HandleTypeDef *_hspi;
  GPIO_TypeDef* _CS_Port;
  uint16_t _CS_Pin;
};

#endif
