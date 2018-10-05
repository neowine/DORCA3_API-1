#include <stdio.h>
#include <string.h>

#include "dorca30_api.h"
#include "dorca30_function.h"
#include "dorca30_api_inside.h"
#include "./include/aes.h"
#include "global.h"


extern int cs;
void CommonDecEncWirteRead(unsigned char PageInfo, int AES_CTRL)
{	

	int i;
	int j;
	unsigned int inst = 0;
	int pass = 1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char temp ;
	unsigned char buf_data[64];
	unsigned char data[32];
	memset(data,0xaa,32);

	tx_data[0] = 0x00;// AES_x1 KEY
	tspi_interface(cs, ADDR_NOR_W,  RG_EE_KEY_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	//tx_data[0] = 0x01;//AES_256
	//printf("\r\n AES CTRL %d",AES_CTRL);
	tx_data[0] =  AES_CTRL;
	tspi_interface(cs, ADDR_NOR_W,  RG_AES_CTRL       , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x9;//SYMCIP MODE
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE    , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x4;//AESEncWrite Mode
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = PageInfo;//USER_ZONE_M01
	tspi_interface(cs, ADDR_NOR_W, RG_EE_USER_ZONE_SEL    , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 3;// AES_KeySetup
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE     , 0, 0, 0, 0, tx_data, rx_data, 1);
	delay_us(30);
	tx_data[0] = 1;//STAND BLYE
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	return;

}

void EncodeAndRead(unsigned char *Key,int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT)
{

	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char keybuffer[32];
	int i,j;
	if(Key != NULL) {
		if(Algorithm == AES256 || Algorithm == ARIA256){
			memcpy(keybuffer,Key+16,16);
			memcpy(keybuffer+16,Key,16);
		}
	else{
		memcpy(keybuffer+16,Key,16);
		}
		KeySetup(keybuffer);
	}
	
	CommonDecEncWirteRead(  (SubFrame << 6) | (SubPage <<4 )  | Page,Algorithm);
	tx_data[0] = 0xF;
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	delay_us(30);
	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 64);
	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF320      , 0, 0, 0, 0, tx_data, rx_data, 16);
	j = 15;
	for( i = 0; i < 16; i++)
		CT[i] = rx_data[j--];
	tx_data[0] = 0x1;
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	//delay_ms(9);
	tx_data[0] = 0x1;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	//delay_ms(9);
	EndOperation();	

	return;
}

void DecodeAndWrite(unsigned char *Key, int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT)
{
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char keybuffer[32];
	int i,j;
	if(Key != NULL) {
		if(Algorithm == AES256 || Algorithm == ARIA256){
			memcpy(keybuffer,Key+16,16);
			memcpy(keybuffer+16,Key,16);
		}
	else{
		memcpy(keybuffer+16,Key,16);
		}
		KeySetup(keybuffer);
	}
	
	
	CommonDecEncWirteRead(  (SubFrame << 6) | (SubPage <<4 )  | Page , Algorithm);//AES_256);
	tx_data[0] = 0xE;
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	reversebuffer(tx_data, CT, 16);
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400      , 0, 0, 0, 0, tx_data, rx_data, 16);
	delay_ms(9);
	tx_data[0] = 0x1;
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	//delay_ms(9);
	tx_data[0] = 0x1;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	//delay_ms(9);
	EndOperation();
	return;
}

