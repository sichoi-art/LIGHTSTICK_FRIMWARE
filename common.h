
#include	"typedef.h"
#include	"delay.h"

#define HIGH 	1
#define LOW 	0

#define LED_ALL_OFF 	0
#define LED_ALL_ON		10

#define PLAY_REMOTE		0
#define PLAY_LOCAL		1

/*******************************************************************************
* LED Constant  Assignment
*******************************************************************************/

/* QFP24 GPIO */
#define	LED1			P20
#define	LED2			P21
#define	LED3			P22
#define	LED4			P23
#define	LED5			P24
#define	LED6			P25
#define	LED7			P26
#define	LED8			P02
#define	LED9			P04

#define	RLED			P35
#define	GLED			P12
#define	BLED			P13

#define P_SWITCH	P10

//#define ADC_IN		P03

#define FCSB			P11
#define FCS_LOW		P11=0
#define FCS_HIGH	P11=1

#define CSB				P17
#define CS_LOW		P17=0
#define CS_HIGH		P17=1

#define SCLK			P16
#define SCLK_L		P16=0
#define SCLK_H		P16=1

#define MOSI			P15
#define MOSI_L		P15=0
#define MOSI_H		P15=1

#define MISO			P14
#define MISO_L		P14=0
#define MISO_H		P14=1

#define PWM_CH_R	0 
#define PWM_CH_B	1
#define PWM_CH_G	2

/*******************************************************************************
* Button Constant & Typedef  Assignment
*******************************************************************************/
#define BUTTON_RESET	1
#define GET_STATE			0

#define DEBOUNCE_MILLIS 				20
#define DOUBLE_GAP_MILLIS_MAX 	150
#define LONG_MILLIS_MIN					600

#define NO_PRESS				0
#define SINGLE_PRESS		1
#define LONG_PRESS			2
#define DOUBLE_PRESS		3
#define BUTTON_PRESSED		4


/*******************************************************************************
* SPI Constant  Assignment
*******************************************************************************/
#define SPI_SLAVE_MODE			0
#define SPI_MASTER_MODE			1

#define SPI_MSB							0
#define SPI_LSB							1

#define SPI_CPOL_LOW				0
#define SPI_CPOL_HIGH				1

#define SPI_CPHA_1EDGE			0
#define SPI_CPHA_2EDGE			1

#define SPI_SS_HW_DISABLE		0
#define SPI_SS_HW_ENABLE		1

#define SPI_MAX_BUFFER_SIZE	8

//======================================================
// Function and global variables definition
//======================================================

#define MAIN_TICK_INTERVAL	2


void LED_ALL(uint8_t onoff, uint8_t except);

void clock_init();            	// initialize operation clock
void BIT_init();              	// initialize Basic interval timer

void SPI_init();              	// initialize SPI interface
void SPI_SendData(uint8_t send_data);
uint8_t SPI_ReceiveData(void);

void Timer0_init();           	// initialize Timer0
void Timer1_init();           	// initialize Timer1
void Timer2_init();           	// initialize Timer2
void UART_init();             	// initialize UART interface
void UART_write(unsigned char dat);	// write UART
void UART_SendString(uint8_t *send_data, uint8_t count);
void putstring(char *str);
void hex2ascii(uint8_t ch);

void WT_init(void);               	// initialize Watch timer
void WDT_Init(void);
void port_init(void);             	// initialize ports
void port_EINT_init(void);


void system_Delay_ms(uint16_t ms);
uint8_t getButtonEvent(void);

void Sel_LED(uint8_t u8Sel_LED);
void setLED_Color(uint8_t color);
void LED_Rainbow_SetON(uint8_t Set_Color);

void Run_Task(void);
void Task_Power(uint8_t btn_event);
uint8_t Task_Button(void);
void Task_LED(uint8_t mode);
uint8_t Task_CMT2219A(void);
