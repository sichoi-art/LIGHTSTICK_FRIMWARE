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
 * file       HopeDuino_CMT2219A.h
 * brief      show how to use CMT2219A
 * hardware   HopeDuino with RFM219S
 *            
 *
 * version    1.0
 * date       Feb 20 2016
 * author     QY Ruan
 */
	
	//*********************************************************
	//Reg mapping
	//*********************************************************
	#define	INT_EN				0x3F
		#define RX_DONE_EN		0x01
		#define CRC_PS_EN			0x02
		#define	NODE_PS_EN		0x04
		#define	SYNC_PS_EN		0x08
		#define PREAM_PS_EN		0x10
		#define	RSSI_VLD_EN		0x20
		#define	RX_TMO_EN			0x40
		#define	SL_TMO_EN			0x80

	#define	IO_SEL				0x40
		#define GPIO1_POR			0x00
		#define GPIO1_INT1		0x01
		#define	GPIO1_INT2		0x02
		#define	GPIO1_Dout		0x03
		
		#define	GPIO2_INT1		0x00
		#define	GPIO2_INT2		0x04
		#define	GPIO2_DCLK		0x08
		#define	GPIO2_0				0x0C	
		
		#define	GPIO3_CLK			0x00
		#define GPIO3_INT1		0x10
		#define	GPIO3_INT2		0x20
		#define	GPIO3_DCLK		0x30
		
		#define	GPIO4_Dout		0x00
		#define	GPIO4_INT1		0x40
		#define	GPIO4_INT2		0x80
		#define	GPIO4_DCLK		0xC0


	#define	INTCTL_A		0x41
		#define	RSSI_VLD		0x01
			
		#define	PREAM_PS		0x02
		#define	SYNC_PS			0x03
		
		#define	NODE_PS			0x04
		#define	CRC_PS			0x05
		#define	RX_DONE			0x06
			
		#define	SL_TMO			0x04	//+OFFSET
		#define	RX_TMO			0x05
		#define	FIFO_NMTY		0x06
		#define	FIFO_TH			0x07
		#define	FIFO_FULL		0x08
		#define	FIFO_WBYTE	0x09
		#define FIFO_OVF		0x0A
		#define	RSSI_INDI		0x0B
		
		#define	OFFSET			0x03

	#define	INTCTL_B			0x42
		#define	RX_DONE_CLR		0x01
		#define	CRC_PS_CLR		0x02
		#define	NODE_PS_CLR		0x04
		#define	SYNC_PS_CLR		0x08
		#define	PREAM_PS_CLR	0x10
		#define	RSSI_VLD_CLR	0x20
		#define RX_TMO_CLR		0x40
		#define	SL_TMO_CLR		0x80

	#define	INTCTL_C			0x43
		#define	RX_DONE_FLG		0x01
		#define	CRC_PS_FLG		0x02
		#define	NODE_PS_FLG		0x04
		#define	SYNC_PS_FLG		0x08
		#define	PREAM_PS_FLG	0x10
		#define	RSSI_VLD_FLG	0x20
		#define	RX_TMO_FLG		0x40			
		#define	SL_TMO_FLG		0x80

	#define	INTCTL_D			0x44
		#define	FIFO_WR_METH	0x01
		#define	FIFO_WR_EN		0x02
		#define	FIFO_CLR			0x04
		#define	FIFO_OVF_FLG	0x08
		#define	FIFO_TH_FLG		0x10
		#define	FIFO_NMTY_FLG	0x20
		#define	FIFO_FULL_FLG	0x40
		#define	PKT_LEN_ERR_FLG	0x80

	#define	RSSI_ADDR			0x45

	#define	FUNC_EN				0x46
		#define	EEPROM_LOCK		0x01
		#define	EEPROM_PWRON 	0x02
		#define FAST_SPI_EN		0x04
		#define	SA_TEST_EN		0x08

	#define	OP_MODE				0x47
		#define OP_EEPROM			0x01
		#define	OP_STANDBY		0x02	
		#define	OP_FS					0x04
		#define	OP_RX					0x08
		#define	OP_SLEEP			0x10

		#define	RD_IDLE				0x00
		#define	RD_SLEEP			0x20
		#define	RD_STBY				0x40
		#define	RD_FS					0x60
		#define	RD_RX					0x80
		#define	RD_EEPROM			0xA0
													
		#define OP_MASK				0xE0

	#define	SOFT_RST 			0x4F//FF

	#define	EE_DAT_LOW		0x50
	#define EE_DAT_HIGH		0x51
	#define	EE_ADD				0x52

	#define	EE_CTL				0x53
		#define	EE_ERASE			0x01
		#define	EE_PROG				0x02
		#define	EE_READ				0x04
	


void vInit(uint8_t cfg[]);	
void vGoRx(void);
void vGoSleep(void);
void vGoStandby(void);
void vSoftReset(void);
void vClearFIFO(void);
uint8_t bReadStatus(void);
uint8_t bReadRssi(void);
uint8_t bReadIngFlag(void);
void vClearIntFlag(void);
void vGpioFuncCfg(uint8_t io_cfg);
void vIntSourcCfg(uint8_t int_1, uint8_t int_2);
void vEnableIntSource(uint8_t en_int);
uint8_t bGetMessage(uint8_t msg[]);
					
extern uint8_t FreqSel;		//Ƶ��ѡ��

//basic func
 uint8_t RFM219A_EntryRx(void);
 void RFM219A_Sleep(void);
 void RFM219A_Standby(void);
 void RFM219A_RFSoftReset(void);
 void RFM219A_ClearIRQ(void);
 void RFM219A_ClearFIFO(void);
 void RFM219A_ReadStatus(void);
 void RFM219A_Config(void);
 void SPI3Write(uint16_t WrPara);
 uint8_t SPI3Read(uint8_t adr);
 void SPI3Cmd8bit(uint8_t WrPara);
 uint8_t SPI3Read8bit(void);

//added func
 void rf_init(void);
 void check_config(void);
 void set_default_register(void);
uint8_t change_frequency(uint8_t SelectFreq);
 void compare_RFPDK_config(void);
uint8_t cmt2219a_bReadStatus(void);
uint8_t cmt2219a_bGetMessage(uint8_t msg[]);
void cmt2219a_Init(void);
void cmt2219a_vGoSleep(void);
void cmt2219a_vGoStandby(void);
void cmt2219a_vGoRx(void);
uint8_t spi3wire_readbyte(uint8_t address);
void spi3wire_writebyte(uint8_t address, uint8_t datao);
void cmt2219a_vSoftReset(void);

