#include "ICMIStatusMonitor.h"

/**
 * Creates a new status monitor instance. You need to pass the pin configuration
 * to the constructor (data in, clock, load) and specify how many indicators (sets
 * of triple LEDs) are connected to the circuit. The minimum is 1, the maximum
 * is 16. You may experience eratic behaviour of the sketch if this number is not
 * correct.
 * The instance then creates a controller instance for the MAX7219 IC using the 
 * values provided. 
 *
 * @param dataPin the number of the Arduino pin that is connected to the data in pin
 *                of the MAX7219 (pin 1, labeled DIN in the datasheet)
 * @param clockPin the number of the Arduino pin that is connected to the clock pin
 *                 of the MAX7219 (pin 13, labeled CLK in the datasheet)
 * @param loadPin the number of the Arduino pin that is connected to the load pin
 *                of the MAX7219 (pin 12, labeled LOAD in the datasheet)
 * @param indicatorCount the number of indicators connected to the chip. Make sure this number
 *                   is correct. A number lower than 1 is treated as on, a number higher
 *                   than 16 will be treated as 16.
 */
void ICMIStatusMonitorClass::begin(uint8_t dataPin, uint8_t clockPin, uint8_t loadPin, uint8_t indicatorCount)
{
    // Make sure we don't init with less than 1 or more than 16
    // indicators
    if (indicatorCount < 1)
    {
      this->indicatorCount = 1;
    }
    else if (indicatorCount > 16)
    {
      this->indicatorCount = 16;
    }
    else
    {
      this->indicatorCount = indicatorCount;
    }

    // configure the display driver and enable it
    this->max7219.begin(dataPin, clockPin, loadPin, 5);
    this->max7219.enable(true);
    
    // pre-init all (even the unused) indicators as off
    for(uint8_t i = 0; i < 8; i++)
    {
      this->indicatorStates[i] = 0;
    }
    
    // now pre-init the used indicators with green
    for(uint8_t j = 0; j < indicatorCount; j++)
    {
      this->setIndicator(j, STATE_GREEN);
    }
}


/**
 * Changes the designated indicator to the desired color. This is done
 * by first determining the row and the position of the indicator,
 * then blanking the indicator and finally switching on the desired
 * color.
 *
 * @param which the indicator number, starting with 0 as the first
 *              indicator. The maximum indicator number is row*2 - 1.
 * @param color one of the color constants (GREEN, YELLOW, RED)
 */
void ICMIStatusMonitorClass::setIndicator(uint8_t which, uint8_t toWhat)
{
    // calculate the row that contains the desired indicator. A simple division
    // is used, it delivers 0 for indicator 0 & 1, 1 for indicators 2 & 3, and so on
    uint8_t indicatorRow = which / 2;  
    
    // determine if the indicator is the first or second in the row. This is done using
    // modulo and treating the 0 as hint to the first indicator, whereas 1 hints
    // at the second indicator
    uint8_t firstOrSecond = which % 2;
    
#ifdef icmi_use_serial
    Serial.print("\n===================\n");
    Serial.print("Number : ");
    Serial.print(which, DEC);
    Serial.print("\nTo What: ");
    Serial.print(toWhat, BIN);
    Serial.print("\nRow    : ");
    Serial.print(indicatorRow, DEC);
    Serial.print("\n1st/2nd: ");
    Serial.print(firstOrSecond, DEC);
#endif

    // default values for offset and pattern are for the first indicator in
    // the row. We only need to change them if the seconds indicator is to be
    // adressed.
    uint8_t offset = 0;
    uint8_t nullPattern = B11110000;

    // a result of one indicates the second indicator in a row which means we need
    // to use the bit offset and inverse the null pattern to aplly to the other four bits
    if (1 == firstOrSecond)
    {
        offset = INDICATOR_OFFSET;
        nullPattern = ~nullPattern;
    }
    
#ifdef icmi_use_serial    
    Serial.print("\nState before: ");
    Serial.print(indicatorStates[indicatorRow], BIN);
#endif

    // this completely resets the designated indicator. This way, we
    // don't have to find out which color was lit before.
    this->indicatorStates[indicatorRow] &= nullPattern;

    // finally: set the desired color to on
    bitWrite(this->indicatorStates[indicatorRow], offset + toWhat, 1);

#ifdef icmi_use_serial 
    Serial.print("\nState after: ");
    Serial.print(indicatorStates[indicatorRow], BIN);
#endif

    // copy the changed row to the display for update
    this->max7219.setRow(indicatorRow, indicatorStates[indicatorRow]);
}

// Pre-instantiate the class for easier access.
ICMIStatusMonitorClass ICMIStatusMonitor;


