/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: 
 *  (1) "AS IS" WITH NO WARRANTY; 
 *  (2) TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, HopeRF SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) HopeRF
 *
 * website: www.HopeRF.com
 *          www.HopeRF.cn     
 */

/*! 
 * file       HopeDuino_CMT2119A.cpp
 * brief      driver for CMT2119A 
 * hardware   HopeDuino with RFM219S
 *            
 *
 * version    1.0
 * date       Feb 25 2016
 * author     QY Ruan
 */
#include	"MC96F8316.h"
#include	"common.h"
#include	"config.h"
#include	"CMT2219A.h"

/* 434MHz Setting */
//uint8_t CfgTbl[62] = {
//0x72,0x21,0x9B,0x7F,
//0x06,0xF3,0xA1,0xF0,
//0xBE,0x53,0x01,0x00,
//0x62,0x1E,0x00,0x10,
//0x84,0x14,0xE0,0x00,
//0x27,0x9F,0x00,0x00,
//0xAA,0xAA,0x00,0x14,
//0x40,0x01,0x4A,0x21,
//0x05,0x84,0x00,0x00,
//0x19,0x00,0x0F,0x1F,
//0xAC,0x56,0x53,0xD4,
//0x40,0x49,0xFF,0x5D,
//0x12,0x18,0x90,0xFA,
//0x00,0x00,0x40,0xC0,
//0x00,0x00,0x20,0xEB,
//0xFF,0x00
//};

///* 447MHz Setting */
//uint8_t CfgTbl[62] = {
//0x52,0x22,0x9B,0x7F,
//0x06,0xF3,0xA1,0xF0,
//0xBE,0x53,0x01,0x00,
//0x62,0x1E,0x00,0x10,
//0x84,0x28,0xE0,0x00,
//0x27,0x9F,0x00,0x00,
//0xAA,0xAA,0x00,0x14,
//0x40,0x01,0x4A,0x21,
//0x05,0x84,0x00,0x00,   
//0x19,0x00,0x0F,0x1F,
//0xAC,0x56,0x53,0xD4,
//0x40,0x49,0xFF,0x5D,
//0x12,0x18,0x90,0xFA,
//0x00,0x00,0x40,0xC0,
//0x00,0x00,0x20,0xEB,
//0xFF,0x00
//};

/* 447.9MHz Setting */
uint8_t CfgTbl[62] = {
0x52,0x22,0x2C,0x9B,
0x07,0xF3,0xA1,0xF0,
0xBE,0x53,0x01,0x00,
0x62,0x1E,0x00,0x10,
0x84,0x28,0xE0,0x00,			// 0x10 ~ 0x13
0x27,0x9F,0x00,0x00,
0xAA,0xAA,0x00,0x14,
0x40,0x01,0x4A,0x21,
0x05,0x84,0x00,0x00,   		// 0x20 ~ 0x23
0x19,0x00,0x0F,0x1F,
0xAC,0x56,0x53,0xD4,
0x40,0x49,0xFF,0x5D,
0x12,0x18,0x92,0xFA,			// 0x30 ~ 0x33
0x00,0x00,0x40,0xC0,
0x00,0x00,0x20,0xEB,			// 0x38 ~ 0x3B
0xFF,0x00
};

uint8_t CrcDisable= 0;					//false: CRC enable£¬ & use CCITT 16bit 
																//true : CRC disable
uint8_t FixedPktLength = 1;			//false: for contain packet length in Tx message, the same mean with variable lenth
																//true : for doesn't include packet length in Tx message, the same mean with fixed length
uint8_t NodeDisable = 1;				//false: Node Address Enable
																//true : Node Address Disable
uint8_t PktLength = 10;											

extern uint8_t remote_r;
extern uint8_t remote_g;
extern uint8_t remote_b;
extern uint8_t remote_mode;

uint8_t spi3wire_readbyte(uint8_t address)
{
	uint8_t tmp[2] = {0,};	

	CSB = 0;
	NOP_1us_Delay();
	NOP_1us_Delay();
	tmp[0] = address|0x80;
	SPI_SendData(tmp[0]);
	tmp[1] = SPI_ReceiveData();
	NOP_1us_Delay();
	NOP_1us_Delay();
	CSB = 1;

	return(tmp[1]);
}

/**
  * @brief  Write one byte.
  * @param  address: address of register which need to write data.
  * @param  data: data which need to write.
  * @return TRUE: write success, FALSE: write failure.
*/
void spi3wire_writebyte(uint8_t address, uint8_t datao)
{
	CSB = 0;
	NOP_1us_Delay();
	NOP_1us_Delay();
	SPI_SendData(address);
	SPI_SendData(datao);
	NOP_1us_Delay();
	NOP_1us_Delay();
	CSB = 1;
}

/**********************************************************
**Name:     vGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vGoRx(void)
{
	uint8_t tmp;
	tmp = spi3wire_readbyte(OP_MODE);
	tmp &= OP_MASK;
	spi3wire_writebyte(OP_MODE, tmp+OP_RX);
}

/**********************************************************
**Name:     vGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vGoSleep(void)
{
	uint8_t tmp;	
	tmp = spi3wire_readbyte((OP_MODE));
	tmp &= OP_MASK;
	spi3wire_writebyte(OP_MODE,tmp+OP_SLEEP);
}

/**********************************************************
**Name:     vGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vGoStandby(void)
{
	uint8_t tmp;	
	tmp = spi3wire_readbyte((OP_MODE));
	tmp &= OP_MASK;
	spi3wire_writebyte(OP_MODE,tmp+OP_STANDBY);	
}

/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vSoftReset(void)
{
	spi3wire_writebyte(SOFT_RST, 0xFF); 
}

/**********************************************************
**Name:     cmt2219a_vClearIntFlag
**Function: clear all irq flag
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vClearIntFlag(void)
{
	spi3wire_writebyte(INTCTL_B,0xFF);
}

/**********************************************************
**Name:     vClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vClearFIFO(void)
{
	uint8_t tmp;	
	tmp = spi3wire_readbyte((INTCTL_D));
	tmp=tmp+FIFO_CLR;
	spi3wire_writebyte(INTCTL_D, tmp);
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
//uint8_t cmt2219a_bReadStatus(void)
//{
//	return(0xE0&(spi3wire_readbyte((OP_MODE))));
//}

/**********************************************************
**Name:     vInit
**Function: Init. CMT2119A
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vInit(uint8_t cfg[])
{
	uint8_t i;
	
	for(i=0; i<62; i++)			//exp file have 62
			spi3wire_writebyte(i, cfg[i]);

	cmt2219a_vClearIntFlag();
	cmt2219a_vGoStandby();
}

/**********************************************************
**Name:     vGpioFuncCfg
**Function: GPIO Function config
**Input:    none
**Output:   none
**********************************************************/
void cmt2219a_vGpioFuncCfg(uint8_t io_cfg)
{
	spi3wire_writebyte(IO_SEL, io_cfg);
}

/**********************************************************
**Name:     vIntSourceCfg
**Function: config 
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void cmt2219a_vIntSourcCfg(uint8_t int_1, uint8_t int_2)
{
	spi3wire_writebyte(INTCTL_A, int_2+((int_1)));
}

/**********************************************************
**Name:     vEnableIntSource
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void cmt2219a_vEnableIntSource(uint8_t en_int)
{
	spi3wire_writebyte(INT_EN, en_int);				
}

/**********************************************************
**Name:     bReadIngFlag
**Function: Read interrupt flag(INTCTL_C)
**Input:    none
**Output:   interrupt flag
**********************************************************/
uint8_t cmt2219a_bReadIngFlag(void)
{
	return(spi3wire_readbyte((INTCTL_C)));
}	

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    none
**Output:   none
**********************************************************/
//uint8_t cmt2219a_bReadRssi(void)
//{
//	return(spi3wire_readbyte((RSSI_ADDR)));
//}

uint8_t bSpi3ReadFIFO(void)
{
	uint8_t RdPara;
	
	FCSB = 0;
	NOP_1us_Delay();
	NOP_1us_Delay();
	RdPara = SPI_ReceiveData();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	NOP_1us_Delay();
	FCSB = 1;
	
 	return(RdPara);
}

void vSpi3BurstReadFIFO(uint8_t ptr[], uint8_t length)
{
	uint8_t i;
	
	if(length!=0)
	{
		for(i=0;i<length;i++)
		{
			ptr[i] = bSpi3ReadFIFO();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
			NOP_1us_Delay();
		}
	}	
}

void cmt2219a_bGetStatus(void)
{
	uint8_t tmp;
	tmp = spi3wire_readbyte(OP_MODE);
	tmp &= OP_RX;

	if(tmp != OP_RX)
	{
		cmt2219a_Init();
		NOP_10us_Delay(50);
	}
}

/******************************************************************************
**函数名称：vbGetMessage
**函数功能：接收一包数据
**输入参数：无
**输出参数：1——接收成功
**          0——接收失败
******************************************************************************/
uint8_t cmt2219a_bGetMessage(uint8_t msg[])
{
	uint8_t tmp=0;
	uint8_t length=0;
	//uint8_t rssi = 0;

	//cmt2219a_vGoRx();
	//CLK_SysTickDelay(100000);

	tmp = cmt2219a_bReadIngFlag();
	//rssi = cmt2219a_bReadRssi();

	if(CrcDisable)
	{
		if(tmp&RX_DONE_FLG)		
		{
			if(FixedPktLength)
			{
				length = PktLength;
			}
			else
			{
				length = bSpi3ReadFIFO();
			}
			vSpi3BurstReadFIFO(msg, length);
			cmt2219a_vClearIntFlag();
			cmt2219a_vClearFIFO();
			//cmt2219a_vGoSleep();
			return(length);
		}
		else if(tmp!=0)
		{
			//cmt2219a_vClearIntFlag();
			//cmt2219a_vClearFIFO();
		}
	}
	else
	{
		if((tmp & 0x03) == 0x03)
		{
			if(FixedPktLength)
			{
				length = PktLength;
			}
			else
			{
				length = bSpi3ReadFIFO();
			}
			vSpi3BurstReadFIFO(msg, length);
			cmt2219a_vClearIntFlag();
			cmt2219a_vClearFIFO();
			//cmt2219a_vGoSleep();
			return(length);
		}
		else if(tmp!=0)
		{
			//cmt2219a_vClearIntFlag();
			//cmt2219a_vClearFIFO();
		}
	}
	
	//cmt2219a_vGoSleep();
	return(0);
}

void cmt2219a_Init(void)
{
	cmt2219a_vSoftReset();
	system_Delay_ms(100);
	cmt2219a_vInit(CfgTbl);
	cmt2219a_vGpioFuncCfg(GPIO1_INT1|GPIO2_DCLK|GPIO3_CLK|GPIO4_Dout);
	cmt2219a_vIntSourcCfg((FIFO_WBYTE+OFFSET), 0);
	cmt2219a_vEnableIntSource(0xFF);
	cmt2219a_vGoRx();
}

uint8_t Task_CMT2219A(void)
{
	uint8_t qlength, result_comp;
	uint8_t spibuf[12] = {0,};
	static uint8_t past_packet[10] = {0,};
	uint8_t remote_on = 0;
	uint8_t i;
	
	cmt2219a_bGetStatus();
	
	qlength = cmt2219a_bGetMessage(spibuf);
	
	if (qlength!=0)
	{
		remote_on = 1;
		
		// New packet ?
		for(i=0; i<10; i++)
		{
			if (past_packet[i] == spibuf[i])
				result_comp = 0;
			else
			{
				result_comp = 1;
				break;
			}
		}
		
		if(result_comp && spibuf[9] == 0x55 && ((spibuf[4]+spibuf[5])==0) || (spibuf[4]==ENT_CODE))
		{
			for(i=0; i<10; i++)
				past_packet[i] = spibuf[i];
			
			remote_r = spibuf[0];
			remote_g = spibuf[1];
			remote_b = spibuf[2];
			remote_mode = spibuf[3];
		}
	}
	
	qlength = 0;
	for(i=0; i<12; i++)
		spibuf[i] = 0;
	
	return remote_on;
}
