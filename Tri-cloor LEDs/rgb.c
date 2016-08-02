#include<reg52.h>	//51单片机常用的头文件，定义常用的IO口

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int	//宏定义一个无符号的int类型

sbit red=P2^2;    //定义红色LED，为P2.2 IO口
sbit green=P2^1;  //定义绿色LED，为P2.1 IO口
sbit blue=P2^0;	  //定义蓝色LED，为P2.0 IO口

uchar count=0;	//定时器T0计数
uchar rgb_red=0;	//红色rgb值，最大255
uchar rgb_green=0;	//绿色rgb值，最大255
uchar rgb_blue=0;	//蓝色rgb值，最大255

void timer0_init()
{
	TMOD=0x01;	//定时器工作方式，仅T0
	TH0= 0XFF;  //100us定时
	TL0= 0XA4;
	EA=1;	//开总中断
	ET0=1;  //开定时器
	TR0=1;  //启动定时器
}

void delay(uint z)	//进行延时处理,z毫秒
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=112;y>0;y--);
}

void led_off()	//灯全灭
{
	rgb_red=0;
	rgb_green=0;
	rgb_blue=0;
}

void led_color(uchar r,uchar g,uchar b)	//亮某色灯
{	
	rgb_red=r;
	rgb_green=g;
	rgb_blue=b;
}

void led_on(uchar r,uchar g,uchar b)	//某种灯常亮（由于rgb的亮度实际有闪烁频度控制，所以值不应过低，且应该大致相当）
{
	delay(50);
	led_color(r,g,b);
	delay(50);
}

void led_flash(uchar r,uchar g,uchar b)	//某种灯闪烁
{
	led_off();
	delay(50);
	led_color(r,g,b);
	delay(50);
}

void main(){
	timer0_init();
	led_off();
	while(1)
	{
		led_flash(0,200,200);			
	}
}

void timer0() interrupt 1	//定时中断led函数，定时器0工作方式1
{
	TH0= 0XFF;  //100us定时
	TL0= 0XA4;
	count++;
	
	if(count>=255)	
		count=0;
	
	if(count<rgb_red)	//红灯高电平的时间
		red=1;
	else
		red=0;
	if(count<rgb_green)	//绿灯高电平的时间
		green=1;
	else
		green=0;
	if(count<rgb_blue)	//蓝灯高电平的时间
		blue=1;
	else 
		blue=0;	
}