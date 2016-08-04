#ifndef _DS1302_    //如果未包含该头文件，就包含它
#define _DS1302_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int	//宏定义一个无符号的int类型

sbit SCLK = P2^1;    //时钟线引脚
sbit SDA  = P2^0;    //数据线引脚
sbit RST  = P2^4;    //复位线引脚
sbit  ACC0 = ACC^0;    //ACC累加寄存器的数据从低到高排列是ACC0-ACC7
sbit  ACC7 = ACC^7;

typedef struct __SYSTEMTIME__    //定义时间的结构体变量
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
}SYSTEMTIME;
 
#define SECOND  0X80    //宏定义时钟写寄存器及相关参数
#define MINUTE  0x82
#define HOUR    0x84
#define WEEK    0x8A
#define DAY     0x86
#define MONTH   0x88
#define YEAR    0x8c

void InputByte(uchar da)    //写入一字节数据
{
    uchar i;
    ACC = da;
    for(i = 8; i > 0; i--)
    {
        SDA = ACC0;    //相当于汇编中的RRC（循环右移指令）
        SCLK = 1;
        SCLK = 0;
        ACC = ACC >> 1;    //右移1位，高位补0
    }
}

uchar OutputByte(void)    //读取一字节数据
{
    uchar i;
    for(i = 8; i > 0; i--)
    {
        ACC = ACC >> 1;    //右移1位，高位补0
        ACC7 = SDA;
        SCLK = 1;
        SCLK = 0;
    }
    return ACC;
}

void WriteData(uchar addr,uchar da)    //写入DS1302某地址的数据 
{
    RST = 0;
    SCLK = 0;
    RST = 1;
    InputByte(addr);
    InputByte(da);
    RST = 0;  
}

uchar ReadData(uchar addr)    //读取DS1302某地址的数据 
{
    uchar da;
    RST = 0;
    SCLK = 0;
    RST = 1;
    InputByte(addr | 0x01);
    da=OutputByte();
    RST = 0;
    return da;
}

void SetProtect(uint flag)    //是否开启写保护，0-关闭，1-开启
{
    if(flag)
        WriteData(0x8E, 0x10);
    else
        WriteData(0x8E, 0x00);
}

void SetTime(uchar addr, uchar value)    //设置时间，将十进制转为BCD码
{
    SetProtect(0);
    WriteData(addr, ((value/10)<<4 | (value%10)));
}

void GetTime(SYSTEMTIME *Time)    //获取时间，将BCD码转为十进制
{
    uchar ReadValue;
    ReadValue = ReadData(SECOND);
    Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = ReadData(MINUTE);
    Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);    
    ReadValue = ReadData(HOUR);
    Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);    
    ReadValue = ReadData(DAY);
    Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);     
    ReadValue = ReadData(WEEK);
    Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = ReadData(MONTH);
    Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);   
    ReadValue = ReadData(YEAR);
    Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 
}

void DateToStr(SYSTEMTIME *Time)    //年月日格式化存入DateString
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

void TimeToStr(SYSTEMTIME *Time)    //时分秒格式化存入TimeString
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

void Initial_DS1302(void)    //模块初始化
{
    uchar Second = ReadData(SECOND);
    if(Second&0x80)
        SetTime(SECOND,0);
}

#endif