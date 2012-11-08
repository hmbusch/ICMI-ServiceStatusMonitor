// We need to include the display driver in the sketch, otherwise 
// our other header file cannot use it. Library usage cannot be
// "hidden" from the sketch.
#include "ICMIMax7219.h"
#include "ICMIStatusMonitor.h"

/**
 * The number of the Arduino pin that is connected to the data in pin.
 * The data in pin is pin 1 on the MAX7219.
 */
const uint8_t PIN_DATA = 5;

/**
 * The number of the Arduino pin that is connected to the clock pin.
 * The clock pin is pin 13 on the MAX7219.
 */
const uint8_t PIN_CLOCK = 6;

/**
 * The number of the Arduino pin that is connected to the load pin
 * The data in pin is pin 12 on the MAX7219.
 */
const uint8_t PIN_LOAD = 7;

/**
 * The number of indicators (indicator = set of 3 LEDs) connected to
 * the circuit.
 */
const uint8_t INDICATOR_COUNT = 3;

void setup()
{
#ifdef icmi_use_serial 
  Serial.begin(9600);
#endif
  ICMIStatusMonitor.begin(PIN_DATA, PIN_CLOCK, PIN_LOAD, INDICATOR_COUNT);
  delay(2000);
  ICMIStatusMonitor.setIndicator(1, STATE_RED);
  delay(1000);
  ICMIStatusMonitor.setIndicator(1, STATE_YELLOW);
  ICMIStatusMonitor.setIndicator(0, STATE_RED);
  delay(1000);
  ICMIStatusMonitor.setIndicator(2, STATE_RED);
  ICMIStatusMonitor.setIndicator(0, STATE_YELLOW);
  delay(2000);
  ICMIStatusMonitor.setIndicator(0, STATE_GREEN);  
}

void loop()
{

}


