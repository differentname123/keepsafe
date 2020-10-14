#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "ov7725.h"
/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *	ALIENTEK Pandora STM32L475 IOT开发板
 *	外部中断驱动代码
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2018/10/27
 *	版本：V1.0
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	初始版本
 *	******************************************************************************/
u8 ov_sta;	//帧中断标记
/**
 * @brief	外部中断初始化函数
 *
 * @param   void
 *
 * @return  void
 */


void EXTI_7725_Init(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	 __HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
	
		GPIO_Initure.Pin = GPIO_PIN_11;	//PB11
    GPIO_Initure.Mode = GPIO_MODE_IT_RISING;   //上升沿触发
    GPIO_Initure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
	   //中断线11
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 3); //抢占优先级为3，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //使能中断线11
}




/**
 * @brief	EXTI15_10中断服务函数
 *
 * @param   void
 *
 * @return  void
 */
void EXTI15_10_IRQHandler(void)
{	
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);//调用中断处理公用函数
}



/**
 * @brief	中断服务程序中需要做的事情,在HAL库中所有的外部中断服务函数都会调用此函数
 *
 * @param   GPIO_Pin	中断引脚号
 *
 * @return  void
 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//并没有用到这个
{
    delay_ms(100);      //消抖
	if(GPIO_Pin==GPIO_PIN_11)
	{
						if(ov_sta==0)
						{
							OV7725_WRST(0);	//复位写指针		 
							OV7725_WRST(1);	
							OV7725_WREN(1);	//允许写入FIFO 	 
							ov_sta++;		//帧中断加1 
						}else OV7725_WREN(0);//禁止写入FIFO
	}
}



/*输入捕获触发帧中断*/


TIM_HandleTypeDef TIM2_Handler;         //定时器2句柄

void TIM2_CH4_Cap_Init(u32 arr,u16 psc)
{
	
	
	TIM_IC_InitTypeDef TIM2_CH4Config; 
	
	
	TIM2_Handler.Instance = TIM2;   													//通用定时器2
	TIM2_Handler.Init.Prescaler = psc;												//分频系数
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;				//向上计数器
	TIM2_Handler.Init.Period = arr;														//自动装载值
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//时钟分频银子
	HAL_TIM_IC_Init(&TIM2_Handler);														//初始化输入捕获时基参数
	
	TIM2_CH4Config.ICPolarity = TIM_ICPOLARITY_RISING;				//上升沿捕获
	TIM2_CH4Config.ICSelection = TIM_ICSELECTION_DIRECTTI;		//映射到TI1上
	TIM2_CH4Config.ICPrescaler = TIM_ICPSC_DIV1;							//配置输入分频，不分频
	TIM2_CH4Config.ICFilter = 0;															//配置输入滤波器，不滤波
	HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CH4Config,TIM_CHANNEL_4);//配置TIM2通道4
	
	HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_4);   //开启TIM2的捕获通道4，并且开启捕获中断
}

//定时器5底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器5句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_11;            //PB11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM2;   //PB11复用为TIM2通道4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启TIM2中断通道  
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2_Handler);//定时器共用处理函数
}

//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行  帧中断操作
{					
		if(ov_sta==0)
		{
			OV7725_WRST(0);	//复位写指针		 
			OV7725_WRST(1);	
			OV7725_WREN(1);	//允许写入FIFO 	 
			ov_sta++;		//帧中断加1 
		}else OV7725_WREN(0);//禁止写入FIFO
}
