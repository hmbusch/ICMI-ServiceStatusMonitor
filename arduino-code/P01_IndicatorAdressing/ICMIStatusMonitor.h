/*
 * ====================================================================
 * ICMI Service Status Monitor Display control library
 * ====================================================================
 * Created by Hendrik Busch in 2012
 *
 * Please visit https://github.com/hmbusch/ICMI-ServiceStatusMonitor
 * for the latest version.
 *
 * For more information, please visit my project site at:
 *
 * http://www.icanmakeit.de/category/make/projects/icmi-service-status-monitor/
 */

// Pre-processor directive to ensure that this header file is only
// included once, no matter how often it is used in an include statement.
#ifndef ICMI_StatusMonitor_h
#define ICMI_StatusMonitor_h

// In order to use the well known Arduino commands, we need to include
// the main Arduino header file. This will only work with Arduino 1.0
// or greater, because the header file was named differently before the
// 1.0 release.
#include <ICMIMax7219.h>

// color designators doubling as bit indices
const uint8_t STATE_GREEN = 0;
const uint8_t STATE_YELLOW = 1;
const uint8_t STATE_RED = 2;

// the offset for the second indicator
const uint8_t INDICATOR_OFFSET = 4;

/**
 * This class represents the logical model of the status monitor as a whole. For 
 * cleaner code and stricter separation of concerns, all the interaction with the
 * monitor is offered as an API and all the communication with the MAX7219 is
 * encapsulated within so that the main sketch only needs to control the monitor
 * logic and this class controls all the necessary low-level functions.
 */
class ICMIStatusMonitorClass
{
public:

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
    void begin(uint8_t dataPin, uint8_t clockPin, uint8_t loadPin, uint8_t indicatorCount);
   
    void setIndicator(uint8_t which, uint8_t toWhat);
    
private:

    /**
     * The controller instance used to talk to the MAX7219 IC.
     */
    ICMIMax7219 max7219;

    /**
     * The number of indicators connected to the monitor.
     */
    uint8_t indicatorCount;
    
    /**
     * The logical state of the indicators. Since the MAX7219 works by dividing the
     * display into rows of 8 LEDs, each 8-bit value in this array represents two
     * indicators.
     */
    uint8_t indicatorStates[8];
    
};

// Indicate that an instance named ICMIStatusMonitor will be declared somewhere
// (actually it will be declared in the corresponding .cpp file)
extern ICMIStatusMonitorClass ICMIStatusMonitor;

#endif
