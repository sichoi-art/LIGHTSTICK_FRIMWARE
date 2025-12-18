//======================================================
// Task program routine
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"
#include	"config.h"
#include	"delay.h"

extern uint8_t pwm_tick_up;
extern uint16_t rotate_tick_up;

extern uint8_t color_now;
extern uint8_t color_old;
extern uint8_t rotate_on;

uint8_t pattern_now = 0;

uint8_t remote_r;
uint8_t remote_g;
uint8_t remote_b;
uint8_t remote_mode;

// Color order
uint8_t OrderOfColor[10][3] =
{
	COLOR_ORDER1,
	COLOR_ORDER2,
	COLOR_ORDER3,
	COLOR_ORDER4,
	COLOR_ORDER5,
	COLOR_ORDER6,
	COLOR_ORDER7,
	COLOR_ORDER8,
	COLOR_ORDER9,
	COLOR_ORDER10
//	{99,99,99}		// MIX COLOR
};

uint8_t OrderOfMode[6] =
{
	MODE_ORDER1,
	MODE_ORDER2,
	MODE_ORDER3,
	MODE_ORDER4,
	MODE_ORDER5,
	MODE_ORDER6
};

uint8_t RandowColor[9] =
{
	1,5,7,4,8,2,6,9,3
};

//void pwm_setduty_percent(uint8_t R_duty_rate, uint8_t G_duty_rate, uint8_t B_duty_rate)
void pwm_setduty_percent(uint8_t PWM_ch_num, uint8_t duty_rate)
{
	uint16_t pwm0_duty = 0;
	uint16_t pwm1L_duty, pwm1H_duty = 0;
	uint8_t tmp_duty_rate = 0;
	uint16_t gamma_duty_rate;
	
	gamma_duty_rate = (duty_rate * duty_rate) / 100;
	
	tmp_duty_rate = 100 - gamma_duty_rate;
	
	switch (PWM_ch_num)
	{
		case PWM_CH_R:
			pwm0_duty = (255 * tmp_duty_rate) / 100;
			T0DR = pwm0_duty;
			break;
		case PWM_CH_G:
			pwm1L_duty = (T1ADRL * tmp_duty_rate) / 100;
			pwm1H_duty = (T1ADRH * tmp_duty_rate) / 100;
			T1BDRL = pwm1L_duty;  				// duty Low
			T1BDRH = pwm1H_duty;  				// duty High
			break;
		case PWM_CH_B:
			pwm1L_duty = (T2ADRL * tmp_duty_rate) / 100;
			pwm1H_duty = (T2ADRH * tmp_duty_rate) / 100;
			T2BDRL = pwm1L_duty;  				// duty Low
			T2BDRH = pwm1H_duty;  				// duty High
			break;
		default:
			break;
	}
}

void Sel_LED(uint8_t u8Sel_LED)
{
	// LED OFF : LOW, LED_ON : HIGH
		LED1 = LOW;
		LED2 = LOW;
		LED3 = LOW;
		LED4 = LOW;
		LED5 = LOW;
		LED6 = LOW;
		LED7 = LOW;
		LED8 = LOW;
		LED9 = LOW;

	switch (u8Sel_LED)
	{
		case LED_ALL_OFF:
			LED1 = LOW;
			LED2 = LOW;
			LED3 = LOW;
			LED4 = LOW;
			LED5 = LOW;
			LED6 = LOW;
			LED7 = LOW;
			LED8 = LOW;
			LED9 = LOW;
			break;
		case 1:
			LED1 = HIGH;
			LED9 = HIGH;
			break;
		case 2:
			LED2 = HIGH;
			LED9 = HIGH;
			break;
		case 3:
			LED3 = HIGH;
			LED9 = HIGH;
			break;
		case 4:
			LED4 = HIGH;
			LED9 = HIGH;
			break;
		case 5:
			LED5 = HIGH;
			LED9 = HIGH;
			break;
		case 6:
			LED6 = HIGH;
			LED9 = HIGH;
			break;
		case 7:
			LED7 = HIGH;
			LED9 = HIGH;
			break;
		case 8:
			LED8 = HIGH;
			LED9 = HIGH;
			break;
		case 9:
			LED9 = HIGH;
			break;
		case LED_ALL_ON:
			LED1 = HIGH;
			LED2 = HIGH;
			LED3 = HIGH;
			LED4 = HIGH;
			LED5 = HIGH;
			LED6 = HIGH;
			LED7 = HIGH;
			LED8 = HIGH;
			LED9 = HIGH;
			break;
		case 11:
			LED1 = HIGH;
			LED2 = HIGH;
			LED9 = HIGH;
			break;
		case 12:
			LED3 = HIGH;
			LED4 = HIGH;
			LED9 = HIGH;
			break;
		case 13:
			LED5 = HIGH;
			LED6 = HIGH;
			LED9 = HIGH;
			break;
		case 14:
			LED7 = HIGH;
			LED8 = HIGH;
			LED9 = HIGH;
			break;
		default:
			break;
	}
}

void setLED_Color(uint8_t u8Color_order)
{
	uint8_t Red,Green,Blue;
	
	Red = OrderOfColor[u8Color_order][0];
	Green = OrderOfColor[u8Color_order][1];
	Blue = OrderOfColor[u8Color_order][2];

	pwm_setduty_percent(PWM_CH_R, Red);
	pwm_setduty_percent(PWM_CH_G, Green);
	pwm_setduty_percent(PWM_CH_B, Blue);
}

void setLED_Color_RF(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	pwm_setduty_percent(PWM_CH_R, Red);
	pwm_setduty_percent(PWM_CH_G, Green);
	pwm_setduty_percent(PWM_CH_B, Blue);
}

uint8_t LED_SetBlink(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	static uint8_t blink_schedule = 0;

	if(sync==1)
	{
		blink_schedule = 0;
		return 0;
	}
	else if(sync==2)
	{ 
		return blink_schedule;
	}


	if(pwm_tick_up> 9)
	{
		blink_schedule++;
		if(blink_schedule > 2)
		{
			pwm_setduty_percent(PWM_CH_R, 0);
			pwm_setduty_percent(PWM_CH_G, 0);
			pwm_setduty_percent(PWM_CH_B, 0);
			blink_schedule = 0;
		}
		else
		{
			pwm_setduty_percent(PWM_CH_R, R);
			pwm_setduty_percent(PWM_CH_G, G);
			pwm_setduty_percent(PWM_CH_B, B);
		}
		pwm_tick_up = 0;
	}
	return 0;
}

uint8_t LED_SetFLK(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	static uint8_t flk_schedule = 0;

	if(sync==1)
	{
		flk_schedule = 0;
		return 0;
	}
	else if(sync==2)
	{
		return flk_schedule;
	}


	if(pwm_tick_up> 4)
	{
		flk_schedule++;
		if(flk_schedule > 1)
		{
			pwm_setduty_percent(PWM_CH_R, 0);
			pwm_setduty_percent(PWM_CH_G, 0);
			pwm_setduty_percent(PWM_CH_B, 0);
			flk_schedule = 0;
		}
		else
		{
			pwm_setduty_percent(PWM_CH_R, R);
			pwm_setduty_percent(PWM_CH_G, G);
			pwm_setduty_percent(PWM_CH_B, B);
		}
		pwm_tick_up = 0;
	}
	return 0;
}

uint8_t LED_SetDIM(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	static uint8_t dim_schedule = 0;
	static uint8_t Red = 0, Green = 0, Blue = 0;
	static float Red_Temp = 0, Green_Temp = 0, Blue_Temp = 0;
	static uint8_t tick_count = 0;
	
	if(sync==1)
	{
		dim_schedule = 0;
//		Red = 0;
//		Green = 0;
//		Blue = 0;

		Red = R;			// CHOI JUNE 17
		Green = G;			// CHOI JUNE 17
		Blue = B;			// CHOI JUNE 17

		Red_Temp = R;			// CHOI JUNE 17
		Green_Temp = G;			// CHOI JUNE 17
		Blue_Temp = B;			// CHOI JUNE 17

		tick_count = 0;
		return 0;
	}
	else if(sync==2)
	{
		return dim_schedule;
	}
	
	if((dim_schedule == 0) && (pwm_tick_up > 0))
	{

		tick_count++;
//		if(Red > 0) Red--;
//		if(Green > 0) Green--;
//		if(Blue > 0) Blue--;

		if(R > Red) 
		{
			Red_Temp = ((float)(R+Red_Temp)*1.01)-(float)R;
			Red = (uint8_t)(Red_Temp);
		}			// CHOI JUNE 17
		if(G > Green)
		{
			Green_Temp =((float)(G+Green_Temp)*1.01)-(float)G;
			Green = (uint8_t)(Green_Temp); 			// CHOI JUNE 17
		}
		if(B > Blue)
		{
			Blue_Temp = ((float)(B+Blue_Temp)*1.01)-(float)B;
			Blue = (uint8_t)(Blue_Temp);				// CHOI JUNE 17
		}
		if(tick_count == 99)
		{
//			Red = 0;
//			Green = 0;
//			Blue = 0;

			Red = R;			// CHOI JUNE 17
			Green = G;			// CHOI JUNE 17
			Blue = B;			// CHOI JUNE 17

			Red_Temp = R;			// CHOI JUNE 17
			Green_Temp = G;			// CHOI JUNE 17
			Blue_Temp = B;			// CHOI JUNE 17

			tick_count = 0;
			dim_schedule=1;
		}
		pwm_tick_up = 0;

	}
	else if(pwm_tick_up > 0)
	{
		tick_count++;
		
//		if(Red < R) Red++;
//		if(Green < G) Green++;
//		if(Blue < B) Blue++;
	
		if(Red > 0)
		{
			Red_Temp = (Red_Temp * 0.99);			// CHOI JUNE 17
			Red = Red_Temp;
		}
		if(Green > 0)
		{
			Green_Temp = (Green_Temp * 0.99);			// CHOI JUNE 17
			Green = Green_Temp;
		}
		if(Blue > 0) 
		{
			Blue_Temp = (Blue_Temp * 0.99);			// CHOI JUNE 17
			Blue = Blue_Temp;
		}
			
		if(tick_count == 99)
		{
			tick_count = 0;
			dim_schedule=0;

			Red = 0;			// CHOI JUNE 17
			Green = 0;			// CHOI JUNE 17
			Blue = 0;			// CHOI JUNE 17

			Red_Temp = 0;			// CHOI JUNE 17
			Green_Temp = 0;			// CHOI JUNE 17
			Blue_Temp = 0;			// CHOI JUNE 17
			
		}
		pwm_tick_up = 0;

	}
	
	pwm_setduty_percent(PWM_CH_R, Red);
	pwm_setduty_percent(PWM_CH_G, Green);
	pwm_setduty_percent(PWM_CH_B, Blue);
	return 0;
}

void LED_SetRotate_RF(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	uint8_t phase = 0;
	
//	if(rotate_tick_up>94 || sync) rotate_tick_up = 0;
	if(rotate_tick_up>88 || sync) rotate_tick_up = 0;
	
	phase = rotate_tick_up/11;
	if(phase >= 8) phase = 8;
	
	pwm_setduty_percent(PWM_CH_R, R);
	pwm_setduty_percent(PWM_CH_G, G);
	pwm_setduty_percent(PWM_CH_B, B);

	Sel_LED(phase+1);
}

void LED_SetRoll_RF(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	uint8_t phase = 0;
	
	if(rotate_tick_up>40 || sync) rotate_tick_up = 0;
	
	phase = rotate_tick_up/5;
	if(phase >= 8) phase = 8;
	
	pwm_setduty_percent(PWM_CH_R, R);
	pwm_setduty_percent(PWM_CH_G, G);
	pwm_setduty_percent(PWM_CH_B, B);

	Sel_LED(phase+1);
}

void Run_Remote(void)
{
	uint8_t r_v, g_v, b_v;
	
	if(remote_r == 100) r_v = 99;
	else r_v = remote_r;

	if(remote_g == 100)	g_v = 99;
	else g_v = remote_g;

	if(remote_b == 100)	b_v = 99;
	else b_v = remote_b;
	
	switch (remote_mode)
	{
		case 0x01: //ON
			Sel_LED(LED_ALL_ON);
			setLED_Color_RF(r_v, g_v, b_v);
			break;

		case 0x02: //Flick
			Sel_LED(LED_ALL_ON);
			LED_SetFLK(r_v, g_v, b_v,0);
			break;

		case 0x03: //Dimming
			Sel_LED(LED_ALL_ON);
			LED_SetDIM(r_v, g_v, b_v,0);
			break;

		case 0x04: //Blink
			Sel_LED(LED_ALL_ON);
			LED_SetBlink(r_v, g_v, b_v,0);
			break;

		case 0x05: //Rotate
			LED_SetRotate_RF(r_v, g_v, b_v,0);
			break;
		case 0x06: //Roll
			LED_SetRoll_RF(r_v, g_v, b_v,0);
			break;
		default:
			break;
	}
}

void Run_Local(void)
{
//	static uint8_t LED_Color = 0;
//	static uint8_t u8Random_num = 0;
	uint8_t mode;
	static uint8_t old_mode = 0;
	uint8_t Red,Green,Blue;
	
	Red = OrderOfColor[color_now][0];
	Green = OrderOfColor[color_now][1];
	Blue = OrderOfColor[color_now][2];	

	mode = OrderOfMode[rotate_on];

	if(old_mode != mode)
	{
		old_mode = mode;
		pwm_tick_up = 0;
	}

	switch(mode)
	{
		case 0:			// On
			Sel_LED(LED_ALL_ON);
			setLED_Color(color_now);
			break;
		case 1:			// Dimming
			if(color_old == color_now)
			{
				Sel_LED(LED_ALL_ON);
				LED_SetDIM(Red,Green,Blue,0); //

				/*
				switch (color_now)
				{
					case 0:	// 
//						LED_SetDIM(99,21,91,0);
//						LED_SetDIM(0,90,0,0); // GREEN
						break;
					case 1:	// 
//						LED_SetDIM(99,21,91,0);
						LED_SetDIM(99,99,99,0);	//WHITE
					break;
					case 2:	
						LED_SetDIM(0,0,90,0);
						LED_SetDIM(0,90,0,0); // GREEN
						break;
					case 3:	// MINT
						LED_SetDIM(5,70,30,0);
						break;
					case 4:	// PINK
						LED_SetDIM(90,9,27,0);
						break;
					case 5:	// PURPLE
						LED_SetDIM(99,21,91,0);
						break;
					case 6:	// RED
						LED_SetDIM(90,0,0,0);
						break;
					case 7:	// LEMON
						LED_SetDIM(88,95,31,0);
						break;
					case 8:	// NAVY
						LED_SetDIM(0,0,50,0);
						break;
					default:
						LED_SetDIM(0,0,0,0);
						break;
				}					
*/

	}
			else
			{
				color_old = color_now;
				LED_SetDIM(0,0,0,1);
			}
			break;
		case 2:			// Blink
			if(pwm_tick_up < 14)
			{
				setLED_Color(color_now);
				Sel_LED(LED_ALL_ON);
			}
			else if(pwm_tick_up < 29)
			{
				setLED_Color_RF(0,0,0);
				Sel_LED(LED_ALL_OFF);
			}
			else if(pwm_tick_up == 30)
				pwm_tick_up = 0;
			break;
		case 3:			// Flicker
			if(pwm_tick_up < 4)
			{
				setLED_Color(color_now);
				Sel_LED(LED_ALL_ON);
			}
			else if(pwm_tick_up < 9)
			{
				setLED_Color_RF(0,0,0);
				Sel_LED(LED_ALL_OFF);
			}
			else if(pwm_tick_up == 10)
				pwm_tick_up = 0;
			break;

		case 0x04: //Rotate
		LED_SetRotate_RF(Red,Green,Blue,0);

//		LED_SetRotate_RF(r_v, g_v, b_v,0);
			break;
		case 0x05: //Roll

		LED_SetRoll_RF(Red,Green,Blue,0);
//		LED_SetRoll_RF(r_v, g_v, b_v,0);
			break;


		default:
			break;
	}
}

void Task_LED(uint8_t mode)
{
	if(mode == PLAY_REMOTE)
		Run_Remote();
	else
		Run_Local();
}
