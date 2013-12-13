#include<reg52.h>
#include<reg52.h>
#include<intrins.h>
sbit BPC=P1^3;
sbit RS=P1^0;
sbit RW=P1^1;
sbit LCDE=P1^2;

int init1602();
int transdata(unsigned char tdata);
int command(unsigned char cmd);
void Delay1ms();
void Delay100ms();
char bpcrecv(char recv[]);
char processbpc();
void writetime();
void writestatus(char OK);
void minplus();
void secplus();
void hourplus();
void dayplus();
void monthplus();
//void yearplus();//����Ѿ���������				   

int count,countms,i,j,bits;
char recvnum[20];
int year=1,month=1,week=1,day=1,hour=0,minute=0,second=0;
unsigned char code weekday[7][4]={{"Mon."},{"Tue."},{"Wed."},{"Thu."},{"Fri."},{"Sat."},{"Sun."}};
char OK=0;
unsigned int checkedminute;

int main()
{
	init1602();
	Delay1ms();
	Delay1ms();
	TMOD=0x01;
	//TH0=(65536-45)/256;//���ٲ���
	//TL0=(65536-45)%256;//���ٲ���
	TH0=(65536-46062)/256;
	TL0=(65536-46062)%256;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		while(bpcrecv(recvnum)!=0);//writestatus(0); 
		processbpc();
		//writetime();
		//writestatus(1);
					   


	}
return 0;
}

char bpcrecv(char recv[])
{
		do
		{
			//writestatus(0);
			for(count=0;BPC==0;count++)
				Delay1ms();
			//if(count<10)writestatus(0);
		}
		while (count<1250) ;
		for(bits=0;bits<19;bits++) 
		{
			while(BPC==0);//�ðɣ��ȵȴ�����������þ�ȷʱ��
			for(count=0;BPC==1;count++)
				Delay1ms();
			if(count<70) return 1;
			recv[bits]=(count+30)/100-1;
			 
		}
		OK=1;
		checkedminute=0;
		return 0;
}

char processbpc()
{	
	int timedelay;
	for(timedelay=(10-(recvnum[18])+1);timedelay<=10;timedelay++)
		Delay100ms();	//��ȷ��
	countms=7;
	switch(recvnum[0])
	{
		case 0:second=19;break;
		case 1:second=39;break;
		case 2:second=59;break;
		//default:return 1;
	}
	
	hour=recvnum[2]*4+recvnum[3]*1;
	if(recvnum[9]>=2)hour+=12;
	minute=recvnum[4]*16+recvnum[5]*4+recvnum[6]*1;
	week=recvnum[7]*4+recvnum[8]*1;
	day=recvnum[10]*16+recvnum[11]*4+recvnum[12]*1;
	month=recvnum[13]*4+recvnum[14]*1;
	year=recvnum[15]*16+recvnum[16]*4+recvnum[17]*1;
	//if(recvnum[18]>=2)year+=64;
	//if(second==60)minute++,second=0;
	//if(minute==60)hour++,minute=0;
	//if(hour==24)day++,hour==0;

	return 0;
}

void writetime()
{
	command(0x80);
	transdata('2');
	transdata(year/100+48);
	transdata(year%100/10+48);
	transdata(year%10+48);
	transdata('-');
	transdata(month/10+48);
	transdata(month%10+48);
	transdata('-');
	transdata(day/10+48);
	transdata(day%10+48);
	transdata(' ');
	transdata(weekday[week-1][0]);
	transdata(weekday[week-1][1]);
	transdata(weekday[week-1][2]);
	transdata(weekday[week-1][3]);
	//transdata(recvnum[0]+48);
}

void writestatus(char OK)
{
	command(0xc0);
	transdata(hour/10+48);
	transdata(hour%10+48);
	transdata(':');
	transdata(minute/10+48);
	transdata(minute%10+48);
	transdata(':');
	transdata(second/10+48);
	transdata(second%10+48);
	transdata(' ');
	if(OK)
	{
		transdata('C');
		transdata('h');
		transdata('e');
		transdata('c');
		transdata('k');
		transdata('e');
		transdata('d');
	}
	else
	{
		transdata('N');
		transdata('o');
		transdata('t');
		transdata('R');
		transdata('e');
		transdata('c');
		transdata('v');
	}
}



void Delay1ms()		//@11.0592MHz,1ms
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


 void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

 int init1602()
 {	
 	LCDE=0;
	RW=0;
	command(0x38);
	command(0x0c);
	command(0x06);
	command(0x01);
	command(0x80);
	return 0;
}

int command(unsigned char cmd)
{
	RS=0;
	P0=cmd;
	//delay(2);
	Delay1ms();
	LCDE=1;
	//delay(5);
	//delay(1);//�ȴ������ȡ��ϣ�LCD�ÿ죡
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();  //��Ĳ����ٿ���
	Delay1ms();
	LCDE=0;
	return 0;
}

int transdata(unsigned char tdata)
{
	RS=1;
	P0=tdata;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//delay(2);
	LCDE=1;
	//delay(5);
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	LCDE=0;
	return 0;
}

void secplus() 
{
	if(++second==60)
	{
		second=0;
		minplus();
	}
	writetime();
	writestatus(OK);
}

void minplus() 
{
	if(++checkedminute>=60)
		OK=0;
	if(++minute==60)
	{
		minute=0;
		hourplus();
	}



}


void hourplus()
{
	if(++hour==24)
	{
		hour=0;
		dayplus();
	}

}

void dayplus()
{ 	
	if(++week==8)
		week=0;
	if(day==28&&month==2&&(year%4!=0&&year!=100))
		{monthplus();day=0;}
	if(day==29&&month==2)
		{monthplus();day=0;}
	if(day==30&&(month==4||month==6||month==9||month==11))
		{monthplus();day=0;}
	if(day==31)
		{monthplus();day=0;}
	day++;
}

void monthplus()
{
	if(month==12)
		{year++;month=0;}
	month++;
}

void seccount() interrupt 1	//��ʱ����50ms*20ms
{
	//TH0=(65536-45)/256;//���ٲ���
	//TL0=(65536-45)%256;//���ٲ���
	TH0=(65536-46062)/256;
	TL0=(65536-46062)%256;
	//scount=1;
	countms++; //ÿ�ζ�ʱ���������൱����ʱ��������20ms������Ӧ�����ж��Ƿ�1s֮ǰ�ۼӼ�����
	if(countms==20)
	{
		countms=0;
		
		secplus();
				}
 }
