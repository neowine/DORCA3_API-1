#include <string.h>
#include <stdio.h> 
#include <stdlib.h> 

#include "dorca30_api.h"
#include "dorca30_api_inside.h"
#include "dorca30_function.h"
#include "global.h"
#include "./include/aes.h"

extern unsigned char gSUPER_PW_PT[16];
void printbyte(unsigned char *pData,int size )
{
	int i = 0;
	printf("\r\n");
	for(i = 0; i < size; i++)
	{
		if( i % 4 == 0)
			PRINTLOG("|");
		PRINTLOG("%02x,",pData[i]);
	}
	return;

}
void PrintBuffer(int type, unsigned char *data, unsigned char *addr)
{
	int byte_num = 64;
	int i = 0;
	printf("\r\n ---------------------------------------------");
	printf("\r\n addr = 0x%02x%02x", addr[0],addr[1]);
	if ( type == TYPE_TX) {
		printf("\r\n spi_tx_data    :"); for ( i=0; i<byte_num; i++ ){ if ( ( i !=0 ) & ( i % 16 == 0 ) ) printf("\n                 "); printf(" 0x%02x", data[i]); } }
	else {
		printf("\r\n spi_rx_data    :"); for ( i=0; i<byte_num; i++ ){ if ( ( i !=0 ) & ( i % 16 == 0 ) ) printf("\n                 "); printf(" 0x%02x", data[i]); } }
	return;		

}
int GetPermissionByPW(unsigned char * SUPER_PW_CT, int TYPE,int index)
{
	int i;
	int result = 1;
	unsigned int inst = 0;
	int PERM_TYPE = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	int j = 0;
	unsigned char temp_addr[2];
	int k = 0;
	int success = 0;
	for(i = 0; i < 64; i++)
	{
		tx_data[i] = 0;
		rx_data[i] = 0;
	}
	
	//SetKEYNormal();
	//printf("\r\n SetKeyNormal Write 0x11");
	tx_data[0] = index;
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x03;	
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = TYPE;	
	tspi_interface(cs, ADDR_NOR_W, RG_PERM_GET_CTRL , 0, 0, 0, 0, tx_data, rx_data, 1);	

	tx_data[0] = 0x0C;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);		
	tx_data[0] = 0x00;	
	tspi_interface(cs, ADDR_NOR_W, RG_PERM_GET_EE_RD_PRE_SP , 0, 0, 0, 0, tx_data, rx_data, 1);		
	delay_ms(16);
	tx_data[0] = 0x03;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);	

	delay_us(30);

	tx_data[0] = 0x01;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);	


	tx_data[0] = 0x04;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);	

	j = 15;
	for(i = 0 ; i < 16; i++)
	{
		tx_data[i] = SUPER_PW_CT[j--];	

	}
	PRINTLOG("\r\n !!!!!!! PASS WD  !!!!!!!");
	printbyte(tx_data,16);

	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400 , 0, 0, 0, 0, tx_data, rx_data, 16);	

	tx_data[0] = 0x01;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);	

	delay_ms(16);

	rx_data[0] = 0;	
	tspi_interface(cs, ADDR_NOR_R, RG_PERM_GET_CTRL1 , 0, 0, 0, 0, tx_data, rx_data, 1);		
	//  PRINTLOG("PERM_TYPE %d",TYPE);
	switch (TYPE) 
	{

	case RG_PERM_SUPER_PASS:
		PERM_TYPE = 1 << 5;
		temp_addr[0] = ADDR_SUPER_PW_CNT[0];
		temp_addr[1] = ADDR_SUPER_PW_CNT[1];
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON 	
			PRINTLOG("\r\nMISS TO GET SUPER_PERMISSION");
#endif
		}
		else{
#if TEST_COUNT_ON  	
			PRINTLOG("\r\n**HIT TO GET SUPER_PERMISSION");
			PRINTLOG("\r\n rx_data %02x  %02x",rx_data[0],PERM_TYPE);
#endif
		}
		break;
	case RG_PERM_DETOUR_PASS:
		PERM_TYPE = 1 << 4;
		temp_addr[0] = ADDR_DETOUR_PW_CNT[0];
		temp_addr[1] = ADDR_DETOUR_PW_CNT[1];
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON			
			PRINTLOG("\r\nMISS TO GET DETOUR_PERMISSION");
#endif
		}
		else
		{
#if TEST_COUNT_ON	       
			PRINTLOG("\r\nHIT TO GET DETOUR_PERMISSION");
#endif
		}

		break;

	case RG_PERM_DESTORY0_PASS:
		PERM_TYPE = 1 << 3;
		temp_addr[0] = ADDR_DESTORY0_PW_CNT[0];
		temp_addr[1] = ADDR_DESTORY0_PW_CNT[1];
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON			
			PRINTLOG("\r\nMISS TO GET DESTORY0_PERMISSION");
#endif		
		}
		else
		{
#if TEST_COUNT_ON	       
			PRINTLOG("\r\nHIT TO GET DESTORY0_PERMISSION");
#endif
		}

		break;


	case RG_PERM_DESTORY1_PASS:
		PERM_TYPE = 1 << 2;		
		temp_addr[0] = ADDR_DESTORY1_PW_CNT[0];
		temp_addr[1] = ADDR_DESTORY1_PW_CNT[1];
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON			
			PRINTLOG("\r\nMISS TO GET DESTORY1_PERMISSION");
#endif
		}
		else
		{
#if TEST_COUNT_ON	       
			PRINTLOG("\r\nHIT TO GET DESTORY1_PERMISSION");
#endif
		}

		break;

	case RG_PERM_EEPROM_PASS:
		PERM_TYPE = 1 << 1;				
		temp_addr[0] = ADDR_EEPROM_PW_CNT[0];
		temp_addr[1] = ADDR_EEPROM_PW_CNT[1];
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON			
			PRINTLOG("\r\nMISS TO GET EEPROM_PERMISSION");
#endif
		}
		else
		{
#if TEST_COUNT_ON	       
			PRINTLOG("\r\nHIT TO GET EEPROM_PERMISSION");
#endif
		}
		break;

	case RG_PERM_UID_PASS:
		PERM_TYPE = 1 ;					
		temp_addr[0] = ADDR_UID_PW_CNT[0];
		temp_addr[1] = ADDR_UID_PW_CNT[1];		
		if( (rx_data[0] & PERM_TYPE) == 0)
		{
			result = 0;
#if TEST_COUNT_ON			
			PRINTLOG("\r\nMISS TO GET UID_PERMISSION");
#endif
		}
		else
		{
#if TEST_COUNT_ON
			PRINTLOG("\r\nHIT TO GET UID_PERMISSION");
#endif
		}
		break;

	default:
		PRINTLOG("\r\nPERM TYPE ERROR %d",PERM_TYPE);
		break;

	}

	PRINTLOG("\r\n GetPermResult 0x%02x",rx_data[0]);


	inst = 0x20; 	 

	tspi_interface(cs, inst, temp_addr , 0, 0, 0, 0, tx_data, rx_data, 1);		



	switch (TYPE) 
	{

	case RG_PERM_SUPER_PASS:

		if(rx_data[0] != 0)
		{
			result = 0;
		}
		break;
	case RG_PERM_DETOUR_PASS:
		if(rx_data[0] != 0)
		{
			result = 0;
		}
		break;
	case RG_PERM_DESTORY0_PASS:
		if(rx_data[0] != 0)
		{
			result = 0;
		}
		break;
	case RG_PERM_DESTORY1_PASS:
		if(rx_data[0] != 0)
		{
			result = 0;

		}
		break;
	case RG_PERM_EEPROM_PASS:
		if(rx_data[0] != 0)
		{
			result = 0;
		}
		break;
	case RG_PERM_UID_PASS:
		if(rx_data[0] != 0)
		{
			result = 0;
		}
		break;
	default:
		PRINTLOG("\r\nPERM TYPE ERROR");
		break;

	}    

	tx_data[0] = 0x01;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);			
	tx_data[0] = 0;	
	tspi_interface(cs, ADDR_NOR_W, RG_ACCESS , 0, 0, 0, 0, tx_data, rx_data, 1);			
	delay_us(5);
	return result;

}

int GetPermission(int iType, unsigned char *pchPW,int index)
{
	unsigned char achCTPW[16];
	
	AES_KEY aes128_ekey,aes128_dkey;	
	AES_set_encrypt_key(gAES_KEY_X1, 128, &aes128_ekey);
	AES_ecb_encrypt(pchPW, achCTPW, &aes128_ekey, AES_ENCRYPT);

	
	return GetPermissionByPW(achCTPW, iType,index);
}

int ReleasePermision()
{
	int success = 1;
	int i;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	int j = 0;
	unsigned char temp_addr[2];
	int k = 0;

	delay_us(5);
	EndOperation();
	tx_data[0] = 0x0C;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE , 0, 0, 0, 0, tx_data, rx_data, 1);			
	tx_data[0] = 0x0;	
	tspi_interface(cs, ADDR_NOR_W, RG_PERM_RELEASE , 0, 0, 0, 0, tx_data, rx_data, 1);
	EndOperation();
	delay_us(5);
	tx_data[0] = 0;	
	tspi_interface(cs, ADDR_NOR_R, RG_PERM_GET_CTRL1 , 0, 0, 0, 0, tx_data, rx_data, 1);			

	if(rx_data[0] != 0 )
	{
		PRINTLOG("\r\n MISS TO RELEASE PERMISSION 0x%02x", rx_data[0]);
		success = 0;

	}
	else
	{
		PRINTLOG("\r\n HIT TO RELEASE PERMISSION");

	}

	return success;

}

void CommonReadWriteConfig(int type, unsigned char* pAddr)
{

	unsigned char tx_data[64];
	unsigned char rx_data[64];
//	SetAddrbyType(type,addr);	

		switch(type)
		{
		case A_EE_CONFIG_NW:
			pAddr[0] = 0xEB;
			pAddr[1] = 0x40; 		
			break;
		case A_EE_CONFIG_FAC:
			pAddr[0] = 0xEB;
			pAddr[1] = 0x80; 				
			break;
		case A_EE_CONFIG_UID:
			pAddr[0] = 0xEB;
			pAddr[1] = 0xc0; 				
			break;
		case A_EE_SEED_KEY:
			pAddr[0] = 0xEC;
			pAddr[1] = 0;
			break;
		case A_EE_CONFIG_USER:
			pAddr[0] = 0xEC;
			pAddr[1] = 0x40; 				
			break;
		case A_EE_CONFIG_LOCK:
			pAddr[0] = 0xEC;
			pAddr[1] = 0x80; 				
			break;
		case A_EE_MEM_TEST:
			pAddr[0] = 0xEC;
			pAddr[1] = 0xc0; 				
			break;
		case A_EE_MIDR:
			pAddr[0] = 0xED;
			pAddr[1] = 0; 				
			break;
		default:
			PRINTLOG("\r\n wrong config type ");
			break;
		}
	tx_data[0] = 0x7;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = pAddr[1];// 0x00
	tx_data[1] = pAddr[0];// 0xeb
	tspi_interface(cs, ADDR_NOR_W, RG_EET_BYOB_ADDR_LSB      , 0, 0, 0, 0, tx_data, rx_data, 2);
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W, RG_EE_CFG_RD_RG_EEBUF_ST      , 0, 0, 0, 0, tx_data, rx_data, 1);
	delay_ms(1);
	return;	
}
void SetConifgArea(int permType,int CfgName,unsigned char *pPW_PT, unsigned char *pBuffer, int length)
{
	unsigned char addr[4];
	unsigned char tx_data[64];
	unsigned char rx_data[64];	
	GetPermission(permType, pPW_PT,KEY_INDEX);
	CommonReadWriteConfig(CfgName,addr);
	tspi_interface(cs, 0x30, addr      , 0, 0, 0, 0, pBuffer, rx_data, length);
	delay_ms(8);
	EndOperation();
	ReleasePermision();
	return;	
}
void GetConfigArea(int permType,int CfgName,unsigned char *pPW_PT, unsigned char *pBuffer, int length)
{
	unsigned char tx_data[64];
	unsigned char rx_data[64];	
	unsigned char addr[2];	
	GetPermission(permType, pPW_PT,KEY_INDEX);	
	CommonReadWriteConfig(CfgName,addr);
	tspi_interface(cs, 0x20, addr      , 0, 0, 0, 0, tx_data, pBuffer, length);
	EndOperation();
	ReleasePermision();	
	return;	
}
void endOP(void)
{

	int i;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	int j = 0;
	Delay_ms(2);
#if PRINTFMODE
	//  PRINTLOG("\r\n=========================================================================");
	//  PRINTLOG("\r\n==       RG_ST0_OPMODE SET=> 0x01                              ==");
	// PRINTLOG("\r\n=========================================================================");
#endif	 
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE, 0, 0, 0, 0, tx_data, rx_data, 1);

#if PRINTFMODE
	// PRINTLOG("\r\n=========================================================================");
	//PRINTLOG("\r\n==       RG_ACCESS SET=> 0x00                              ==");
	// PRINTLOG("\r\n=========================================================================");
#endif	 
	tx_data[0] = 0x00;	
	tspi_interface(cs, ADDR_NOR_W, RG_ACCESS, 0, 0, 0, 0, tx_data, rx_data, 1);
	Delay_ms(2);
	delay_us(5);
	return;		
}


void MAKE_RANDOM_SEED_KEY()
{

	int i;
	int j;
	unsigned int inst = 0;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char addr[2];
	unsigned char buf0xxx[64];
	int TestSize =0 ;
	int success = 1;
	unsigned char msb = 0xe9;
	unsigned char lsb = 0x00;

	//	SetKEYNormal();

	while(1) {
	//GetPermissionByPW(UID_PW_CT, RG_PERM_UID_PASS);
	GetPermission(RG_PERM_UID_PASS,gUID_PW_PT,KEY_INDEX);

	tx_data[0] = 0x7;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE	  , 0, 0, 0, 0, tx_data, rx_data, 1);

	addr[0] = 0xEC;
	addr[1] = 0x00;
	tx_data[0] = addr[1];// 0x00
	tx_data[1] = addr[0];// 0xeb

	for( i = 0; i < 64; i++)
		buf0xxx[i] = rand() & 0xFF;

	tspi_interface(cs, ADDR_NOR_W, RG_EET_BYOB_ADDR_LSB 	 , 0, 0, 0, 0, tx_data, rx_data, 2);
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W, RG_EE_CFG_RD_RG_EEBUF_ST 	 , 0, 0, 0, 0, tx_data, rx_data, 1);
	delay_ms(1);

	tspi_interface(cs, 0x30, addr	   , 0, 0, 0, 0, buf0xxx, rx_data, 64);
	PrintBuffer(TYPE_TX,buf0xxx,addr);
	delay_ms(8);
	tspi_interface(cs, 0x20, addr	   , 0, 0, 0, 0, tx_data, rx_data, 64);
	PrintBuffer(TYPE_RX,rx_data,addr);
	delay_us(10);
	endOP();
	ReleasePermision();
	if(memcmp(buf0xxx,rx_data,32) == 0)
		printf("\r\n PASS");
	else{
		printf("\r\n FAIL");
		exit(0);
		}
	}
	return;		
}

