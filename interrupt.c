//======================================================
// interrupt routines
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"

extern uint16_t msTicks;
extern uint8_t spi_writeBuff0;
extern uint8_t temp_spi_readBuff0;
extern uint8_t spi_readNum0;

uint16_t main_tick_up = 0;
uint8_t pwm_tick_up = 0;
uint16_t rotate_tick_up = 0;
uint8_t wt_cnt = 0;

void INT_BIT() interrupt 22
{
	uint8_t tmp = 0;
	
	msTicks++;

	if(WDTCNT == 63)
	{
		tmp = WDTCR;
		tmp |= 0x20;
		WDTCR = tmp;
	}
}

void INT_Timer0_OVF() interrupt 12
{
}

void INT_Timer0() interrupt 13
{
}

void INT_Timer1() interrupt 14
{
}

void INT_Timer2() interrupt 15
{
}

void INT_WT() interrupt 20
{
	main_tick_up++;
	if(wt_cnt++ == 9)
	{
		pwm_tick_up++;
		rotate_tick_up++;
		if(pwm_tick_up>97) pwm_tick_up=3;
		if(rotate_tick_up>999) rotate_tick_up = 0;
		wt_cnt = 0;
	}
}

void INT_WDT() interrupt 21
{
//	uint8_t tmp = 0;
//	
//	tmp = WDTCR;
//	tmp |= 0x20;
//	WDTCR = tmp;
}

void INT_UART_Rx() interrupt 9
{
	// UART Rx interrupt
	// TODO: add your code here
}

void INT_UART_Tx() interrupt 10
{
//	UARTCR2 |= 0x80;
//	IE1 |= 0x10;
}

void INT_SPI() interrupt 19
{
	//IE3 &= ~0x02;			// Disable SPI Interrupt
}

void INT_EINT6() interrupt 6
{
//	on_off = 0;
	
	EIFLAG0 &= 0xDF;
}
