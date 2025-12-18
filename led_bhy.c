//======================================================
// Task program routine
// - Device name  : MC96F8316
// - Package type : QFN 32PIN
//======================================================

#include	"MC96F8316.h"
#include	"common.h"
#include	"config.h"
#include	"delay.h"

extern uint16_t main_tick_up;
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
uint8_t OrderOfColor[NUM_COLOR][3] =
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
	{99,99,99}		// MIX COLOR
};

uint8_t OrderOfMode[NUM_MODE] =
{
	MODE_ORDER1,
	MODE_ORDER2,
	MODE_ORDER3,
	MODE_ORDER4,
	MODE_ORDER5,
	MODE_ORDER6,
	MODE_ORDER7
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
	uint8_t gamma_duty_rate;
	
	//gamma_duty_rate = (duty_rate * duty_rate) / 100;
	
	tmp_duty_rate = 100 - duty_rate;
	
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
			break;
		case 12:
			LED3 = HIGH;
			LED4 = HIGH;
			break;
		case 13:
			LED5 = HIGH;
			LED6 = HIGH;
			break;
		case 14:
			LED7 = HIGH;
			LED8 = HIGH;
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

void LED_Rainbow_SetON(uint8_t Set_Color)
{
	switch (Set_Color)
	{
		case 1:
		case 2:
			setLED_Color(0);	// RED
			Sel_LED(11);			// LED 1 & 2
			break;
		case 3:
		case 4:
			setLED_Color(3);	// YELLOW
			Sel_LED(12);			// LED 3 & 4
			break;
		case 5:
		case 6:
			setLED_Color(4);	// GREEN
			Sel_LED(13);			// LED 5 & 6
			break;
		case 7:
		case 8:
			setLED_Color(5);	// BLUE
			Sel_LED(14);			// LED 7 & 8
			break;
		case 9:
			setLED_Color(8);	// WHITE
			Sel_LED(9);				// LED 9
			break;
	}
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
	static uint8_t tick_count = 0;
	
	if(sync==1)
	{
		dim_schedule = 0;
		Red = 0;
		Green = 0;
		Blue = 0;
		tick_count = 0;
		return 0;
	}
	else if(sync==2)
	{
		return dim_schedule;
	}
	
	if((dim_schedule == 0) && (pwm_tick_up == 5))
	{
		tick_count = tick_count + 5;
		
		Red = R * tick_count / 100;
		Green = G * tick_count / 100;
		Blue = B * tick_count / 100;

		if(tick_count > 95)
		{
			tick_count = 0;
			dim_schedule=1;
		}
		pwm_tick_up = 0;
	}
	else if(pwm_tick_up == 5)
	{
		tick_count = tick_count - 5;
		
		Red = R * tick_count / 100;
		Green = G * tick_count / 100;
		Blue = B * tick_count / 100;

		if(tick_count < 5)
		{
			Red = 0;
			Green = 0;
			Blue = 0;
			tick_count = 0;
			dim_schedule=0;
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
	
	if(rotate_tick_up>94 || sync) rotate_tick_up = 0;
	
	phase = rotate_tick_up/10;
	if(phase >= 8) phase = 8;
	
	pwm_setduty_percent(PWM_CH_R, R);
	pwm_setduty_percent(PWM_CH_G, G);
	pwm_setduty_percent(PWM_CH_B, B);

	Sel_LED(phase+1);
}

void LED_SetRotate(void)
{
	uint8_t phase = 0;
	
	if(rotate_tick_up>94) rotate_tick_up = 0;
	
	phase = rotate_tick_up/10;
	if(phase >= 8) phase = 8;
	
	if(color_now == (NUM_COLOR-1))
		setLED_Color(phase+1);
	else
		setLED_Color(color_now);
	
	Sel_LED(phase+1);
}

void LED_SetRoll_RF(uint8_t R, uint8_t G, uint8_t B, uint8_t sync)
{
	uint8_t phase = 0;
	
	if(rotate_tick_up>49 || sync) rotate_tick_up = 0;
	
	phase = rotate_tick_up/5;
	if(phase >= 8) phase = 8;
	
	pwm_setduty_percent(PWM_CH_R, R);
	pwm_setduty_percent(PWM_CH_G, G);
	pwm_setduty_percent(PWM_CH_B, B);

	Sel_LED(phase+1);
}

void LED_SetRoll(void)
{
	uint8_t phase = 0;

	if(rotate_tick_up>49) rotate_tick_up = 0;
	
	phase = rotate_tick_up/5;
	if(phase >= 8) phase = 8;
	
	if(color_now == (NUM_COLOR-1))
		setLED_Color(phase+1);
	else
		setLED_Color(color_now);
	
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
			//setLED_Color(r_v, g_v, b_v);
			break;

		case 0x02: //Flick
			LED_SetFLK(r_v, g_v, b_v,0);
			break;

		case 0x03: //Dimming
			LED_SetDIM(r_v, g_v, b_v,0);
			break;

		case 0x04: //Blink
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
	static uint8_t LED_Color = 0;
	static uint8_t u8Random_num = 0;
	uint8_t mode;
	
	mode = OrderOfMode[rotate_on];
	
	switch(mode)
	{
		case 0:			// On
			if(color_now == (NUM_COLOR-1))
			{
				LED_Rainbow_SetON(LED_Color);
				LED_Color++;
				if(LED_Color > (NUM_COLOR-1)) LED_Color = 0;
				
				color_old = color_now;
			}
			else
			{
				Sel_LED(LED_ALL_ON);
				if(color_old != color_now)
				{
					setLED_Color(color_now);
					color_old = color_now;
				}
			}
			break;
		case 1:			// Dimming
			if(color_old == color_now)
			{
				Sel_LED(LED_ALL_ON);
				switch (color_now)
				{
					case 0:	// RED
						LED_SetDIM(99,0,0,0);
						break;
					case 1:	// ORANGE
						LED_SetDIM(90,26,0,0);
						break;
					case 2:	// PINK
						LED_SetDIM(90,12,35,0);
						break;
					case 3:	// YELLOW
						LED_SetDIM(99,60,0,0);
						break;
					case 4:	// GREEN
						LED_SetDIM(0,90,0,0);
						break;
					case 5:	// BLUE
						LED_SetDIM(0,0,90,0);
						break;
					case 6:	// SKY
						LED_SetDIM(0,99,84,0);
						break;
					case 7:	// PURPLE
						LED_SetDIM(99,21,91,0);
						break;
					case 8:	// WHITE
						LED_SetDIM(70,70,70,0);
						break;
					default:
						LED_SetDIM(80,80,0,0);
						break;
				}					
			}
			else
			{
				color_old = color_now;
				LED_SetDIM(0,0,0,1);
			}
			break;
		case 2:			// Blink
			if(main_tick_up < 20)
			{
				if(color_now == (NUM_COLOR-1))
				{
					LED_Rainbow_SetON(LED_Color);
					LED_Color++;
					if(LED_Color > 9) LED_Color = 1;
				}
				else
				{
					setLED_Color(color_now);
					Sel_LED(LED_ALL_ON);
				}
			}
			else if(main_tick_up < 40)
				Sel_LED(LED_ALL_OFF);
			else if(main_tick_up == 40)
				main_tick_up = 0;
			break;
		case 3:			// Flicker
			if(main_tick_up < 10)
			{
				if(color_now == (NUM_COLOR-1))
				{
					LED_Rainbow_SetON(LED_Color);
					LED_Color++;
					if(LED_Color > 9) LED_Color = 1;
				}
				else
				{
					setLED_Color(color_now);
					Sel_LED(LED_ALL_ON);
				}
			}
			else if(main_tick_up < 20)
				Sel_LED(LED_ALL_OFF);
			else if(main_tick_up == 20)
				main_tick_up = 0;
			break;
		case 4:			// Rotate
			LED_SetRotate();
			break;
		case 5:			// Roll
			LED_SetRoll();
			break;
		case 6: //Random
			if(main_tick_up < 20)
			{
				setLED_Color(RandowColor[u8Random_num]);
				Sel_LED(LED_ALL_ON);
			}
			else if(main_tick_up < 40)
				Sel_LED(LED_ALL_OFF);
			else if(main_tick_up == 40)
			{
				main_tick_up = 0;
				u8Random_num++;
				if(u8Random_num >= (NUM_COLOR-1)) u8Random_num = 0;
			}
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
