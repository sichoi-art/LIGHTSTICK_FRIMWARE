//======================================================
// peripheral setting routines
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"

/*******************************************************************************
* Private Variable
*******************************************************************************/
uint8_t spi_writeBuff0 = 0;
uint8_t temp_spi_readBuff0 = 0;
uint8_t spi_readBuff0 = 0;

void clock_init()
{
	// external clock
	OSCCR = _0000_1010;   			// Enable Int. 1MHz and Ext. OSC
	BITCR = _0000_1001;   			// Set waiting time : 16ms@1MHz
	while((BITCR & 0x80) == 0);	// Ext. OSC stabilizing time
	SCCR = _0000_0001;    			// Change to Ext. OSC
	OSCCR |= _0000_0100;  			// Disable Int. OSC
}

void BIT_init()
{
	/*--- Set Basic Timer ---------------------------------------------------------------
   * BITCR 	: 0000 0000 = 0x00			: Basic Timer Control Register 
   * BITCR 	= 0x01;		// bitifr,BITCK1,BITCK0,-,bclr,bck2,bck1,bck0
	 * external fx=12MHz, 3MHz/4096=500us, overflow period=1ms
   *---------------------------------------------------------------------------------*/
	BITCR = 0x01;   	// fx/4096, Clock*4
	BITCR |= 0x08;  	// clear counter
	BITCR &= 0x7F;  	// clear BIT Interrupt Flag
	IE3 |= 0x10;    	// Enable BIT Interrupt
}

void Timer0_init()
{
	/*--- Set Timer0 (8-Bit, 1us Interval) -------------------------------------------
   * T0CR 	: 1001 0011 = 0x93			: Timer0 Control Register
   * 					8bit PWM, period = fx/4/256 = 2MHz/256 = 7.81KHz
   * T0DR 	: 0x7F									: Timer0 Data Register
   *  	      duty rate = 50%
   *----------------------------------------------------------------------------------*/
	T0CR = 0x93;    	// PWM setting
	T0DR = 0xFF;    	// period count, duty rate 50%
	IE2 |= 0x01;    	// Enable Timer0 overflow Interrupt
	T0CR |= 0x01;   	// clear counter
	IE2 |= 0x02;    	// Enable Timer0 Interrupt
}

void Timer1_init()
{
	// initialize Timer1
	// 16bit PWM, period = 100.000000mS ( 10.000000Hz )
	//     PWM duty = 50.000000%
	T1CRH = 0x20;   	// PWM setting High, 1 shot mode
	T1CRH |= 0x30;  	// PWM repeat mode
	T1CRL = 0x84;   	// PWM setting Low, Start Low
	T1ADRH = 0x00;  	// period count High
	T1ADRL = 0xFF;  	// period count Low
	T1BDRH = 0x01;  	// duty High
	T1BDRL = 0x00;  	// duty Low
	IE2 |= 0x04;    	// Enable Timer1 interrupt
	T1CRH |= 0x80;  	// enable counter
}

void Timer2_init()
{
	// initialize Timer2
	// 16bit PWM, period = 100.000000mS ( 10.000000Hz )
	//     PWM duty = 50.000000%
	T2CRH = 0x20;   	// PWM setting High, 1 shot mode
	T2CRH |= 0x30;  	// PWM repeat mode
	T2CRL = 0x84;   	// PWM setting Low, Start Low
	T2ADRH = 0x00;  	// period count High
	T2ADRL = 0xFF;  	// period count Low
	T2BDRH = 0x01;  	// duty High
	T2BDRL = 0x00;  	// duty Low
	IE2 |= 0x08;    	// Enable Timer2 interrupt
	T2CRH |= 0x80;  	// enable counter
}

void WT_init()
{
	// initialize Watch timer
//	WTCR = 0xA3;    	// setting, 1msec
	WTCR = 0x92;    	// Enable Watch Timer, WT interrupt gen., fx/128/2^7
	WTDR = 0x01;    	// set duty
	WTDR = 0x81;    	// clear WT
	IE3 |= 0x04;    	// Enable WT interrupt
}

void WDT_Init(void)
{
	// Initialize Watch Dog Timer peripheral
	// rst_on	This parameter contains the enable of watch dog timer reset.
	//   - WDT_FREE_RUN		= 0
	//   - WDT_RST_ON			= 1
	// wdtck	This parameter contains the selection of Clock.
	//   - WDT_BIT_OVER   = 0
	//   - WDT_WDTRC      = 1
	
	WDTCR = 0xC1;
	WDTDR = 127;
	IE3 |= 0x08;
}

void UART_init()
{
	// initialize UART interface
	// ASync. 57600 N 8 1
	UARTBD = 0x0C;  	// baud rate
	UARTCR1 = 0x06; 	// bit count, parity
	UARTCR2 = 0x0A; 	// interrupt, speed, activate UART
	UARTCR3 = 0x00; 	// stop bit
	IE1 |= 0x10;    	// enable UART TX interrupt
}

void UART_write(uint8_t dat)
{
	while(!(UARTST & 0x80));	// wait
	UARTDR = dat;   	// write
}

void UART_SendString(uint8_t *send_data, uint8_t count)
{
	uint8_t i;
	
	for(i = 0; i < count; i++)
	{
		while(!(UARTST & 0x80));
			UARTDR = send_data[i];
	}
}

void putstring(char *str)
{
	while(*str != 0)
		UART_SendString(str++, 1);
	
//	UART_write(0x0a);
//	UART_write(0x0d);
}

void hex2ascii(uint8_t ch)
{
	uint8_t tmp;
	
	tmp = ch >> 4;
	if(tmp < 0x0a)
		tmp = tmp + 0x30;
	else
		tmp = tmp + 0x37;
	UART_write(tmp);

	tmp = ch & 0x0F;
	if(tmp < 0x0a)
		tmp = tmp + 0x30;
	else
		tmp = tmp + 0x37;
	UART_write(tmp);
}

//uint8_t UART_read()
//{
//	uint8_t dat;
//	
//	while(!(UARTST & 0x20));	// wait
//	dat = UARTDR;   	// read
//	return	dat;
//}

void SPI_init()
{
	// initialize SPI interface
	SPICR = 0xA3;   	// setting, fx/128
	SPISR = 0x00;   	// SS pin enable
//	IE3 |= 0x02;    	// enable SPI interrupt
}

void SPI_SendData(uint8_t send_data)
{
	uint8_t tmp;

	SPIDR = send_data;
	while(!(SPISR & 0x80));
	tmp = SPIDR;

}

uint8_t SPI_ReceiveData(void)
{
	SPIDR = 0x00;
	while(!(SPISR & 0x80));		// Wait Data in
	
	return SPIDR;
}

void port_init()
{
	// initialize ports
	
// P0 setting
	//PIN 22 : P06      out 
	//PIN 23 : P05      out 
	//PIN 24 : P04      out LED9
	//PIN 25 : P03      out 
	//PIN 26 : P02      out LED8
	//PIN 27 : P01      out DSDL
	//PIN 28 : P00      out DSDA
	P0		= _0000_0000;		// 0 : Low,	1 : High
	P0OD	= _0000_0000;		// 0 : Disable,	1 : Enable (Open-drain)
	P0PU	= _0000_0000;		// 0 : Disable,	1 : Enable (Pull-up)
	P0IO	= _1111_1111;		// 0 : Input,	1 : Output
	P03DB = _1000_0000;  	// bit7~6(debounce clock), bit5~0=P35,P06~02 debounce
			
	P0FSR = _0000_0000;
	//BIT[7]		: P06     // 0 : I/O (EINT4), 1 : AN6
	//BIT[6]		: P05     // 0 : I/O (EINT3), 1 : AN5
	//BIT[5]		: P04     // 0 : I/O (EINT2), 1 : AN4
	//BIT[4]		: P03     // 0 : I/O (EINT1), 1 : AN3
	//BIT[3:2]	: P02     // 0 : I/O (EINT0), 1 : AVREF,      2 : AN2,      3 : reserved
	//BIT[1] 		: P01     // 0 : I/O				, 1 : AN1
	//BIT[0] 		: P00			// 0 : I/O				, 1 : AN0
		
// P1 setting
	//PIN 14 : P17,SS   in SS
	//PIN 15 : P16,SCK  out SCK
	//PIN 16 : PI5,MOSI out MOSI
	//PIN 17 : P14,MISO in  MISO
	//PIN 18 : P13      out LED_BLUE
	//PIN 19 : P12      out LED_GREEN
	//PIN 20 : P11      out FCSB
	//PIN 21 : P10      in SWITCH_IN
	P1		= _0000_0000;		// 0 : Low,	1 : High
	P1OD	= _0000_0000;		// 0 : Disable,	1 : Enable (Open-drain)
	P1PU	= _0000_0000;		// 0 : Disable,	1 : Enable (Pull-up)
	P1IO	= _1110_1110;		// 0 : Input,	1 : Output
	P12DB = _0000_0001;   // debounce : P23~20, P13~10
	
	P1FSRH = _0001_0101;
	//BIT6			: P17     // 0 : I/O,					1 : AN14
	//BIT[5:4]	: P16     // 0 : I/O, 				1 : SCK,      	2 : AN13,      3 : reserved
	//BIT[3:2]	: P15     // 0 : I/O,         1 : MOSI,				2 : AN12,			3 : reserved
	//BIT[1:0]	: P14     // 0 : I/O,         1 : MISO,      	2 : AN11,			3 : reserved
	P1FSRL = _0010_1000;
	//BIT[6:5]	: P13     // 0 : I/O (EINT12),1 : T2O/PWM2O,  2 : AN10,     3 : reserved
	//BIT[4:3]	: P12     // 0 : I/O (EINT11),1 : T1O/PWM1O,  2 : AN9,    	3 : reserved
	//BIT[2:1]	: P11     // 0 : I/O (EINT6),	1 : BUZO,      	2 : AN8,    	3 : reserved
	//BIT0			: P10     // 0 : I/O (EINT5), 1 : AN7

// P2 setting
	//  07 : P26      out LED7
	//  08 : P25      out LED6
	//  09 : P24      out LED5
	//  10 : P23      out LED4
	//  11 : P22      out LED3
	//  12 : P21      out LED2
	//  13 : P20      out LED1
	P2		= _0000_0000;		// 0 : Low,	1 : High
	P2OD	= _0000_0000;		// 0 : Disable,	1 : Enable (Open-drain)
	P2PU	= _0000_0000;		// 0 : Disable,	1 : Enable (Pull-up)
	P2IO	= _1111_1111;		// 0 : Input,	1 : Output
	
	P2FSR = _0000_0000;   	// P2 selection
	//BIT1		: P21     	// 0 : I/O,         1 : SCL
	//BIT0		: P20     	// 0 : I/O,         1 : SDA

// P3 setting    
	//   31 : P37,XOUT		out XOUT
	//   32 : P36,XIN			out XIN
	//   01 : P35      		out LED_RED
	//   02 : P34,SXIN 		out SXIN
	//   03 : P33,SXOUT   out SXOUT
	//   04 : P32,RESETB  in  RESET
	//   05 : P31,RXD     in	UART_RXD
	//   06 : P30,TXD     out UART_TXD
	P3		= _0000_0000;		// 0 : Low,	1 : High
	P3OD	= _0000_0000;		// 0 : Disable,	1 : Enable (Open-drain)
	P3PU	= _0000_0110;		// 0 : Disable,	1 : Enable (Pull-up)
	P3IO	= _1011_1001;		// 0 : Input,	1 : Output

	P3FSR = _1110_0001;   	// P3 selection
	//BIT7			: P37			// 0 : I/O,   			1 : XOUT
	//BIT6			: P36			// 0 : I/O,   			1 : XIN
	//BIT5			: P35			// 0 : I/O,   			1 : T0O/PWM0O
	//BIT4			: P34			// 0 : I/O,   			1 : SXIN
	//BIT3			: P33			// 0 : I/O,   			1 : SXOUT
	//BIT2			: P31			// 0 : I/O,   			1 : SCL
	//BIT[1:0]	: P30 		// 0 : I/O,   			1 : TXD,				2 : SDA,				3 : reserved
}

void port_EINT_init(void)
{
	uint8_t	tmp;
	
	tmp	= EIPOL0H;
	tmp = tmp | 0x04;
	EIPOL0H = tmp;
	IE1 |= 0x01;
}


