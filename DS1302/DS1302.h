#ifndef _DS1302_    //如果未包含该头文件，就包含它
#define _DS1302_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int      //宏定义一个无符号的int类型

sbit DS1302_SCL = P2^1;    //时钟线引脚
sbit DS1302_SDA = P2^0;    //数据线引脚
sbit DS1302_RST = P2^4;    //复位线引脚

sbit ACC0 = ACC^0;    //ACC累加寄存器的数据从低到高排列是ACC0-ACC7
sbit ACC7 = ACC^7;

typedef struct __DS1302_TIME__    //定义时间的结构体变量
{
    uchar Second;
    uchar Minute;
    uchar Hour;
    uchar Week;
    uchar Day;
    uchar Month;
    uchar Year;
    uchar DateString[9];    //年月日格式化字符串
    uchar TimeString[9];    //时分秒格式化字符串
}DS1302_TIME;
 
#define DS1302_SECOND  0X80    //宏定义时钟写寄存器及相关参数
#define DS1302_MINUTE  0x82
#define DS1302_HOUR    0x84
#define DS1302_WEEK    0x8A
#define DS1302_DAY     0x86
#define DS1302_MONTH   0x88
#define DS1302_YEAR    0x8c

void DS1302InputByte(uchar da)    //写入一字节数据
{
    uchar i;
    ACC = da;
    for(i = 8; i > 0; i--)
    {
        DS1302_SDA = ACC0;    //相当于汇编中的RRC（循环右移指令）
        DS1302_SCL = 1;
        DS1302_SCL = 0;
        ACC = ACC >> 1;       //右移1位，高位补0
    }
}

uchar DS1302OutputByte(void)    //读取一字节数据
{
    uchar i;
    for(i = 8; i > 0; i--)
    {
        ACC = ACC >> 1;    //右移1位，高位补0
        ACC7 = DS1302_SDA;
        DS1302_SCL = 1;
        DS1302_SCL = 0;
    }
    return ACC;
}

void DS1302WriteData(uchar addr,uchar da)    //写入DS1302某地址的数据 
{
    DS1302_RST = 0;
    DS1302_SCL = 0;
    DS1302_RST = 1;
    DS1302InputByte(addr);
    DS1302InputByte(da);
    DS1302_RST = 0;  
}

uchar DS1302ReadData(uchar addr)    //读取DS1302某地址的数据 
{
    uchar da;
    DS1302_RST = 0;
    DS1302_SCL = 0;
    DS1302_RST = 1;
    DS1302InputByte(addr | 0x01);
    da=DS1302OutputByte();
    DS1302_RST = 0;
    return da;
}

void DS1302SetProtect(uint flag)    //是否开启写保护，0-关闭，1-开启
{
    if(flag)
        DS1302WriteData(0x8E, 0x10);
    else
        DS1302WriteData(0x8E, 0x00);
}

void DS1302SetTime(uchar addr, uchar value)    //设置时间，将十进制转为BCD码
{
    DS1302SetProtect(0);
    DS1302WriteData(addr, ((value/10)<<4 | (value%10)));
}

void DS1302GetTime(DS1302_TIME *Time)    //获取时间，将BCD码转为十进制
{
    uchar ReadValue;
    ReadValue = DS1302ReadData(DS1302_SECOND);
    Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = DS1302ReadData(DS1302_MINUTE);
    Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);    
    ReadValue = DS1302ReadData(DS1302_HOUR);
    Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);    
    ReadValue = DS1302ReadData(DS1302_DAY);
    Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);     
    ReadValue = DS1302ReadData(DS1302_WEEK);
    Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = DS1302ReadData(DS1302_MONTH);
    Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = DS1302ReadData(DS1302_YEAR);
    Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 
}

void DS1302DateToStr(DS1302_TIME *Time)    //年月日格式化存入DateString
{
    Time->DateString[0] = Time->Year/10;
    Time->DateString[1] = Time->Year%10;
    Time->DateString[2] = '-';
    Time->DateString[3] = Time->Month/10;
    Time->DateString[4] = Time->Month%10;
    Time->DateString[5] = '-';
    Time->DateString[6] = Time->Day/10;
    Time->DateString[7] = Time->Day%10;
    Time->DateString[8] = '\0';
}

void DS1302TimeToStr(DS1302_TIME *Time)    //时分秒格式化存入TimeString
{
    Time->TimeString[0] = Time->Hour/10;
    Time->TimeString[1] = Time->Hour%10;
    Time->TimeString[2] = ':';
    Time->TimeString[3] = Time->Minute/10;
    Time->TimeString[4] = Time->Minute%10;
    Time->TimeString[5] = ':';
    Time->TimeString[6] = Time->Second/10;
    Time->TimeString[7] = Time->Second%10;
    Time->TimeString[8] = '\0';
}

void DS1302Initial(void)    //模块初始化
{
    uchar Second = DS1302ReadData(DS1302_SECOND);
    if(Second&0x80)
        DS1302SetTime(DS1302_SECOND,0);
}

#endif