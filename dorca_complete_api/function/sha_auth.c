#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "dorca30_api.h"
#include "dorca30_api_inside.h"
#include "dorca30_function.h"
#include "./include/aes.h"
#include "global.h"
#include "dorca.h"
#include "miracl.h"
extern unsigned char gSUPER_PW_PT[16];
void MCU_SHA256_EXE(unsigned char *txdata, unsigned char *exdata, unsigned int ByteNo)
{
	int i = 0;
	shs256_init_mcu_frm();

	//printf("\r\n sha_in :"); for ( i=0; i<64; i++ ) { if ( i%32 == 0 ) printf("\r\n      "); printf(" %02X", txdata[i]); }
	for (i=0;i<ByteNo;i++) shs256_process_mcu_frm( txdata[i]);

	shs256_hash_mcu_frm( exdata);  


}
int SHAAUTH_FROM_DORCA()
{
	unsigned char GID[4];
	unsigned char AuthRand[16];
	unsigned char GID_EEPROM_DATA[64];
	unsigned char SHA1_INPUT[20];
	unsigned char SHA1_OUT[32];
	unsigned char AuthMsgDevice[32];
	unsigned char AuthMsgMCU[32];
	int i = 0;
	int j = 0;
	unsigned int inst = 0;
	int success =  1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char BKUP_ADDR = 0;
	int iResult = 0;
	unsigned char addr[2] = {0xeb,0x40} ;
	GetPermission(RG_PERM_SUPER_PASS,gSUPER_PW_PT,KEY_INDEX);
	tx_data[0] = 0x7;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tspi_interface(cs, 0x20, addr      , 0, 0, 0, 0, tx_data, rx_data, 64);
	//PrintBuffer(TYPE_RX,rx_data,addr);
	delay_us(10);
	ReleasePermision();

	GID[0] = rx_data[3];//GID_EEPROM_DATA[3];
	GID[1] = rx_data[2];//GID_EEPROM_DATA[2];
	GID[2] = rx_data[1];//GID_EEPROM_DATA[1];
	GID[3] = rx_data[0];//GID_EEPROM_DATA[0];	

	tx_data[0] = 0x09;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE       , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x0a;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_SHAAUTH_CTRL        , 0, 0, 0, 0, tx_data, rx_data, 1);


	tx_data[0] = 0x04;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_RND_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);
	Delay_us(10);

	tspi_interface(cs, ADDR_NOR_R, RG_ACCESS        , 0, 0, 0, 0, tx_data, rx_data, 1);
	Delay_us(50);


	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 64);

	j = 31;
	for(i = 0; i < 32;i++)
		AuthMsgDevice[i] = rx_data[j--];
	j = 47;
	for(i = 0; i < 16;i++)
		AuthRand[i] = rx_data[j--];
	
	for( i = 0; i < 4 ; i++)
		SHA1_INPUT[i] = GID[i];
	j = 0;
	for( i = 4; i < 20 ; i++)
		SHA1_INPUT[i] = AuthRand[j++];	
	
	MCU_SHA256_EXE(SHA1_INPUT,SHA1_OUT,20);

	MCU_SHA256_EXE(SHA1_OUT,AuthMsgMCU,256/8);

	if( memcmp(AuthMsgDevice,AuthMsgMCU,256/8) == 0)
		printf("\r\n SHA_AUTH  SHAAUTH_FROM_DORCA PASS");
	else
	{
		printf("\r\n SHA_AUTH  SHAAUTH_FROM_DORCA  FAIL");
		success = 0;
	}


	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_RND_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x03;
	tspi_interface(cs, ADDR_NOR_W, RG_SHAAUTH_CTRL        , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W, RG_ACCESS        , 0, 0, 0, 0, tx_data, rx_data, 1);


	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W,RG_ST0_OPMODE         , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W,RG_ACCESS         , 0, 0, 0, 0, tx_data, rx_data, 1);
	return success;
}

int SHAAUTH_FROM_MCU()
{
	unsigned char GID[4] = {0x01,0x02,0x03,0x04};
	unsigned char AuthText[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	unsigned char GID_EEPROM_DATA[64];
	unsigned char SHA1_INPUT[20];
	unsigned char SHA1_OUT[32];

	unsigned char AuthMsgMCU[32];

	unsigned char Input[3] = {'a','b','c'};	
	int i = 0;
	int j = 0;
	unsigned int inst = 0;
	int success =  1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char BKUP_ADDR = 0;
	int iResult = 0;
	unsigned char addr[2] = {0xeb,0x40} ;
	
	memset(tx_data,0,64);
	memset(rx_data,0,64);
		GetPermission(RG_PERM_SUPER_PASS,gSUPER_PW_PT,KEY_INDEX);
	tx_data[0] = 0x7;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tspi_interface(cs, 0x20, addr      , 0, 0, 0, 0, tx_data, rx_data, 64);
	//PrintBuffer(TYPE_RX,rx_data,addr);
	delay_us(10);
	ReleasePermision();

	GID[0] = rx_data[3];//GID_EEPROM_DATA[3];
	GID[1] = rx_data[2];//GID_EEPROM_DATA[2];
	GID[2] = rx_data[1];//GID_EEPROM_DATA[1];
	GID[3] = rx_data[0];//GID_EEPROM_DATA[0];	


	memcpy(SHA1_INPUT,GID,4);
	memcpy(&SHA1_INPUT[4],AuthText,16);	

	MCU_SHA256_EXE(SHA1_INPUT,SHA1_OUT,20);
	//printf("\r\n SHA1_OUT \r\n");
	//printbyte(SHA1_OUT,32);


	MCU_SHA256_EXE(SHA1_OUT,AuthMsgMCU,256/8);
	tx_data[0] = 0x09;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE       , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W, RG_SHAAUTH_CTRL       , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x0a;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);

	j = 15;
	for(i = 16; i < 32; i++)
	{
		tx_data[i] = AuthText[j--];
	}
	j = 31;
	for(i = 32; i < 64; i++)
	{
		tx_data[i] = AuthMsgMCU[j--];// ^ 0xFF;
	}

	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 64);
	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_R, RG_ACCESS        , 0, 0, 0, 0, tx_data, rx_data, 1);
	Delay_us(50);

	tspi_interface(cs, ADDR_NOR_R,RG_MCUAuthResult        , 0, 0, 0, 0, tx_data, rx_data, 1);
	if(rx_data[0] == 0 )
	{
		printf("\r\n SHA AUTH SHAAUTH_FROM_MCU FAIL");
		success = 0;
	}
	else
	{
		printf("\r\n SHA AUTH SHAAUTH_FROM_MCU PASS");

	}
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE        , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W,RG_ST0_OPMODE         , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W,RG_ACCESS         , 0, 0, 0, 0, tx_data, rx_data, 1);
	return success;
}
