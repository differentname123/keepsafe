#include "wifiap.h"
#include "lcd.h"
#include "malloc.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "stdlib.h"
#define SRAMIN 0

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="asdfgh6";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="12345678"; 	//连接密码
const u8* portnum="12345";	
const u8* ipadd="192.168.043.180";	
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI STA驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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
	//HAL_UART_Transmit(&UART1_Handler, (uint8_t*)data, len, 1000);	//发送接收到的数据

	printf("%s\r\n",data);

}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
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
	 while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);		//等待发送结束

            printf("\r\n\r\n");//插入换行
            USART_RX_STA = 0;

}
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	int i;
	u8 res=0; 
	USART_RX_STA=0;
	//uartpa2pa3(cmd);	//发送命令
	printf("%s\r\n",cmd);
	
	LCD_ShowString(0, 105, 240, 16, 16, cmd);
	
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			
			delay_ms(10);
			if(USART_RX_STA&0X8000)//接收到期待的应答结果
			{
				//if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
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
	//cmd="AT+CIPSEND=4";//开始透传 
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
	//cmd="AT+CIPSEND=4";//开始透传 
	sencmd(cmd);
	//LCD_ShowString(0,80, 240, 16, 16, cmd);
	sencmd(data);
	//LCD_ShowString(0,60, 240, 16, 16, data);
	//delay_ms(1000);
	
}


//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码

u8 netpro=0;	//网络模式
void lianjie()
{
	USART_RX_BUF[0]='b';
	const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
	char *cmd;
	cmd=mymalloc(SRAMIN,100);
	int t=1;
	u8 *p;
	p=mymalloc(SRAMIN,32);	
	
	cmd="AT+UART=115200,8,1,0,0\r\n";
	//sprintf((char*)cmd,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
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
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//??è?WAN IPμ??·ê§°ü
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
				if(key==KEY0_PRES)	//KEY0 发送数据 
					{
						
						i++;
						//sprintf((char*)p,"阿伟牛逼%d次",i);//设置无线参数:ssid,密码
						//senddata(p);
					}
        if(USART_RX_STA & 0x8000)
        {
						i++;
					//	sprintf((char*)p,"niubi%s%d",USART_RX_BUF,i);//设置无线参数:ssid,密码
					//	sprintf((char*)p,"1231210812345678");
						//senddata(p);
					//	LCD_ShowString(0,100, 240, 16, 16, p);
//            len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
//            printf("\r\n您发送的消息为:\r\n");
//            HAL_UART_Transmit(&UART1_Handler, (uint8_t*)USART_RX_BUF, len, 1000);	//发送接收到的数据

//            while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);		//等待发送结束

//            printf("\r\n\r\n");//插入换行
           USART_RX_STA = 0;
        }
        else
        {
            times++;

            if(times % 5000 == 0)
            {
                //printf("\r\nALIENTEK 潘多拉 STM32L475 IOT开发板 串口实验\r\n");
               // printf("正点原子@ALIENTEK\r\n\r\n\r\n");
            }

           // if(times % 200 == 0)printf("请输入数据,以回车键结束\r\n");
            if(times % 30 == 0)LED_B_TogglePin; //闪烁LED,提示系统正在运行.

            delay_ms(10);
        }
			}
			

}

u8 atk_8266_wifista_test(void)
{
	//u8 netpro=0;	//网络模式
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p,*cmd;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	int i=0;
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	LCD_ShowString(0, 0, 200, 16, 16, &p[9]);
	atk_8266_send_cmd(p,"WIFI GOT IP",30);

	netpro=0x01;

		if(netpro&0X01)     //TCP Client    透传模式测试
		{
			//LCD_Clear(WHITE);
			POINT_COLOR=RED;
			int i;
			for(i=0;i<16;i++)
				ipbuf[i]=ipadd[i];
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","192.168.43.154",portnum);    //配置目标TCP服务器
			
					cmd="AT+CIPSTART=\"TCP\",\"192.168.43.154\",12345";
		printf("%s\r\n",cmd);
		LCD_ShowString(0,120, 240, 16, 16, cmd);
		delay_ms(5000);
		
			
			LCD_ShowString(30, 125, 200, 16, 16, ipbuf);
			LCD_ShowString(0, 145, 200, 16, 16, &p[10]);

			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
		
		}


				atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
				sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
				printf("%s",p);
			USART_RX_STA=0;
			while(1)
			{
				int i=0;
				key=KEY_Scan(0);
				if(key==KEY0_PRES)	//KEY0 发送数据 
				{
					if((netpro==1))   //TCP Client
					{
						i++;
					sprintf((char*)p,"阿伟牛逼%d次",i);//设置无线参数:ssid,密码
					senddata(p);
					}
				}
				if(USART_RX_STA&0X8000)		//接收到一次数据了
				{ 
					rlen=USART_RX_STA&0X7FFF;	//得到本次接收到的数据长度
					USART_RX_BUF[rlen]=0;		//添加结束符 
					printf("shouu%s",USART_RX_BUF);	//发送到串口   
					sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
					USART_RX_STA=0;
					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
					else t=0;                   //状态为已经连接了,10秒后再检查
				}  
			}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 




























