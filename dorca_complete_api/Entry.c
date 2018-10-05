//#define ETRY_CODE

#ifndef ETRY_CODE 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#endif


#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "./function/dorca30_api.h"
#include "./function/define.h"
#include "./function/global.h"
#include "./function/dorca.h"
#ifdef OPENSSL_SUPPORT
#include <openssl/ecdh.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#endif

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define NETBUF_SIZE 161
#define ENCBUF_SIZE 160
#define WHEREAMI() printf("%s %d\n",__FILE__,__LINE__)

int fd_19;
#define MAX_BUF 1024
//#define DEMO_BOARD
#ifndef ETRY_CODE
#if 1
#define CS0   8
#define CS1   7
#define GPIO05 5
#define POWER 12
#define INT0 6
#else
#define POWER 26
#define INT0 19
#endif
void Export(int gpio)
{
	char buf[MAX_BUF]; 
	int fd;
	//   printf("export %d\n",gpio); 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio); 
	write(fd, buf, strlen(buf));
	close(fd);
	return;
}

void UnExport(int gpio)
{
	char buf[MAX_BUF]; 
	int fd;

	//	printf("unExport %d\n",gpio);
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
	return;
}

void Direction(int gpio, int dir)
{
	char buf[MAX_BUF]; 
	int fd;

	//	printf("Direction %d dir %d\n",gpio,dir);
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);

	// Set out Direction
	if(dir == 1)
		write(fd, "out", 3); 
	else// Set in Direction
		write(fd, "in", 2); 

	close(fd);
	return;
}

void ReadValue(int gpio)
{
	char buf[MAX_BUF]; 
	int fd;

	char value;

	//  printf("ReadValue %d \n",gpio);
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);

	read(fd, &value, 1);

	if(value == '0')
	{ 
		// Current GPIO status low
	}
	else
	{
		// Current GPIO status high
	}

	close(fd);
	return;
}

void WriteValue(int gpio, int val)
{
	char buf[MAX_BUF]; 
	int fd;

	//   printf("WriteValue  gpio %d val %d \n",gpio,val); 
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	// Set GPIO high status
	if(val == 1)
		write(fd, "1", 1); 
	// Set GPIO low status 
	else
		write(fd, "0", 1); 

	close(fd);
	return;
}


void PortOpen()
{
	char buf[MAX_BUF];
	//	 printf("WriteValue	gpio %d val %d \n",gpio,val); 
	sprintf(buf, "/sys/class/gpio/gpio%d/value", 6);
	fd_19 = open(buf, O_WRONLY);
	// Set GPIO high status



	return;
}
#endif

void PrintCntEx(int HitCnt, int MissCnt,int IgnoreCnt, int TotalCnt)
{
	printf("\r\n---------------------------------------------");
	printf("\r\nHIT Cnt : %d   MISS Cnt : %d   IgnoreCnt : %d  TOTAL Cnt : %d",HitCnt,MissCnt,IgnoreCnt,TotalCnt);
	//	gTESTAllCnt++;
	if(MissCnt == 0)
		printf("\r\n PASS");
	else
	{
		printf("\r\n FAIL");
		//			gTESTAllErrorCnt++;
	}
	printf("\r\n---------------------------------------------");
	return;
}


int gPrintMode;


#define LOGI(...)	printf(__VA_ARGS__)
#define LOGE(...)	printf(__VA_ARGS__)
#define SPI0_SPEED 0
#define SPI1_SPEED 500*1000

void Pon()
{
	Export(POWER);
	Direction(POWER,1);
	WriteValue(POWER,0);

	UnExport(CS0);
	UnExport(CS1);

	Export(GPIO05);
	Direction(GPIO05, 1);
	WriteValue(GPIO05, 0);

	Export(INT0);
	Direction(INT0, 1);
	WriteValue(INT0, 0);

	Export(CS0);
	Direction(CS0, 1);
	WriteValue(CS1, 1);

	Export(CS1);
	Direction(CS1, 1);
	WriteValue(CS1, 1);


	Dorca3SpiInit(SPI0_SPEED);

	printf("\r\n Power On");
	return;
}

void Poff()
{

	Export(POWER);
	Direction(POWER,1);
	WriteValue(POWER,1);

	Dorca3Close();

	Export(CS0);
	Export(CS1);
	Export(GPIO05);

	Direction(CS0, 1);
	WriteValue(CS0, 0);

	Direction(CS1, 1);
	WriteValue(CS1, 0);

	Direction(GPIO05, 1);
	WriteValue(GPIO05, 0);
	printf("\r\n Power Off");
	return;
}

void PowerOn()
{
#ifdef DEMO_BOARD

	Export(POWER);
	Export(INT0); 		
	Export(8);
	Export(7);			
	Export(9);
	Export(10);
	Export(11);

	printf("Export");
	Direction(POWER,1);
	Direction(INT0,1);			
	Direction(8,1); 			
	Direction(7,1); 
	Direction(9,1); 			
	Direction(10,1); 
	Direction(11,1); 
	printf("\r\n Clear ALL");
	WriteValue(POWER,0);
	WriteValue(INT0,0);			
	WriteValue(7,0); 			
	WriteValue(8,0); 			
	WriteValue(9,0); 			
	WriteValue(10,0); 			
	WriteValue(11,0); 			
	usleep(300*1000);
	printf("\r\n Power On");
	WriteValue(POWER,1);;
	usleep(300*1000);

	printf("\r\n Clear ALL");
	WriteValue(POWER,0);
	WriteValue(INT0,0);			
	WriteValue(8,0); 			
	WriteValue(7,0); 			
	usleep(300*1000);
	printf("\r\n Power On");
	WriteValue(POWER,1);;
#else
	Pon();
	usleep(600*1000);
	Poff();
	usleep(600*1000);
	Pon();
#endif	
	return;
}

void GenINT0()
{
#ifndef ETRY_CODE
	int j = 0;
	write(fd_19, "1", 1); 
	write(fd_19, "0", 1);

#endif
	return;

}

void GenINT1SEC()
{
#ifndef ETRY_CODE
	int j = 0;
	WriteValue(19,1);
	sleep(1);
	for(j = 0; j < 1; j++);;

	WriteValue(19,0);
#endif
	return;

}


extern int SPEED;

int main(void)
{
	int i;
	int j;
	unsigned char temp ;
	unsigned char crc[2];
	int HitCnt,MissCnt,IgnoreCnt;
#ifndef ETRY_CODE		
	PowerOn();
	printf("PowerOn();");	
	PortOpen();
	printf("PortOpen()");
	Dorca3SpiInit(SPEED);
#endif
	printf("\r\n WAKE UP DORCA !!!!");
	Delay_us(1);
	Delay_ms(100);
	delay_ms(10);
	//OKA_COMMON_KEY_PRINT();


#ifndef ETRY_CODE
	SetSPI0();
#endif
	WakeUp();
	printf("\r\n ****************  CALLING WAKE UP ******************");
	//	SHA_TEST_MAIN();
	//	return;
	//	printf("test interver start");
	//	delay_ms(5000);
	//	printf("test interver end");

	while(1)
	{
L_START:   
		temp = 'z' ;

		printf("\r\n\n");
		printf("\r\n  *****************************************************");
		printf("\r\n  *                 TEST MAIN                         *");
		printf("\r\n  *****************************************************");
		printf("\r\n  -----------------------------------------------------");
		printf("\r\n  * s. SPI SPEED SET                                  *");	
		printf("\r\n  * O. API TEST MODE                                 *");		
		//		printf("\r\n  * y. RSSHARead  :  ");					
		//		printf("\r\n  * t. RSCreate  :  ");					
		//		printf("\r\n  * 1. RSDir READ                         *");
		printf("\r\n  * Select : ");
		MissCnt = 0;
		HitCnt = 0;
		IgnoreCnt = 0;
		while(temp == 'z')
		{
			temp = _uart_get_char();
			if(temp == 0x0d)
				goto L_START;
			printf("\r\n input%02x\n", temp);
			if ( temp != 'z' ) printf("%c\n", temp);
			printf("\r\n");

			switch ( temp )
			{
			case 's' : SpiSpeedSet(); break;
			case 'O': API_TEST_MAIN();break;
				break;
			}
		}
	}


	printf("\n\nFinished the Test\n");

	return 0;
}
