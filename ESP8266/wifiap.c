//#include "wifiap.h"
//#include "lcd.h"
//#include "malloc.h"
//#include "key.h"
//#include "led.h"
//#include "usart.h"
//#include "delay.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32开发板
////ATK-ESP8266 WIFI模块 WIFI AP驱动代码	   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2014/4/3
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2009-2019
////All rights reserved									  
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
//const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
//const u8* portnum="12345";		
////在指定位置开始显示一个字符串	    
////支持自动换行
////(x,y):起始坐标
////width,height:区域
////str  :字符串
////size :字体大小
////mode:0,非叠加方式;1,叠加方式    	   		   
//void Show_Str(u16 x,u16 y,u16 width,u16 height,char*str,u8 size,u8 mode)
//{
//	LCD_ShowString(x,y,width,height,size,str);

//}


//void Show_Str_Mid(u16 x,u16 y,char*str,u8 size,u8 len)
//{
//	LCD_ShowString(x,y,200,16,size,str);

//}


//u8* kbd_fn_tbl[2];

////ATK-ESP8266发送命令后,检测接收到的应答
////str:期待的应答结果
////返回值:0,没有得到期待的应答结果
////    其他,期待应答结果的位置(str的位置)
//u8* atk_8266_check_cmd(u8 *str)
//{
//	
//	char *strx=0;
//	if(USART_RX_STA&0X8000)		//接收到一次数据了
//	{ 
//		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
//		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
//	} 
//	return (u8*)strx;
//}

////向ATK-ESP8266发送命令
////cmd:发送的命令字符串
////ack:期待的应答结果,如果为空,则表示不需要等待应答
////waittime:等待时间(单位:10ms)
////返回值:0,发送成功(得到了期待的应答结果)
////       1,发送失败
//u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
//{
//	u8 res=0; 
//	USART_RX_STA=0;
//	printf("%s\r\n",cmd);	//发送命令
//	if(ack&&waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(USART_RX_STA&0X8000)//接收到期待的应答结果
//			{
//				if(atk_8266_check_cmd(ack))
//				{
//					printf("ack:%s\r\n",(u8*)ack);
//					break;//得到有效数据 
//				}
//					USART_RX_STA=0;
//			} 
//		}
//		if(waittime==0)res=1; 
//	}
//	return res;
//} 




////IP设置
////title:ip设置标题
////mode:工作模式
////port:端口号
////*ip:ip缓存区(返回IP给上层函数)
////返回值:0,确认连接;1,取消连接.
//u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
//{
//	u8 res=0;
// 	u8 key;
//	u8 timex=0;  
//	u8 iplen=0;			//IP长度 
//	LCD_Clear(WHITE);  
//	POINT_COLOR=RED; 

//	kbd_fn_tbl[0]="连接";
//	kbd_fn_tbl[1]="返回"; 
//	printf("atk_8266_load_keyboard(0,180);");			//显示键盘 
//	POINT_COLOR=BLUE;
// //	Show_Str(30+72,90,200,16,mode,16,0);	//显示工作模式	
// 	//Show_Str(30+40,130,200,16,port,16,0);	//显示端口 	
//	ip[0]=0; 		
////	while(1)
////	{ 
////		key=atk_8266_get_keynum(0,180);
////		if(key)
////		{   
////			if(key<12)
////			{
////				if(iplen<15)
////				{ 
////					ip[iplen++]=kbd_tbl[key-1][0];
////				}
////			}else
////			{
////				if(key==13)if(iplen)iplen--;	//删除  
////				if(key==14&&iplen)break; 		//确认连接 
////				if(key==15){res=1;break;}		//取消连接
////			} 
////			ip[iplen]=0; 
////			LCD_Fill(30+56,110,239,110+16,WHITE);
////			//Show_Str(30+56,110,200,16,ip,16,0);			//显示IP地址 	
////		} 
////		timex++;
////		if(timex==20)
////		{
////			timex=0;
////			
////		}
////		delay_ms(10);
////		printf("atk_8266_at_response(1)");//WIFI模块发过来的数据,及时上传给电脑
////	} 
//	return res;
//}







//u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
//{
//	u8 key=0,t=0,*p;
//	u8 netpro=0;
//	LCD_Clear(WHITE);
//	POINT_COLOR=RED;
//	p=mymalloc(0,50);//申请50个字节的内存
//	sprintf((char*)p,"%s 工作模式选择",name);			    	 
//	Show_Str(x,y+30,200,16,"KEY0:下一个",16,0); 				    	 
//	Show_Str(x,y+50,200,16,"KEY1:上一个",16,0);				    	 
//	Show_Str(x,y+70,200,16,"WK_UP:确定",16,0); 
//	Show_Str(x,y+100,200,16,"请选择:",16,0); 
//	POINT_COLOR=BLUE;
// 	Show_Str(x+16,y+120,200,16,"TCP服务器",16,0); 				    	 
//	Show_Str(x+16,y+140,200,16,"TCP客户端",16,0);				    	 
//	Show_Str(x+16,y+160,200,16,"UDP模式",16,0); 
//	POINT_COLOR=RED;
//	Show_Str(x,y+120,200,16,"→",16,0); 
//	while(1)
//	{
//		key=KEY_Scan(0); 
//		if(key)
//		{
//			if(key==WKUP_PRES)break;       //WK_UP按下
//			Show_Str(x,y+120+netpro*20,200,16,"  ",16,0);//清空之前的显示
//			if(key==KEY0_PRES)//KEY0按下
//			{
//				if(netpro<2)netpro++;
//				else netpro=0;
//			}else if(key==KEY1_PRES)//KEY1按下
//			{
//				if(netpro>0)netpro--;
//				else netpro=2; 
//			}
//			Show_Str(x,y+120+netpro*20,200,16,"→",16,0);//指向新条目
//				
//		} 
//		delay_ms(10);
//		//atk_8266_at_response(1);
//		printf("//atk_8266_at_response(1);\n");
//		int temp=0;
//		if((t++)==20)
//			{
//				temp++;
//				temp%=2;
//				LED_R(temp);

//			}//LED闪烁
//	} 
//	myfree(0,p);
//	return netpro;//返回网络模式选择值 
//} 








////ATK-ESP8266 WIFI AP测试
////用于测试TCP/UDP连接
////返回值:0,正常
////    其他,错误代码

//u8 atk_8266_wifiap_test(void)
//{
//	u8 netpro=0;	//网络模式
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP缓存
//	u8 *p=0;
//	u16 t=999;		//加速第一次获取链接状态
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//连接状态
//	//LCD_ShowString(30, 85, 200, 16, 16, "Pandora STM32L4 IOT");
//	p=mymalloc(0,32);							//申请32字节内存
//	printf("\r\n申请32字节内chun%d\r\n",(int)p);
//	LCD_ShowString(30, 85, 200, 16, 16, (char*)p);
////	
//PRESTA:
//	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//选择网络模式
//	if(netpro&0X02)   //UDP
//	{
//		
//		printf("\r\nudp mode\r\n");
////				LCD_Clear(WHITE);
////				POINT_COLOR=RED;
////				Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
////				Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
////				if(atk_8266_ip_set("WIFI-AP 远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
////				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器
////				atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //单链接模式
////				LCD_Clear(WHITE);
////				while(atk_8266_send_cmd(p,"OK",500));
//			
//	}
//	else     //TCP
//	{
//		     //TCP Client    透传模式测试
//		
//			LCD_Clear(WHITE);
//			POINT_COLOR=RED;
//			Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
//			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
//			char ipadd[]="1011101010101010";
//			int i;
//			for(i=0;i<16;i++)
//				ipbuf[i]=ipadd[i]-'0';
//			//if(atk_8266_ip_set("WIFI-AP 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
//			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
//			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
//			while(atk_8266_send_cmd(p,"OK",200))
//			{
//				LCD_Clear(WHITE);
//				POINT_COLOR=RED;
//				Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
//				Show_Str_Mid(0,80,"ATK-ESP 连接TCP Server失败",12,240); //连接失败	
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)goto PRESTA;
//			}	
//			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
//			
//	}
////			LCD_Clear(WHITE);
////			POINT_COLOR=RED;
////			Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240);
////			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
////			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
////			Show_Str(30,50,200,16,"配置ATK-ESP模块成功!",12,0);
////			delay_ms(200);
////			Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
////			LCD_Fill(30,80,239,80+12,WHITE);
////			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
////			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
////			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
////			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
////			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
////			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
////			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
////			atk_8266_wificonf_show(30,180,"请用设备连接WIFI热点:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
////			POINT_COLOR=BLUE;
////			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
////			USART3_RX_STA=0;
////			while(1)
////			{
////				key=KEY_Scan(0);
////				if(key==WKUP_PRES)			//WK_UP 退出测试		 
////				{  
////					res=0;
////					atk_8266_quit_trans();	//退出透传
////					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
////					break;												 
////				}
////				else if(key==KEY0_PRES)	//KEY0 发送数据 
////				{
////				
////					if((netpro==3)||(netpro==2))   //UDP
////					{
////						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
////						Show_Str(30+54,100,200,12,p,12,0);
////						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //发送指定长度的数据
////						delay_ms(200);
////						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
////						timex=100;
////					}
////					else if((netpro==1))   //TCP Client
////					{
////						atk_8266_quit_trans();
////						atk_8266_send_cmd("AT+CIPSEND","OK",20);       //开始透传
////						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
////						Show_Str(30+54,100,200,12,p,12,0);
////						u3_printf("%s",p);
////						timex=100;
////					}
////					else    //TCP Server
////					{
////						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
////						Show_Str(30+54,100,200,12,p,12,0);
////						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
////						delay_ms(200);
////						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
////						timex=100;
////					}
////				}
////			
////	if(timex)timex--;
////	if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
////	t++;
////	delay_ms(5);
////	if(USART3_RX_STA&0X8000)		//接收到一次数据了
////	{ 
////		rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
////		USART3_RX_BUF[rlen]=0;		//添加结束符 
////		printf("%s",USART3_RX_BUF);	//发送到串口   
////		sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
////		LCD_Fill(30+54,115,239,130,WHITE);
////		POINT_COLOR=BRED;
////		Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
////		POINT_COLOR=BLUE;
////		LCD_Fill(30,130,239,319,WHITE);
////		Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据  
////		USART3_RX_STA=0;
////		if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
////		else t=0;                   //状态为已经连接了,10秒后再检查
////	}  
////	if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
////	{
////		constate=atk_8266_consta_check();//得到连接状态
////		if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
////		else Show_Str(30+30,80,200,12,"连接失败",12,0); 	  	 
////		t=0;
////	}
////	if((t%20)==0)LED0=!LED0;
////	atk_8266_at_response(1);
////}
////	myfree(SRAMIN,p);		//释放内存 
//	return res;		
//} 







