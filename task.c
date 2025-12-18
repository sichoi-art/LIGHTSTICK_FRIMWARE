//======================================================
// Task program routine
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"
#include	"config.h"
#include	"delay.h"
#include	"CMT2219A.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Definition                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

uint16_t msTicks;
uint8_t TimeIndex = 0;

static uint8_t u8Powerdown_mode_flag = 1;

uint8_t color_now = 0;
uint8_t color_old = 0;
uint8_t rotate_on = 0;

uint8_t play_mode = PLAY_LOCAL;

static uint8_t u8Timeout_Remote = 0;

extern uint16_t main_tick_up;
extern uint16_t u32Button_Pressed_ts;
extern uint16_t u32Button_Released_ts;

void pwm_off(void)
{
//	P3FSR &= ~0x20;  	// PWM0 disable
	T0DR = 0xFF;  	// PWM0 disable
	T1BDRH = 0x01;  	// PWM1 disable
	T1BDRL = 0x00;
	T2BDRH = 0x01;  	// PWM2 disable
	T2BDRL = 0x00;
}

void enter_stop(void)
{
	Sel_LED(LED_ALL_OFF);
	pwm_off();
	cmt2219a_vGoSleep(); 
	NOP_10us_Delay(100);
	
	/* Enter STOP Mode */ 
	PCON = 0x03;

	/* Must execute NOP */ 
	NOP;
	NOP;
	NOP;
	
	//Sel_LED(LED_ALL_ON);
}

void longTapAction() 
{
	color_now = 0;
	color_old = 0;
	rotate_on = 0; 
	
	setLED_Color(color_now);
	Sel_LED(LED_ALL_ON);
	
	cmt2219a_Init();
	NOP_10us_Delay(50);
//	putstring("CMT2219A Status : 0x");
//	hex2ascii(cmt2219a_bReadStatus());
//	putstring("\n");
//	putstring("Release STOP Mode...\n");

}

uint8_t btn_action(uint8_t Button_Event)
{
	switch(Button_Event)
	{
		case NO_PRESS:
			return 0;
		break;
		case SINGLE_PRESS:
			if(u8Powerdown_mode_flag == 1)
				return 0;
			else
			{
				if((play_mode == PLAY_REMOTE) && (u8Timeout_Remote == 0))
				{
					return 0;
				}
				else
				{
					play_mode = PLAY_LOCAL;
					color_now++;
					if(color_now > (NUM_COLOR-2)) color_now = 0;
					return 1;
				}
			}
		break;
		case LONG_PRESS:
			play_mode = PLAY_LOCAL;
			longTapAction();
			return 2;
		break;
		case DOUBLE_PRESS:
			if(u8Powerdown_mode_flag == 1)
				return 0;
			else
			{
				if((play_mode == PLAY_REMOTE) && (u8Timeout_Remote == 0))
				{
					return 0;
				}
				else
				{
					play_mode = PLAY_LOCAL;
					rotate_on++;
					if(rotate_on>(NUM_MODE-1)) rotate_on = 0;
					return 3;
				}

			}
		break;
		default:
			return 0;
		break;
	}
}

void Task_Power(uint8_t btn_event)
{
//	static uint8_t old_btn_event = NO_PRESS;
//	if(btn_event) hex2ascii(btn_event);

		if(u8Powerdown_mode_flag == 0)
		{
			if(btn_event == LONG_PRESS)
			{
				u8Powerdown_mode_flag = 1;
				enter_stop();							// enable Idle and stop mode 
			}
			else btn_action(btn_event); // normal active single and double click. 
				
		}
		else
		{
			if(btn_event == LONG_PRESS)
			{
				u8Powerdown_mode_flag = 0;
				btn_action(btn_event);  // start light stick and lighting on.
			}
				else enter_stop(); // in Power idle mode when button click event single and double  
		}
}
	
void Run_Task(void)
{
	uint8_t btn_event = NO_PRESS;
	static uint8_t link_count = 0;
	
	CSB = 1;
	FCSB = 1;

	NOP_10us_Delay(100);
	cmt2219a_Init();
//	cmt2219a_vGoSleep(); 
	NOP_10us_Delay(50);

	pwm_off();
	NOP_10us_Delay(50);
	
	enter_stop();
	u8Powerdown_mode_flag = 1;

	cmt2219a_Init();
	NOP_10us_Delay(50);

	putstring("Start...");
	UART_write(0x0a);
	UART_write(0x0d);
	
	while(1)
	{
		if(u8Powerdown_mode_flag == 0)
			Task_LED(play_mode);
		

			btn_event = Task_Button();
			if(btn_event > 0)Task_Power(btn_event);

		
		if(msTicks > 1) // 1 tick - 1ms
		{   
			msTicks = 0;

			TimeIndex++;
			
			if( TimeIndex == 10)
			{
				if(u8Powerdown_mode_flag == 0)
				{
					if(Task_CMT2219A())
					{
            play_mode = PLAY_REMOTE;
            link_count = 0;
						u8Timeout_Remote = 0;
					}
					else {
						if(link_count < 100)
							link_count++;

            if (link_count >= 100)
						{
              u8Timeout_Remote = 1;
						}
					}
				}
				TimeIndex = 0;
			}
		}

	}  // While
}
