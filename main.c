//======================================================
// Main program routine
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"
#include	"delay.h"

void system_Delay_ms(uint16_t ms)
{
	int i;
	
	for(i = 0; i < ms; i++)
	{
		NOP_10us_Delay(58);
	}
}

void main()
{
	cli();          	// disable INT. during peripheral setting
	port_init();    	// initialize ports
	port_EINT_init();	// initialize EINT5
	clock_init();   	// initialize operation clock
	BIT_init();     	// initialize Basic interval timer
	Timer0_init();  	// initialize Timer0
	Timer1_init();  	// initialize Timer1
	Timer2_init();  	// initialize Timer2
	WT_init();      	// initialize Watch timer
	WDT_Init();
	SPI_init();     	// initialize SPI interface
	UART_init();    	// initialize UART interface
	sei();          	// enable INT.

	Run_Task();
}
