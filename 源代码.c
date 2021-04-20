/**
@author curiosity2620
@date 2019 / 3 / 12 - 20:43
*/
#include <reg52.h>
#define uint unsigned int   
unsigned char data;
sbit M=P3^5;//此引脚接舵机信号线
sbit N=P2^0;//或此引脚接舵机信号线
sbit led=P3^6;
void delayms(uint);//延时函数

void UsartInit()//串口初始化
{
  TMOD=0X20;//工作方式控制寄存器
  TH1=0xF4;//Timer寄存器的高八位
  TL1=0xF4;//低八位
  PCON=0x80;//PCON 电源管理寄存器
  TR1=1;//T1计数器控制位
  SCON=0x50;//串行口控制寄存器
  ES=1;//串口中断开关
  EA=1;//中断总开关
}

void main()
{
  UsartInit();//调用串口初始化
  while(1);//等待串口中断
}

void Usart() interrupt 4 //中断处理函数，中断号为4
{  
   data=SBUF;//发送接收数据缓存空间
   RI=0;//接收中断标志位
   SBUF=data;//原样返回接收的数
   while(!TI);//如果发送了数据则继续向后运行
   TI=0;//发送中断标志位
   
   if(data=='D')//(door)如果接收到数据D，让舵机转动
  { 
            int b=0;//用于控制产生的pwm信号段长度，每个不同的pwm段对应一个舵机角度，因此控制了舵机在某角度停留的时长
            while(b<30)//产生pwm信号，控制舵机转动到起始角度
            {
				 M=1,N=1;
	             delayms(1);
	             M=0,N=0;
	             delayms(19);
	             b++;
            }
			 	 b=0;
            while(b<200)//产生pwm信号，控制舵机转动到中间角度并保持2s
            {
				 M=1,N=1;
	             delayms(2);
	             M=0,N=0;
	             delayms(18);
	             b++;
            }
				 b=0;
			 while(b<30)//产生pwm信号，控制舵机回到初始位置
            {
				 M=1,N=1;
	             delayms(1);
	             M=0,N=0;
	             delayms(19);
	             b++;
            }
   }
  	
   if(data=='L')//(led)如果接收到的数据L，让LED反转
   {
     led=~led;
   }
}  

void delayms(uint xms)//延时函数，参数为X毫秒
{
uint i,j;
for(i=xms;i>0;i--)
   for(j=110;j>0;j--);

}
