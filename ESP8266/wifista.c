#include "wifiap.h"
#include "lcd.h"
#include "malloc.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "stdlib.h"
#define SRAMIN 0

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="asdfgh6";			//·����SSID��
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="12345678"; 	//��������
const u8* portnum="12345";	
const u8* ipadd="192.168.043.180";	
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI STA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void sencmd(char *data)
{
	int i=0;
	while(data[i]!='\0')
		i++;
	//data[i++]='\r';
	//data[i++]='\n';
	//int len=i;
	//HAL_UART_Transmit(&UART1_Handler, (uint8_t*)data, len, 1000);	//���ͽ��յ�������

	printf("%s\r\n",data);

}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
void uartpa2pa3(uint8_t *data)
{
	int i;
	while(data[i]!='\0'&&data[i]!='\r'&&data[i]!='\n')
	{
		i++;
	}
	data[i++]='\r';
	
	data[i++]='\n';
	HAL_UART_Transmit(&UART1_Handler,data,i,1000);
	 while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);		//�ȴ����ͽ���

            printf("\r\n\r\n");//���뻻��
            USART_RX_STA = 0;

}
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	int i;
	u8 res=0; 
	USART_RX_STA=0;
	//uartpa2pa3(cmd);	//��������
	printf("%s\r\n",cmd);
	
	LCD_ShowString(0, 105, 240, 16, 16, cmd);
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			
			delay_ms(10);
			if(USART_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				//if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
void sendphoto(u16 data)
{
	int i=0;
	u16 x=data;
	char *cmd;
	char temp[5];
	temp[0]=data/10000+'0';
	temp[1]=data%10000/1000+'0';
	temp[2]=data%1000/100+'0';
	temp[3]=data%100/10+'0';
	temp[4]=data%10+'0';

	
	sprintf((char*)cmd,"AT+CIPSEND=%d\r\n",5);
	//cmd="AT+CIPSEND=4";//��ʼ͸�� 
	sencmd(cmd);
	//LCD_ShowString(0,80, 240, 16, 16, cmd);
	sencmd(temp);
	//LCD_ShowString(0,60, 240, 16, 16, data);
	//delay_ms(1000);
	
}
void senddata(char *data)
{
	int i=0;
	char *cmd;
	cmd=mymalloc(SRAMIN,32);
	while(data[i]!='\0'&&data[i]!='\r'&&data[i]!='\n')
	{
		i++;
	}
	
	
	sprintf((char*)cmd,"AT+CIPSEND=%d\r\n",i);
	//cmd="AT+CIPSEND=4";//��ʼ͸�� 
	sencmd(cmd);
	//LCD_ShowString(0,80, 240, 16, 16, cmd);
	sencmd(data);
	//LCD_ShowString(0,60, 240, 16, 16, data);
	//delay_ms(1000);
	
}


//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������

u8 netpro=0;	//����ģʽ
void lianjie()
{
	USART_RX_BUF[0]='b';
	const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
	char *cmd;
	cmd=mymalloc(SRAMIN,100);
	int t=1;
	u8 *p;
	p=mymalloc(SRAMIN,32);	
	
	cmd="AT+UART=115200,8,1,0,0\r\n";
	//sprintf((char*)cmd,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	sencmd(cmd);
	LCD_ShowString(0,0, 240, 16, 16, cmd);
	delay_ms(1000);
	
	
	
	cmd="AT+CWMODE=1\r\n";//sta mode
sencmd(cmd);
	LCD_ShowString(0,200, 240, 16, 16, cmd);
	delay_ms(1000);
	
	cmd="AT+RST\r\n";//restart
sencmd(cmd);
	LCD_ShowString(0,180, 240, 16, 16, cmd);
	delay_ms(1000);
	
	cmd="AT+CWJAP=\"asdfgh67\",\"12345678\"\r\n";//ap information
sencmd(cmd);
	LCD_ShowString(0,160, 240, 16, 16, cmd);
	delay_ms(5000);
	
	cmd="AT+CIPMUX=0\r\n";
sencmd(cmd);
	LCD_ShowString(0,140, 240, 16, 16, cmd);
	delay_ms(5000);
	
	//while(1)
	{
		cmd="AT+CIPSTART=\"TCP\",\"192.168.43.98\",12345\r\n";
sencmd(cmd);
		LCD_ShowString(0,120, 240, 16, 16, cmd);
		delay_ms(5000);
	
	}
	
		cmd="AT+CIPMODE=0\r\n";
		sencmd(cmd);
		LCD_ShowString(0,100, 240, 16, 16, cmd);
		delay_ms(1000);
		
}


void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//??��?WAN IP��??������㨹
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

void jieshou()
{
	u8 *p;
	p=mymalloc(SRAMIN,32);
	int rlen;
	int key;
	int times;
	int len;
	int i=0;
	lianjie();
			//	while(1)
			{
				
				key=KEY_Scan(0);
				if(key==KEY0_PRES)	//KEY0 �������� 
					{
						
						i++;
						//sprintf((char*)p,"��ΰţ��%d��",i);//�������߲���:ssid,����
						//senddata(p);
					}
        if(USART_RX_STA & 0x8000)
        {
						i++;
					//	sprintf((char*)p,"niubi%s%d",USART_RX_BUF,i);//�������߲���:ssid,����
					//	sprintf((char*)p,"1231210812345678");
						//senddata(p);
					//	LCD_ShowString(0,100, 240, 16, 16, p);
//            len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
//            printf("\r\n�����͵���ϢΪ:\r\n");
//            HAL_UART_Transmit(&UART1_Handler, (uint8_t*)USART_RX_BUF, len, 1000);	//���ͽ��յ�������

//            while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);		//�ȴ����ͽ���

//            printf("\r\n\r\n");//���뻻��
           USART_RX_STA = 0;
        }
        else
        {
            times++;

            if(times % 5000 == 0)
            {
                //printf("\r\nALIENTEK �˶��� STM32L475 IOT������ ����ʵ��\r\n");
               // printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
            }

           // if(times % 200 == 0)printf("����������,�Իس�������\r\n");
            if(times % 30 == 0)LED_B_TogglePin; //��˸LED,��ʾϵͳ��������.

            delay_ms(10);
        }
			}
			

}

u8 atk_8266_wifista_test(void)
{
	//u8 netpro=0;	//����ģʽ
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p,*cmd;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	int i=0;
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	LCD_ShowString(0, 0, 200, 16, 16, &p[9]);
	atk_8266_send_cmd(p,"WIFI GOT IP",30);

	netpro=0x01;

		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
		{
			//LCD_Clear(WHITE);
			POINT_COLOR=RED;
			int i;
			for(i=0;i<16;i++)
				ipbuf[i]=ipadd[i];
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","192.168.43.154",portnum);    //����Ŀ��TCP������
			
					cmd="AT+CIPSTART=\"TCP\",\"192.168.43.154\",12345";
		printf("%s\r\n",cmd);
		LCD_ShowString(0,120, 240, 16, 16, cmd);
		delay_ms(5000);
		
			
			LCD_ShowString(30, 125, 200, 16, 16, ipbuf);
			LCD_ShowString(0, 145, 200, 16, 16, &p[10]);

			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��		
		
		}


				atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
				sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
				printf("%s",p);
			USART_RX_STA=0;
			while(1)
			{
				int i=0;
				key=KEY_Scan(0);
				if(key==KEY0_PRES)	//KEY0 �������� 
				{
					if((netpro==1))   //TCP Client
					{
						i++;
					sprintf((char*)p,"��ΰţ��%d��",i);//�������߲���:ssid,����
					senddata(p);
					}
				}
				if(USART_RX_STA&0X8000)		//���յ�һ��������
				{ 
					rlen=USART_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
					USART_RX_BUF[rlen]=0;		//��ӽ����� 
					printf("shouu%s",USART_RX_BUF);	//���͵�����   
					sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 
					USART_RX_STA=0;
					if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
					else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
				}  
			}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return res;		
} 




























