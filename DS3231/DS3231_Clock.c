#include<reg52.h>	            //51单片机常用的头文件，定义常用的IO口
#include<DS3231.h>              //DS3231模块自定义头文件
#include<LedSegmentDisplay.h>   //数码管显示自定义头文件

#define uchar unsigned char    //宏定义一个无符号的char类型
    
uchar code table[] = {         //数码管显示表
0x3f  , 0x06 , 0x5b , 0x4f ,
0x66 , 0x6d ,0x7d , 0x07 , 
0x7f  , 0x6f , 0x77 , 0x7c ,
0x39 , 0x5e , 0x79 , 0x71
};

void main()
{
    uchar t0, t1, t2, t3, t4, t5;
    DS3231_TIME CurrentTime;
    DS3231Init();
    if(!BcdByte(DS3231ReadData(DS3231_SECOND))){
        DS3231SetTime(DS3231_HOUR, 12);    //设置时间为12:34:56
        DS3231SetTime(DS3231_MINUTE, 34);  
        DS3231SetTime(DS3231_SECOND, 56);  
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
    }
}
 