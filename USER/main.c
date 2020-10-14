#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "ov7725.h"
#include "sccb.h"
#include "exti.h"
#include "key.h"
#include "timer.h"
#include "wifiap.h"
#include "malloc.h"
#include "rc522.h"
#include "beep.h"
#include "aht10.h"
#include "pwm.h"
/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	����ԭ�� Pandora STM32L475 IoT������	ʵ��10
 *	TFTLCD��ʾʵ��		HAL��汾
 *	����֧�֣�www.openedv.com
 *	�Ա����̣�http://openedv.taobao.com
 *	��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 *	������������ӿƼ����޹�˾
 *	���ߣ�����ԭ�� @ALIENTEK
 *	******************************************************************************/
#define  OV7725 1
#define  OV7725_WINDOW_WIDTH		240 // <=240
#define  OV7725_WINDOW_HEIGHT		240 // <=240

u8 LCD_7725DATA(void)
{	
	u8 data=0;
	if(1)
	{
		data |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)<<7;		//D7
		data |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)<<6;		//D6
		data |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)<<5;		//D5
		data |= HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)<<4;		//D4
		
		data |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)<<3;		//D3
		data |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)<<2;		//D2
		data |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14)<<1;		//D1
		data |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);			//D0
	}
	return data;
}

extern u8 ov_frame;	//��timer.c���涨�� 
extern u8 ov_sta;	//��exit.c�� �涨��
#define LCD_7725_SIZE	(240*240*2)
#define LCD_7725Buf_Size 480							//һ��һ�д�������ݳ��ȴ�С
static u8 lcd_7725buf[LCD_7725Buf_Size];	//һ��һ�д�������黺��


void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	int key;
	int count=0;
	int k;
	char *cmd;
	cmd=mymalloc(0,32);
	key=KEY_Scan(1);
//			if(key==WKUP_PRES)
//				{
//		cmd="AT+CIPMODE=1";
//		printf("%s\r\n",cmd);
//				}
	if(ov_sta)//��֡�жϸ���
	{
		
		LCD_Address_Set(0, 0, OV7725_WINDOW_WIDTH, OV7725_WINDOW_HEIGHT);									//����������ʾ����
		OV7725_RRST(0);				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST(1);				//��λ��ָ����� 
		OV7725_RCK_H; 
		key=KEY_Scan(0);
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{			
				OV7725_RCK_L;
				color=LCD_7725DATA()&0XFF;	//������ *(TLS_REG *)reg
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=LCD_7725DATA()&0XFF;	//������
				OV7725_RCK_H; 
				//printf("%d\r\n",color);
				if(key==KEY0_PRES)
					sendphoto(color);
				lcd_7725buf[j * 2] = color >> 8;  //��2����3���߰�λ  
				lcd_7725buf[j * 2 + 1] = color;	  //��2����3���Ͱ�λ	
				
			}
		//	sendphoto(lcd_7725buf);
			
//			if(key==WKUP_PRES)
//				{
//					
//					for(k=0;k<480;k++)
//					{
			
//						count++;
//						//sendphoto(lcd_7725buf[k]);
//						printf("%#x",lcd_7725buf[k]);
//						//printf("%d\t\n",count);
//					}
//					
//					//sendphoto(lcd_7725buf);
//				}
					
			
			LCD_DC(1); 											//��2��
			LCD_SPI_Send(lcd_7725buf, LCD_7725Buf_Size);  //LCD�ײ�SPI�������ݺ�����2��
		}
		key=0;
		ov_sta=0;					//����֡�жϱ��
		ov_frame++;
	} 
}
int main(void)
{
	
		float temperature, humidity;
   	u8 lightmode=0,effect=0;
		u8 key;
		u32 i;
  	s8 saturation=0,brightness=0,contrast=0;
	
	
   // HAL_Init();
   // SystemClock_Config();		//��ʼ��ϵͳʱ��Ϊ80M
   // delay_init(80); 			//��ʼ����ʱ����    80Mϵͳʱ��
   // uart_init(115200);			//��ʼ�����ڣ�������Ϊ115200

   // LED_Init();					//��ʼ��LED
   // KEY_Init();					//��ʼ������
   // LCD_Init();					//��ʼ��LCD

	 // printf("��ʼ�����\n");
	

    HAL_Init();
    SystemClock_Config();	//��ʼ��ϵͳʱ��Ϊ80M
    delay_init(80); 		//��ʼ����ʱ����    80Mϵͳʱ��
    uart_init(115200);		//��ʼ�����ڣ�������Ϊ115200
    LED_Init();				//��ʼ��LED
    LCD_Init();				//��ʼ��LCD	
		KEY_Init();
		BEEP_Init();	
	  RC522_Init();
		jieshou();
		LCD_Clear(WHITE);
		LCD_ShowString(30,100,240,16,16,"WIFI_INIT_SUCCESS");
		delay_ms(1000);
	//	senddata("qweq");
		//sendphoto(6565);
	 
		POINT_COLOR = RED;
		key=WKUP_PRES;
			while(AHT10_Init())			//��ʼ��AHT10
    {
				LCD_Clear(WHITE);
        LCD_ShowString(30, 170, 200, 16, 16, "AHT10 Error");
        delay_ms(200);
        LCD_Fill(30, 170, 239, 170 + 16, WHITE);
        delay_ms(200);
    }
		LCD_Clear(WHITE);
while(1)
{
	//LCD_Clear(WHITE);
	LCD_ShowString(0,100,240,24,24,"choose");


	
	//if(key==KEY0_PRES)
	{

	 while(1)
    {
//		HAL_Init();
//    SystemClock_Config();	//��ʼ��ϵͳʱ��Ϊ80M
//    delay_init(80); 		//��ʼ����ʱ����    80Mϵͳʱ��
//    uart_init(115200);		//��ʼ�����ڣ�������Ϊ115200
//    LED_Init();				//��ʼ��LED
//    LCD_Init();				//��ʼ��LCD	
//		KEY_Init();
//		BEEP_Init();	
//	  RC522_Init();
			//RC522_Handel();
			if(OV7725_Init()==0)
			{
				LCD_Clear(WHITE);
				LCD_ShowString(0,120,240,24,24,"OV7725 Init OK       ");
				while(1)
				{
					
					//if(key==KEY0_PRES)
				//	{
						//OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���
						break;
				//	}
//					else if(key==KEY1_PRES)
//					{
					//	OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGAģʽ���
					//	break;
//					}
//					i++;
//					if(i==100)LCD_ShowString(0,150,240,24,24,"KEY0:QVGA KEY1:VGA"); //��˸��ʾ��ʾ��Ϣ
//					if(i==200)
//					{	
//						LCD_Fill(0,150,240,240,WHITE);
//						i=0; 
//					}
					delay_ms(5);
				}		
				OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���				
				OV7725_Light_Mode(lightmode);
				OV7725_Color_Saturation(saturation);
				OV7725_Brightness(brightness);
				OV7725_Contrast(contrast);
				OV7725_Special_Effects(effect);
				OV7725_CS(0);
				break;
			}
			else
			{
				LCD_ShowString(0,200,240,16,16,"OV7725_OV7670 Error!!");
				delay_ms(200);
				LCD_Fill(0,200,200,240,WHITE);
				delay_ms(200);
			}
    }
		TIM3_Init(10000-1, 8000 - 1);       	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ80M����Ƶϵ��Ϊ8000-1��
																					//���Զ�ʱ��3��Ƶ��Ϊ80M/8000=10K���Զ���װ��Ϊ10000-1����ô��ʱ�����ھ���1s
		TIM2_CH4_Cap_Init(65535 - 1, 40 - 1);  //TIM2ʱ��Ƶ�� 80M/40=2M ʹ�����벶��������֡�ж�
		LCD_Clear(WHITE);
		
		while(1)
		{
            //TIM_SetTIM2Compare1(500);
            //TIM_SetTIM2Compare2(0);
				//LCD_Clear(WHITE);
				LCD_ShowString(0,20,240,24,40,"choose");
				LCD_ShowString(0,50,240,24,24,"rfid:wk_up");
				LCD_ShowString(0,80,240,24,24,"wendu:key1");
				LCD_ShowString(0,110,240,24,24,"photo:key0");
				key=KEY_Scan(1);
				if(key==WKUP_PRES)
				{
						LCD_Clear(WHITE);
						LCD_ShowString(0,130,240,24,24,"rfid");
						RC522_Init();
			//		HAL_Init();
			//    SystemClock_Config();	//��ʼ��ϵͳʱ��Ϊ80M
			//    delay_init(80); 		//��ʼ����ʱ����    80Mϵͳʱ��
			//    uart_init(115200);		//��ʼ�����ڣ�������Ϊ115200
			//    LED_Init();				//��ʼ��LED
			//    LCD_Init();				//��ʼ��LCD	
			//		KEY_Init();
			//		BEEP_Init();	
			//	  RC522_Init();
					while(1)
					{
						
						key=KEY_Scan(1);
						RC522_Handel();
						if(key==KEY2_PRES)
							break;
					}
					LCD_Clear(WHITE);
				
				}
				if(key==KEY1_PRES)
				{
					LCD_Clear(WHITE);
					LCD_ShowString(0,100,240,24,24,"wendu");
					POINT_COLOR = BLUE; //��������Ϊ��ɫ
					LCD_ShowString(30, 190, 200, 16, 16, "Temp:   . C");
					LCD_ShowString(30, 210, 200, 16, 16, "Humi:   . %RH");
					while(1)
					{
							temperature = AHT10_Read_Temperature();
						humidity = AHT10_Read_Humidity();
							if(temperature < 0)
							{
									LCD_ShowChar(30 + 40, 190, '-', 16);	//��ʾ����
									temperature = -temperature;				//תΪ����
							}
							else
									LCD_ShowChar(30 + 40, 190, ' ', 16);	//ȥ������

							LCD_ShowNum(30 + 48, 190, temperature, 2, 16);					//��ʾ�¶�����
							LCD_ShowNum(30 + 72, 190, (u32)(temperature * 10) % 10, 1, 16);	//��ʾ�¶�С��

							LCD_ShowNum(30 + 48, 210, humidity, 2, 16);						//��ʾʪ������
							LCD_ShowNum(30 + 72, 210, (u32)(humidity * 10) % 10, 1, 16);	//��ʾʪ��С��
							key=KEY_Scan(1);
							if(key==KEY2_PRES)
								break;
					
					}
					LCD_Clear(WHITE);
			
					
				}

			if(key==KEY0_PRES)
			{
			//	HAL_Init();
			//	SystemClock_Config();	//��ʼ��ϵͳʱ��Ϊ80M
				//delay_init(80); 		//��ʼ����ʱ����    80Mϵͳʱ��
			//	uart_init(115200);		//��ʼ�����ڣ�������Ϊ115200
				LED_Init();				//��ʼ��LED
				LCD_Init();				//��ʼ��LCD	
				KEY_Init();
				BEEP_Init();
				//RC522_Init();
				
				//while(0==OV7725_Init());
				OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���				
				OV7725_Light_Mode(lightmode);
				OV7725_Color_Saturation(saturation);
				OV7725_Brightness(brightness);
				OV7725_Contrast(contrast);
				OV7725_Special_Effects(effect);
				OV7725_CS(0);
				TIM3_Init(10000-1, 8000 - 1);       	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ80M����Ƶϵ��Ϊ8000-1��
																					//���Զ�ʱ��3��Ƶ��Ϊ80M/8000=10K���Զ���װ��Ϊ10000-1����ô��ʱ�����ھ���1s
				TIM2_CH4_Cap_Init(65535 - 1, 40 - 1);  //TIM2ʱ��Ƶ�� 80M/40=2M ʹ�����벶��������֡�ж�
				LCD_Clear(BLACK);
				while(1)
				{
				
					OV7725_camera_refresh();//������ʾ
					i++;
					if(i>=15)//DS0��˸.
					{
						i=0;
						LED_R_TogglePin; //��Ʒ�ת
					}	
					key=KEY_Scan(1);
					if(key==KEY2_PRES)
						break;
				}
				LCD_Clear(WHITE);
			
			}


		}		

	}

		
		
		

}
	
		
}


