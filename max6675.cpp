// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/

#include "max6675.h"

#define TRUE  1
#define FALSE 0

/* SPI Receive one Byte */
static uint8_t SPI_RxByte(SPI_HandleTypeDef *hspi)
{
  uint8_t dummy, data;
  dummy = 0xFF;
  data = 0;

  while ((HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY));
  HAL_SPI_TransmitReceive(hspi, &dummy, &data, 1, SPI_TIMEOUT);

  return data;
}

/**************************************************************************/
/*!
    @brief  Initialize a MAX6675 sensor
    @param   SCLK The Arduino pin connected to Clock
    @param   CS The Arduino pin connected to Chip Select
    @param   MISO The Arduino pin connected to Data Out
*/
/**************************************************************************/
MAX6675::MAX6675(SPI_HandleTypeDef *hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin) {
	_hspi = hspi;
	_CS_Port = CS_Port;
	_CS_Pin = CS_Pin;
	_filterLvl = FILTER_LVL;
	HAL_GPIO_WritePin(_CS_Port, _CS_Pin, GPIO_PIN_SET); // CS High
//	HAL_Delay(250); 									// 4Hz max
//	_cFiltered = readCelsius(FALSE); 					// Read once to init filtered value
}

//**************************************************************************
//    Init first reading
//**************************************************************************
void MAX6675::initMeasure(void) {
	HAL_Delay(250); 									// 4Hz max wait for the first reading
	_cFiltered = readCelsius(FALSE);
}
//**************************************************************************
//    @brief  Read the Celsius temperature
//    @returns Temperature
//**************************************************************************
float MAX6675::readCelsius(bool filter) {
	unsigned short data;

	//SELECT();
	HAL_GPIO_WritePin(_CS_Port, _CS_Pin, GPIO_PIN_RESET); // CS Low

	data = SPI_RxByte(_hspi);
	data <<= 8;
	data |= SPI_RxByte(_hspi);
	//DESELECT();
	HAL_GPIO_WritePin(_CS_Port, _CS_Pin, GPIO_PIN_SET); // CS High


	if (data & 4) _connected = FALSE;
	else _connected = TRUE;

	data  >>= 3;
	_celcius = data * 0.25;
	_cFiltered = (_filterLvl*_cFiltered)+((1-_filterLvl)*_celcius);

	if (filter) return (_cFiltered);
	return(_celcius);

}

/**************************************************************************/
/*!
 *  input bool true if request filtered value
    @returns flloat ast Celcius measure (member)
*/
/**************************************************************************/
float MAX6675::get_celcius(bool filter) const {
	if (filter) return (_cFiltered);
	return (_celcius);
}

/**************************************************************************/
/*!
 *  input -
    @returns TRUE if connected
*/
/**************************************************************************/
bool MAX6675::get_status(void) const {
	return (_connected);
}

/**************************************************************************/
/*!
 *  input bool true if request filtered value
    @returns flloat ast Celcius measure (member)
*/
/**************************************************************************/
void MAX6675::set_filterLevel(uint8_t level) {
	if (level>=100)level = 99;
	_filterLvl=level/100;
}


