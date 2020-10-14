#ifndef _OV7725_H
#define _OV7725_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////
//ALIENTEKս��STM32������
//OV7725 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/11/1
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////


#define OV7725_MID				0X7FA2    
#define OV7725_PID				0X7721

#define OV7725_VSYNC(n)  	 (n?HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET):HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET))			//ͬ���źż��IO
#define OV7725_WRST(n)		 (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET))	//дָ�븴λ
#define OV7725_WREN(n)		 (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET))		//д��FIFOʹ��
#define OV7725_RCK_H			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)//���ö�����ʱ�Ӹߵ�ƽ
#define OV7725_RCK_L	 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)	//���ö�����ʱ�ӵ͵�ƽ
#define OV7725_RRST(n)	 	(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET))  		//��ָ�븴λ
#define OV7725_CS(n)		 	(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))  		//Ƭѡ�ź�(OE)
								  					 
#define OV7725_DATA   	GPIO_ReadInputData(GPIOC,0x00FF) 	//��������˿�



	    				 
u8   OV7725_Init(void);		  	   		 
void OV7725_Light_Mode(u8 mode);
void OV7725_Color_Saturation(s8 sat);
void OV7725_Brightness(s8 bright);
void OV7725_Contrast(s8 contrast);
void OV7725_Special_Effects(u8 eft);
void OV7725_Window_Set(u16 width,u16 height,u8 mode);
#endif





















