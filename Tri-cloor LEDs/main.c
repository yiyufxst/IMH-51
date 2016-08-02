#include<reg52.h>	//51单片机常用的头文件，定义常用的IO口

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int	//宏定义一个无符号的int类型

sbit red=P2^2;	  //定义红色LED，为P2.2 IO口	灯1
sbit green=P2^1;  //定义绿色LED，为P2.1 IO口
sbit blue=P2^0;	  //定义蓝色LED，为P2.0 IO口

sbit red1=P2^7;	    //定义红色LED，为P2.5 IO口	灯2
sbit green1=P2^6;	//定义绿色LED，为P2.4 IO口
sbit blue1=P2^5;	//定义蓝色LED，为P2.2 IO口

void delay(uint z)	//进行延时处理,z毫秒
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=112;y>0;y--);
}

void led_off()	//灯1全灭
{
	red=0;
	green=0;
	blue=0;
}

void led_off1()	//灯2全灭
{
	red1=0;
	green1=0;
	blue1=0;
}

void led_on(uchar r,uchar g,uchar b)	//亮灯1
{	
	red=r;
	green=g;
	blue=b;
}

void led_on1(uchar r,uchar g,uchar b)	//亮灯2
{	
	red1=r;
	green1=g;
	blue1=b;
}

void led_flash(uchar r,uchar g,uchar b)	//灯1闪烁
{
	led_off();
	delay(50);
	led_on(r,g,b);
	delay(50);
}

void led_flash1(uchar r,uchar g,uchar b)	//灯2闪烁
{
	led_off1();
	delay(50);
	led_on1(r,g,b);
	delay(50);
}

void main(){
	led_off();
	led_off1();
	while(1)
	{
		P0=0xFF;	//关闭数码管
		led_flash(1,0,1);
		led_flash1(1,1,0);
	}	
}
