//======================================================
// Button program routine
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"

#define false 0;
#define true 1;


// Define button pin and timing constants
const unsigned long DEBOUNCE_DELAY = 50; // milliseconds
const unsigned long DOUBLE_CLICK_TIME = 300; // milliseconds
const unsigned long LONG_PRESS_TIME = 1000; // milliseconds

// Button state variables

uint8_t Button_timer = 0;
int lastButtonState = LOW;
unsigned long lastPressTime = 0;
unsigned long lastReleaseTime = 0;
unsigned long lastDebounceTime = 0;
uint8_t doubleClickPending = 0;
//int longPressPending = 1;
int longPressPending = 0;

uint8_t longPressEnd = 0;
//uint16_t u32Button_Pressed_ts = 0;
//uint16_t u32Button_Released_ts = 0;

extern uint16_t main_tick_up;
extern u8Powerdown_mode_flag;

uint16_t current_time_ms() {
	uint16_t time_msec;

	if(Button_timer > 0) {
		time_msec = main_tick_up;
//		time_msec++;
//		if(time_msec > 2000) time_msec=0;
	}
	else {
		main_tick_up = 0;
		time_msec = 0;
	}
	return time_msec;
}

void clearButtonTimer()
{
	doubleClickPending = false;
	Button_timer = false ;	
	lastReleaseTime = current_time_ms();
	lastPressTime = lastReleaseTime;

}	

uint8_t getButtonEvent()
{
    int currentButtonState = P_SWITCH;
    uint16_t currentTime = current_time_ms();
		uint8_t eButton_Event=NO_PRESS;

			
    if(currentButtonState != lastButtonState) {
			lastDebounceTime = currentTime;
			Button_timer = true ;
			}

    if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
//        if (currentButtonState == HIGH && longPressPending == 0) { // PRESS THE BUTTON
        if (currentButtonState == HIGH && longPressPending == 0) { // PRESS THE BUTTON
            lastPressTime = currentTime;
						longPressPending = true; // Assume a long press initially
        } 
				else if(currentButtonState == LOW && longPressPending  == 1){//
           lastReleaseTime = currentTime - lastPressTime;
						if(longPressEnd == 1) {
							longPressEnd = false; // Prevent repeated long press actions
							clearButtonTimer();
							longPressPending = false;
							return 	eButton_Event= NO_PRESS;
						}
						longPressPending = false;

            if (doubleClickPending && (currentTime - lastReleaseTime) < DOUBLE_CLICK_TIME) {
                eButton_Event = DOUBLE_PRESS;
                doubleClickPending = false;
								clearButtonTimer();
            } 
						else {
                doubleClickPending = true;
                lastReleaseTime = currentTime;
            }
        }
    }

    //
    if (doubleClickPending && (currentTime - lastReleaseTime) > DOUBLE_CLICK_TIME) {
        eButton_Event = SINGLE_PRESS;
				clearButtonTimer();
        doubleClickPending = false;
//				lastButtonState = currentButtonState;
//				return eButton_Event;
    }
		lastButtonState = currentButtonState;

	if (longPressPending && currentButtonState == HIGH && (currentTime - lastPressTime) >= LONG_PRESS_TIME) {
			// Long press detected
			// Perform long press action
//			Button_timer = false ;
			clearButtonTimer();		
			longPressEnd = true; // Prevent repeated long press actions
			lastButtonState = LOW;
			longPressPending = false;
      doubleClickPending = false;
		  eButton_Event = LONG_PRESS;
//	    return eButton_Event;
	}
	
    return eButton_Event;



		/*	
			if(currentButtonState == HIGH ) { // Button pressed
			// Check for long press
				longPressPending = true; // Assume a long press initially
				Button_timer = true ;
				lastPressTime = currentTime;
			}
			

			else { // Button released
				// Debouncing
        // Button state has changed reliably
				if(longPressEnd == 1) {
					longPressEnd = false; // Prevent repeated long press actions
					clearButtonTimer();
					return 	eButton_Event= NO_PRESS;
				}
				longPressPending = false; // Prevent repeated long press actions
			
				if(currentTime - lastReleaseTime < DEBOUNCE_DELAY ) {					// [CHOI_2507]CHECK 50mS for filter of chattering
        // Check for single or double click
					if(currentTime - lastPressTime < DOUBLE_CLICK_TIME) { // [CHOI_2507] Button click releaed within 100mS
						if (doubleClickPending) {
						// Double click detected
						// Perform double click action
              doubleClickPending = false;
							lastButtonState = currentButtonState;
							clearButtonTimer();
							return eButton_Event = DOUBLE_PRESS;
						} 
						else {
							doubleClickPending = true;	// [CHOI_2507] Set first click
						}
					} 
				else {
					// Single click detected (if not part of a double click)
          // Perform single click action
					doubleClickPending = false;
					lastButtonState = currentButtonState;
					clearButtonTimer();
					return eButton_Event = SINGLE_PRESS;
				}
			}
		}
			lastButtonState = currentButtonState;
			Button_timer = true ;
	}

	if (longPressPending && currentButtonState == HIGH && (currentTime - lastPressTime) >= LONG_PRESS_TIME) {
			// Long press detected
			// Perform long press action
			Button_timer = false ;
			lastPressTime = 0;			
			lastButtonState = LOW;
			longPressEnd = 1;
			return eButton_Event = LONG_PRESS;
	}

  lastButtonState = currentButtonState;
	return eButton_Event= NO_PRESS;
	*/
}

uint8_t Task_Button()
{
	uint8_t Button_Event;
	
	Button_Event = getButtonEvent();
	
	return Button_Event;
}
