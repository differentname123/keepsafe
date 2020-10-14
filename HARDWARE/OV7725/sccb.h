#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序参考自网友guanfu_wang代码。
//ALIENTEK战舰STM32开发板V3
//SCCB 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/18
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
  
#define SCCB_SDA_IN()   {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<(7*2);}	//PC7输入模式
#define SCCB_SDA_OUT()	{GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<(7*2);} 	//PC1输出模式

//IO操作函数	 
#define SCCB_SCL(n)   		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET))//SCL
#define SCCB_SDA(n)    		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET))//SDA

#define SCCB_READ_SDA    HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)//输入SDA  
#define SCCB_ID   			0X42  			//OV7670的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













