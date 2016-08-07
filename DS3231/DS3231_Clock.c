#include<reg52.h>	            //51单片机常用的头文件，定义常用的IO口
#include<DS3231.h>              //DS3231模块自定义头文件
#include<LedSegmentDisplay.h>   //数码管显示自定义头文件

#define uchar unsigned char    //宏定义一个无符号的char类型
    
sbit sound = P2^3;    //蜂鸣器IO口
    
uchar code table[] = {         //数码管显示表
0x3f  , 0x06 , 0x5b , 0x4f ,
0x66 , 0x6d ,0x7d , 0x07 , 
0x7f  , 0x6f , 0x77 , 0x7c ,
0x39 , 0x5e , 0x79 , 0x71
};

DS3231_TIME CurrentTime;    //DS3231时钟自定义时间结构体

void warn(uchar n)    //蜂鸣器闹铃
{
    uchar i;
    for(i = 0; i < n; i++)
    {
        sound = 0;
        delay(20);
        sound = 1;  
    }
}

void AlarmClock(uchar hour, uchar minute, uchar second)    //设置闹钟
{
    uchar clock[2];
    DS3231WriteData(DS3231_ALARM1HOUR, hour);         //写入闹钟时间
    DS3231WriteData(DS3231_ALARM1MINUTE, minute);
    DS3231WriteData(DS3231_ALARM1SECOND, second);
    
    clock[0] =  DS3231ReadData(DS3231_ALARM1HOUR);    //读取闹钟时间
    clock[1] =  DS3231ReadData(DS3231_ALARM1MINUTE);
    clock[2] =  DS3231ReadData(DS3231_ALARM1SECOND);
    if(clock[0]==CurrentTime.Hour && clock[1]==CurrentTime.Minute && clock[2]==CurrentTime.Second)
    {
        warn(8);
    }
}
void main()
{
    uchar t0, t1, t2, t3, t4, t5;
    DS3231Init();
    if(!BcdByte(DS3231ReadData(DS3231_SECOND)))
    {
        DS3231SetTime(DS3231_HOUR, 11);    //设置时间为11:12:41
        DS3231SetTime(DS3231_MINUTE, 12);  
        DS3231SetTime(DS3231_SECOND, 41);  
    }
    while(1)
    {
        DS3231GetTime(&CurrentTime);       //获取当前时间，格式化并在数码管上显示出来
        DS3231TimeToStr(&CurrentTime);    
        t0 = CurrentTime.TimeString[0];
        t1 = CurrentTime.TimeString[1];
        t2 = CurrentTime.TimeString[3];
        t3 = CurrentTime.TimeString[4];
        t4 = CurrentTime.TimeString[6];
        t5 = CurrentTime.TimeString[7];
        display(table[t0], table[t1], table[t2], table[t3], table[t4], table[t5]);
        AlarmClock(11, 13, 51);        
    }
}
 