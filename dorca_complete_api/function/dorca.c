#include <bcm2835.h>
#include <stdint.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#define LOG_TAG1		"JNI_SPI_NEOWINE"
#define LOGI(...)	printf(__VA_ARGS__)
#define LOGE(...)	printf(__VA_ARGS__)
//<==
#define WHEREAMI() printf("%s %d\n",__FILE__,__LINE__)
#include "dorca.h"



struct spi_ioc_transfer xfer[2];
struct spi_ioc_transfer xferCM0[2];

char* value;
unsigned char  buf[1024];



/*
*
Name
Spi_init ()Initialize sip_device for Neowine application
Synopsis
#include < dorca.h>
int spi_init();
Description
The spi_init () function used to initialize spi_device on AP's interface.
Return Value
The spi_init () function returns a OK or NotOK
* */

//char* filePath ="/mnt/sdcard/";
/*
int create_file()
{
int open_flags = O_RDWR | O_CREAT | O_DIRECT | O_SYNC;
//LOGI("filePath: %s\n", filePath);
fd = open(filePath, open_flags, 0766);
if(fd<0){
LOGE("Create errno: %s\n", strerror(errno));
return -1;
}
//LOGI("Neowine create OK , fd=%d \n", fd);
return fd;

}
*/
extern int fd0;
extern int fd1;
void Dorca3SpiInit( int com_speed )
{
	if (!bcm2835_init())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
		return ;
	}
	if (!bcm2835_spi_begin())
	{
		printf("bcm2835_spi_begin failed. Are you running as root??\n");
		return ;
	}
#if 0
	printf("\r\n  * 0. 1.9MHz											 *");
	printf("\r\n  * 1. 3.9MHz											 *");
	printf("\r\n  * 2. 7.8MHz											 *");
	printf("\r\n  * 3. 15.6MHz 										  *");
	printf("\r\n  * 4. 31.25MHz										   *");
	printf("\r\n  * 5. 62.5MHz 										  *");
#endif	 

	printf("\r\n Dorca3SpiInit");	 
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);	   // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);				   // The default
	switch( com_speed)
	{
	case 0:
		//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128    ); // 15.2Mhz
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128 ); // 15.2Mhz
		printf("\r\n  * 0. 1.9MHz											 *");	 	
		break;
	case 1:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64    ); // 15.2Mhz
		printf("\r\n  * 1. 3.9MHz											 *");	 	
		break;
	case 2:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32     ); // 15.2Mhz
		printf("\r\n  * 2. 7.8MHz											 *");	 	
		break;
	case 3:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16    ); // 15.2Mhz
		printf("\r\n  * 3. 15.6MHz 										  *");	 	
		break;
	case 4:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8   ); // 15.2Mhz
		printf("\r\n  * 4. 31.25MHz										   *");	 	
		break;		
	case 5:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4    ); // 15.2Mhz
		printf("\r\n  * 5. 62.5MHz 										  *");	 	
		break;		
	case 6:
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048    ); // 15.2Mhz
		printf("\r\n  * 6. 122Khz 										  *");
		break;				
	default:
		printf("\r\n SPI0 com_speed error");
		break;
	}
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);					   // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);	   // the default
	return;

}



void Dorca3Cm0Close()
{
	bcm2835_spi_end();
	bcm2835_close();
	printf("\r\n Dorca3Cm0Close");
	return;

}
void Dorca3Close()
{
	bcm2835_spi_end();
	bcm2835_close();
	printf("\r\n Dorca3Close");
	return;
}

void  Dorca3Cm0SpiInit( int com_speed )
{
	if (!bcm2835_init())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
		return ;
	}
	if (!bcm2835_spi_begin())
	{
		printf("bcm2835_spi_begin failed. Are you running as root??\n");
		return ;
	}
	printf("\r\n Dorca3Cm0SpiInit");
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);	   // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);				   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256  ); // 976khz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);					   // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);	   // the default
	return;

}

void SendData(unsigned char *buffer,int size)
{
	int status, i;
	memset(buf, 0, sizeof buf);



	for (i=0; i< size ; i++)
		buf[i] = buffer[i];


	bcm2835_spi_transfern(&buf[0], size);
	return;

	//--- Stop

}

void ReadData(unsigned char *tx_buffer,unsigned char *rx_buffer, int size)
{
	int status, i;
	memset(buf, 0, sizeof buf);


	for (i=0; i< 5 ; i++)
		buf[i] = tx_buffer[i];

	bcm2835_spi_transfern(&buf[0], 5);
	usleep(50*1000);
	bcm2835_spi_transfern(&rx_buffer[0], size);
	return;

	//--- Stop

}

void SpiRead (int fd,int inst, int addr, int nbytes, unsigned char *rx_data)
{
	int status, i;

	memset(buf, 0, sizeof buf);


	buf[0] = (char)inst;
	buf[1] = (addr>>8) & 0xFF;
	buf[2] = addr & 0xFF;
	buf[3] = 0xFF;

	// 1 instruction
	// 2 addr MSB
	// 3 addr LSB
	// 4 dummy

	bcm2835_spi_transfern(&buf[0], nbytes+4);			//data_buffer used for tx and rx    printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02X.\n", send_data, read_data);
	memcpy(rx_data,&buf[4],nbytes);
	if (status < 0)
	{
		LOGE("SPI_IOC_MESSAGE SpiRead");
		return;
	}
	return;
}


void SpiWrite(int fd,int inst, int addr, int nbytes, char *value)
{

	int status, i;

	memset(buf, 0, sizeof buf);

	buf[0] = (char)inst;
	buf[1] = (addr>>8) & 0xFF;
	buf[2] = addr & 0xFF;
	buf[3] = 0xFF;

	// 1 instruction
	// 2 addr MSB
	// 3 addr LSB
	// 4 dummy
	// 5 ~ N byte payload
	// 6 dummy
	for (i=0; i< nbytes ; i++)
		buf[i + 4] = value[i];

	buf[nbytes+4] = 0xFF;

	bcm2835_spi_transfern(&buf[0], nbytes+5);
	return;

}

