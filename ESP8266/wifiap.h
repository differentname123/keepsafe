#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "lcd.h"   
#include "string.h"    

u8 atk_8266_wifista_test(void);	//WIFI AP??
u8 atk_8266_wifiap_test(void);
void lianjie();
void sendphoto(u16 data);
void senddata(char *data);
void jieshou();

#endif





