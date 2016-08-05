#ifndef _LedSegmentDisplay_    //如果未包含该头文件，就包含它
#define _LedSegmentDisplay_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int      //宏定义一个无符号的int类型
    
sbit du = P2^6;    //数码管显示值
sbit we = P2^7;    //选择某个数码管显示
    
void delay()    //进行延时处理,1毫秒
{
	uint x, y;
	for(x = 1; x > 0; x--)
		for(y = 112; y > 0; y--);
}

void displayOne(uchar addr, uchar da)    //选取某个数码管（地址addr），显示数据（da）
{
    we = 1;
    P0 = addr;
    we = 0;
    P0 = 0;
    du = 1;
    P0 = da;
    du = 0;
    delay();
}

void display(uchar sw, uchar wan, uchar qian, uchar bai, uchar shi, uchar ge)    //在前六个数码管显示相应数据
{
    displayOne(0xfe, sw);
    displayOne(0xfd, wan);
    displayOne(0xfb, qian);
    displayOne(0xf7, bai);
    displayOne(0xef, shi);
    displayOne(0xdf, ge);
}

#endif