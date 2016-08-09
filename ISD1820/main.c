#include<reg52.h>    //51单片机常用的头文件，定义常用的IO口

sbit play = P1^4;   //ISD1820单次播放

void playe()    //电压的变化触发ISD1820的单次播放
{
    play = 0;
    play = 1;   
}

void main()
{
    while(1)
    {
        int i = 65000;
        while(--i);
        playe();
    }
}