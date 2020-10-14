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
 *	������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *	ALIENTEK Pandora STM32L475 IOT������
 *	�ⲿ�ж���������
 *	����ԭ��@ALIENTEK
 *	������̳:www.openedv.com
 *	��������:2018/10/27
 *	�汾��V1.0
 *	��Ȩ���У�����ؾ���
 *	Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	��ʼ�汾
 *	******************************************************************************/
u8 ov_sta;	//֡�жϱ��
/**
 * @brief	�ⲿ�жϳ�ʼ������
 *
 * @param   void
 *
 * @return  void
 */


void EXTI_7725_Init(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	 __HAL_RCC_GPIOB_CLK_ENABLE();               //����GPIOBʱ��
	
		GPIO_Initure.Pin = GPIO_PIN_11;	//PB11
    GPIO_Initure.Mode = GPIO_MODE_IT_RISING;   //�����ش���
    GPIO_Initure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
	   //�ж���11
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 3); //��ռ���ȼ�Ϊ3�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //ʹ���ж���11
}




/**
 * @brief	EXTI15_10�жϷ�����
 *
 * @param   void
 *
 * @return  void
 */
void EXTI15_10_IRQHandler(void)
{	
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);//�����жϴ����ú���
}



/**
 * @brief	�жϷ����������Ҫ��������,��HAL�������е��ⲿ�жϷ�����������ô˺���
 *
 * @param   GPIO_Pin	�ж����ź�
 *
 * @return  void
 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//��û���õ����
{
    delay_ms(100);      //����
	if(GPIO_Pin==GPIO_PIN_11)
	{
						if(ov_sta==0)
						{
							OV7725_WRST(0);	//��λдָ��		 
							OV7725_WRST(1);	
							OV7725_WREN(1);	//����д��FIFO 	 
							ov_sta++;		//֡�жϼ�1 
						}else OV7725_WREN(0);//��ֹд��FIFO
	}
}



/*���벶�񴥷�֡�ж�*/


TIM_HandleTypeDef TIM2_Handler;         //��ʱ��2���

void TIM2_CH4_Cap_Init(u32 arr,u16 psc)
{
	
	
	TIM_IC_InitTypeDef TIM2_CH4Config; 
	
	
	TIM2_Handler.Instance = TIM2;   													//ͨ�ö�ʱ��2
	TIM2_Handler.Init.Prescaler = psc;												//��Ƶϵ��
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;				//���ϼ�����
	TIM2_Handler.Init.Period = arr;														//�Զ�װ��ֵ
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//ʱ�ӷ�Ƶ����
	HAL_TIM_IC_Init(&TIM2_Handler);														//��ʼ�����벶��ʱ������
	
	TIM2_CH4Config.ICPolarity = TIM_ICPOLARITY_RISING;				//�����ز���
	TIM2_CH4Config.ICSelection = TIM_ICSELECTION_DIRECTTI;		//ӳ�䵽TI1��
	TIM2_CH4Config.ICPrescaler = TIM_ICPSC_DIV1;							//���������Ƶ������Ƶ
	TIM2_CH4Config.ICFilter = 0;															//���������˲��������˲�
	HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CH4Config,TIM_CHANNEL_4);//����TIM2ͨ��4
	
	HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_4);   //����TIM2�Ĳ���ͨ��4�����ҿ��������ж�
}

//��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��5���
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_11;            //PB11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM2;   //PB11����ΪTIM2ͨ��4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����TIM2�ж�ͨ��  
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2_Handler);//��ʱ�����ô�����
}

//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��  ֡�жϲ���
{					
		if(ov_sta==0)
		{
			OV7725_WRST(0);	//��λдָ��		 
			OV7725_WRST(1);	
			OV7725_WREN(1);	//����д��FIFO 	 
			ov_sta++;		//֡�жϼ�1 
		}else OV7725_WREN(0);//��ֹд��FIFO
}
