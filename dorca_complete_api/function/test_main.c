#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "dorca30_api.h"
#include "dorca30_api_inside.h"
#include "dorca30_function.h"
#include "./include/aes.h"
#include "global.h"
#include "dorca.h"
#define DALY_SWICHING 20
#define SPI0_SPEED 0
#define SPI1_SPEED 500*1000
#define TWO_LEN 64
 
extern int cs;
int SPEED = 0;

unsigned char gAES_KEY_X3[32] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13};
unsigned char gAES_KEY_X2[32] = {0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12};
unsigned char gAES_KEY_X1[32] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
unsigned char gAES_KEY_X0[32]=  {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
unsigned char gAES_KEY_SEED[32]={0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};


unsigned char gSUPER_PW_PT[16];

unsigned char gEEPROM_PW_PT[16];
unsigned char gUID_PW_PT[16];//cc			
void PWWrite()
{
	int i = 0;
	for(i = 0; i < 16;i++)
	{
		gSUPER_PW_PT[i] = 0x77; //77

		gEEPROM_PW_PT[i] = 0xbb;//bb
		gUID_PW_PT[i] = 0xcc;//cc 	
	}
	return;	
}
void GetPermissionMenu()
{
	unsigned char temp = 0 ;
	unsigned char out_rand[16];
	int i = 0;
	int iResult = 0;

	SHA_Test_Main_START:
	while(1)
	{
			temp = 'z' ;		
	printf("\r\n  * 1. SUPER_PW_PT				");	
	printf("\r\n  * 2. DESTROY0_PW_PT					  		");
	printf("\r\n  * 3. DESTROY1_PW_PT					  		");			
	printf("\r\n  * 4. EEPROM_PW_PT						");			
	printf("\r\n  * 5. UID_PW_PT						");						
	printf("\r\n  * 7. Release Permission						");								
		
	/*
	RG_PERM_SUPER_PASS = 5,
	RG_PERM_DETOUR_PASS = 4,
	RG_PERM_DESTORY0_PASS = 3,
	RG_PERM_DESTORY1_PASS= 2,
	RG_PERM_EEPROM_PASS= 1,
	RG_PERM_UID_PASS = 0
	*/
	//	while(temp == 'z')
		{
			int HitCnt = 0;
			int MissCnt = 0;
			temp = _uart_get_char();
		
			if ( temp != 'z' ) printf("%c\n", temp);
			printf("\r\n");
			if(temp == 0x0d)
				goto SHA_Test_Main_START;
			if( temp == 'm')
				return;
		
			switch ( temp )
			{
				case '1':
					printf("\r\n 					GetPermission(RG_PERM_SUPER_PASS,SUPER_PW_PT);");
					PrintByte(gSUPER_PW_PT,16);
					GetPermission(RG_PERM_SUPER_PASS,gSUPER_PW_PT,KEY_INDEX);
					break;
				case '4':
					PrintByte(gEEPROM_PW_PT,16);					
					printf("\r\n 					GetPermission(RG_PERM_EEPROM_PASS,EEPROM_PW_PT);");					
					GetPermission(RG_PERM_EEPROM_PASS,gEEPROM_PW_PT,KEY_INDEX);
					break;				
				case '5':
					PrintByte(gUID_PW_PT,16);						
					printf("\r\n 					GetPermission(RG_PERM_UID_PASS,UID_PW_PT);");					
					GetPermission(RG_PERM_UID_PASS,gUID_PW_PT,KEY_INDEX);
					break;				
				case '7':
						printf("\r\n  * 7. Release Permission						");								
						ReleasePermision();
					break;
				default:
					break;


			}

		}

		}
	return;			

}

unsigned char SpiSpeedSet(void)
{
	unsigned char temp = 0;
	while(1)
	{
		printf("\r\n\n");
		printf("\r\n  *****************************************************");
		printf("\r\n  *                 SpiSpeedSet                     *");
		printf("\r\n  *****************************************************");
		printf("\r\n  * 0. 1.9MHz                                           *");
		printf("\r\n  * 1. 3.9MHz                                           *");
		printf("\r\n  * 2. 7.8MHz                                           *");
		printf("\r\n  * 3. 15.6MHz                                           *");
		printf("\r\n  * 4. 31.25MHz                                           *");
		printf("\r\n  * 5. 62.5MHz                                           *");
		printf("\r\n  * 6. 62.5MHz                                           *");		
		printf("\r\n  * m return to top menu                                          *");	

		printf("\r\n  *****************************************************");
		{
			if(SPEED == 0)
				printf("\r\n  Current Speed. 1.9MHz                                           *");
			if(SPEED == 1)
				printf("\r\n  Current Speed. 3.9MHz                                           *");
			if(SPEED == 2)
				printf("\r\n  Current Speed. 7.8MHz                                           *");
			if(SPEED == 3)
				printf("\r\n  Current Speed. 15.6MHz                                           *");
			if(SPEED == 4)
				printf("\r\n  Current Speed. 32.2MHz                                           *");
			if(SPEED == 5)
				printf("\r\n  Current Speed. 62.5MHz                                           *");
			if(SPEED == 6)
				printf("\r\n  Current Speed. 122Khz");

		}
		printf("\r\n");
		printf("\r\n  * Select : ");


		temp = 'z' ;

		while(temp == 'z')
		{
			temp = _uart_get_char();
			if(temp == 0x0d)
				break;
			if(temp != 'z') printf("%c\n", temp);

			switch(temp)
			{
			case '0' : Dorca3Close();Dorca3SpiInit(0);SPEED = 0; break ;
			case '1' : Dorca3Close();Dorca3SpiInit(1);SPEED = 1; break ;
			case '2' : Dorca3Close();Dorca3SpiInit(2);SPEED = 2; break ;
			case '3' : Dorca3Close();Dorca3SpiInit(3);SPEED = 3; break ;
			case '4' : Dorca3Close();Dorca3SpiInit(4);SPEED = 4; break ;
			case '5' : Dorca3Close();Dorca3SpiInit(5);SPEED = 5; break ;			
			case '6' : Dorca3Close();Dorca3SpiInit(6);SPEED = 6; break ;			
			}
			if(temp == 'm')
				return 0;
		}

	}   
	return 0;
}

void KeyMakeTest()
{
	KeyCreatXn(0,MODE256,gAES_KEY_SEED ,gAES_KEY_X0);
	KeyCreatXn(1,MODE256,gAES_KEY_X0,gAES_KEY_X1);						
	KeyCreatXn(2,MODE256,gAES_KEY_X1,gAES_KEY_X2);
	KeyCreatXn(3,MODE256,gAES_KEY_X2,gAES_KEY_X3);		
	return;	
}
void AesAriaOperationModeTest32()
{
	//AES TEST
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char SOURCE[16*10];
	unsigned char RESULT[16*10];
	unsigned char IV[16];
	unsigned char KEY[32];	
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char CT[32];
	unsigned char PT[32];	
	unsigned char AES128KEY[16];
	unsigned char AES128CT[TWO_LEN];
	unsigned char AES128PT[TWO_LEN];
	unsigned char AES256KEY[32];
	unsigned char AES256CT[TWO_LEN];
	unsigned char AES256PT[TWO_LEN];

	unsigned char ARIA128KEY[16];
	unsigned char ARIA128CT[TWO_LEN];
	unsigned char ARIA128PT[TWO_LEN];
	unsigned char ARIA256KEY[32];
	unsigned char ARIA256CT[TWO_LEN];
	unsigned char ARIA256PT[TWO_LEN];

	unsigned char *pKEY;
	unsigned char *pPT;
	unsigned char *pCT;
	unsigned char KEYBUFFER[64];
	memset(KEYBUFFER,0,64);
#if 1	
	
	Hexstr2Bytes("000102030405060708090a0b0c0d0e0f",AES128KEY);
	Hexstr2Bytes("69c4e0d86a7b0430d8cdb78070b4c55a69c4e0d86a7b0430d8cdb78070b4c55a69c4e0d86a7b0430d8cdb78070b4c55a69c4e0d86a7b0430d8cdb78070b4c55a",AES128CT);	
	Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",AES128PT);		

	Hexstr2Bytes("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",AES256KEY);
	Hexstr2Bytes("8ea2b7ca516745bfeafc49904b4960898ea2b7ca516745bfeafc49904b4960898ea2b7ca516745bfeafc49904b4960898ea2b7ca516745bfeafc49904b496089",AES256CT);	
	Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",AES256PT);	

	printf("\r\n . AES MODE_ECB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AES128KEY,16,0,PT,AES128CT,32,MODE_ECB,0);
	if(memcmp(PT,AES128PT,32) == 0)
	   printf(" PASS");
	else {
		printf("\r\n PT");
		PrintByte(PT,32);
		printf("\r\n AES128PT");
		PrintByte(AES128PT,32);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,AES128CT+32,32,MODE_ECB,LAST);
	if(memcmp(PT,AES128PT+32,32) == 0)
	   printf(" PASS");
	else {
		printf("\r\n PT");
		PrintByte(PT,32);
		printf("\r\n AES128PT");
		PrintByte(AES128PT+32,32);	
	}
	

	
	printf("\r\n . AES MODE_ECB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AES128KEY,16,0,CT,AES128PT,32,MODE_ECB,0);
	if(memcmp(CT,AES128CT,32) == 0)
	   printf(" PASS");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,AES128PT+32,32,MODE_ECB,LAST);
	if(memcmp(CT,AES128CT+32,32) == 0)
	   printf(" PASS");	
	
	

	printf("\r\n . AES MODE_ECB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AES256KEY,32,0,CT,AES256PT,32,MODE_ECB,0);
	if(memcmp(CT,AES256CT,32) == 0)
	   printf(" PASS");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,AES256PT+32,32,MODE_ECB,LAST);
	if(memcmp(CT,AES256CT+32,32) == 0)
	   printf(" PASS");
	
	printf("\r\n . AES MODE_ECB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AES256KEY,32,0,PT,AES256CT,32,MODE_ECB,0);
	if(memcmp(PT,AES256PT,32) == 0)
	   printf(" PASS");
	else {
		printf("\r\n PT");
		PrintByte(PT,32);
		printf("\r\n AES256PT");
		PrintByte(AES256PT,32);	
	}	

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,AES256CT,32,MODE_ECB,LAST);
	if(memcmp(PT,AES256PT,32) == 0)
	   printf(" PASS");
	else {
		printf("\r\n PT");
		PrintByte(PT,32);
		printf("\r\n AES256PT");
		PrintByte(AES256PT,32);	
	}	


#endif
	{
	unsigned char AESCBC128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESCBC128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCBC128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCBC128CT[] = { 0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d,0x50,0x86,0xcb,0x9b,0x50,0x72,0x19,0xee,0x95,0xdb,0x11,0x3a,0x91,0x76,0x78,0xb2,0x73,0xbe,0xd6,0xb8,0xe3,0xc1,0x74,0x3b,0x71,0x16,0xe6,0x9e,0x22,0x22,0x95,0x16,0x3f,0xf1,0xca,0xa1,0x68,0x1f,0xac,0x09,0x12,0x0e,0xca,0x30,0x75,0x86,0xe1,0xa7};
	printf("\r\n . AES MODE_CBC 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCBC128KEY,16,AESCBC128IV,CT,AESCBC128PT,32,MODE_CBC,0);
	if(memcmp(CT,AESCBC128CT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,32);
	
		printf("\r\n AESCBC128CT");
		PrintByte(AESCBC128CT,32);	
	}


	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCBC128PT[16*2],32,MODE_CBC,LAST);
	if(memcmp(CT,&AESCBC128CT[16*2],32) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC128CT");
		PrintByte(&AESCBC128CT[16*2],16);	
	}
	

		
	printf("\r\n . AES MODE_CBC 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCBC128KEY,16,AESCBC128IV,PT,AESCBC128CT,32,MODE_CBC,0);
	if(memcmp(PT,AESCBC128PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(AESCBC128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCBC128CT[16*2],32,MODE_CBC,LAST);
	if(memcmp(PT,&AESCBC128PT[16*2],32) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(&AESCBC128PT[16*2],16);	
	}
	

}
	{
	unsigned char AESCBC256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESCBC256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCBC256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCBC256CT[] = { 0xf5,0x8c,0x4c,0x04,0xd6,0xe5,0xf1,0xba,0x77,0x9e,0xab,0xfb,0x5f,0x7b,0xfb,0xd6,0x9c,0xfc,0x4e,0x96,0x7e,0xdb,0x80,0x8d,0x67,0x9f,0x77,0x7b,0xc6,0x70,0x2c,0x7d,0x39,0xf2,0x33,0x69,0xa9,0xd9,0xba,0xcf,0xa5,0x30,0xe2,0x63,0x04,0x23,0x14,0x61,0xb2,0xeb,0x05,0xe2,0xc3,0x9b,0xe9,0xfc,0xda,0x6c,0x19,0x07,0x8c,0x6a,0x9d,0x1b};
	printf("\r\n . AES MODE_CBC 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCBC256KEY,32,AESCBC256IV,CT,AESCBC256PT,32,MODE_CBC,0);
	if(memcmp(CT,AESCBC256CT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(AESCBC256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCBC256PT[16*2],32,MODE_CBC,LAST);
	if(memcmp(CT,&AESCBC256CT[16*2],32) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(&AESCBC256CT[16*2],16);	
	}
	
		
	printf("\r\n . AES MODE_CBC 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCBC256KEY,32,AESCBC256IV,PT,AESCBC256CT,32,MODE_CBC,0);
	if(memcmp(PT,AESCBC256PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(AESCBC256PT,16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCBC256CT[16*2],32,MODE_CBC,LAST);
	if(memcmp(PT,&AESCBC256PT[16*2],32) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(&AESCBC256PT[16*2],16);	
	}

	

}

	{
	unsigned char AESCFB128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESCFB128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCFB128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCFB128CT[] = { 0x3b,0x3f,0xd9,0x2e,0xb7,0x2d,0xad,0x20,0x33,0x34,0x49,0xf8,0xe8,0x3c,0xfb,0x4a,0xc8,0xa6,0x45,0x37,0xa0,0xb3,0xa9,0x3f,0xcd,0xe3,0xcd,0xad,0x9f,0x1c,0xe5,0x8b,0x26,0x75,0x1f,0x67,0xa3,0xcb,0xb1,0x40,0xb1,0x80,0x8c,0xf1,0x87,0xa4,0xf4,0xdf,0xc0,0x4b,0x05,0x35,0x7c,0x5d,0x1c,0x0e,0xea,0xc4,0xc6,0x6f,0x9f,0xf7,0xf2,0xe6};
	printf("\r\n . AES MODE_CFB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCFB128KEY,16,AESCFB128IV,CT,AESCFB128PT,32,MODE_CFB,0);
	if(memcmp(CT,AESCFB128CT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(AESCFB128CT,16);	
	}

	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCFB128PT[16*2],32,MODE_CFB,LAST);
	if(memcmp(CT,&AESCFB128CT[16*2],32) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(&AESCFB128CT[16*2],16);	
	}
	
		
	printf("\r\n . AES MODE_CFB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCFB128KEY,16,AESCFB128IV,PT,AESCFB128CT,32,MODE_CFB,0);
	if(memcmp(PT,AESCFB128PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(AESCFB128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCFB128CT[16*2],32,MODE_CFB,LAST);
	if(memcmp(PT,&AESCFB128PT[16*2],32) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(&AESCFB128PT[16*2],16);	
	}

	

}
	{
	unsigned char AESCFB256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESCFB256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCFB256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCFB256CT[] = { 0xdc,0x7e,0x84,0xbf,0xda,0x79,0x16,0x4b,0x7e,0xcd,0x84,0x86,0x98,0x5d,0x38,0x60,0x39,0xff,0xed,0x14,0x3b,0x28,0xb1,0xc8,0x32,0x11,0x3c,0x63,0x31,0xe5,0x40,0x7b,0xdf,0x10,0x13,0x24,0x15,0xe5,0x4b,0x92,0xa1,0x3e,0xd0,0xa8,0x26,0x7a,0xe2,0xf9,0x75,0xa3,0x85,0x74,0x1a,0xb9,0xce,0xf8,0x20,0x31,0x62,0x3d,0x55,0xb1,0xe4,0x71};
	printf("\r\n . AES MODE_CFB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCFB256KEY,32,AESCFB256IV,CT,AESCFB256PT,32,MODE_CFB,0);
	if(memcmp(CT,AESCFB256CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(AESCFB256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCFB256PT[16*2],32,MODE_CFB,LAST);
	if(memcmp(CT,&AESCFB256CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(&AESCFB256CT[16*2],16);	
	}
	
		
	printf("\r\n . AES MODE_CFB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCFB256KEY,32,AESCFB256IV,PT,AESCFB256CT,32,MODE_CFB,0);
	if(memcmp(PT,AESCFB256PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(AESCFB256PT,16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCFB256CT[16*2],32,MODE_CFB,LAST);
	if(memcmp(PT,&AESCFB256PT[16*2],32) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(&AESCFB256PT[16*2],16);	
	}

	

}
	{
	unsigned char AESOFB128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESOFB128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESOFB128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESOFB128CT[] = { 0x3b,0x3f,0xd9,0x2e,0xb7,0x2d,0xad,0x20,0x33,0x34,0x49,0xf8,0xe8,0x3c,0xfb,0x4a,0x77,0x89,0x50,0x8d,0x16,0x91,0x8f,0x03,0xf5,0x3c,0x52,0xda,0xc5,0x4e,0xd8,0x25,0x97,0x40,0x05,0x1e,0x9c,0x5f,0xec,0xf6,0x43,0x44,0xf7,0xa8,0x22,0x60,0xed,0xcc,0x30,0x4c,0x65,0x28,0xf6,0x59,0xc7,0x78,0x66,0xa5,0x10,0xd9,0xc1,0xd6,0xae,0x5e};
	printf("\r\n . AES MODE_OFB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESOFB128KEY,16,AESOFB128IV,CT,AESOFB128PT,32,MODE_OFB,0);
	if(memcmp(CT,AESOFB128CT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(AESOFB128CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESOFB128PT[16*2],32,MODE_OFB,LAST);
	if(memcmp(CT,&AESOFB128CT[16*2],32) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(&AESOFB128CT[16*2],16);	
	}
	
		
	printf("\r\n . AES MODE_OFB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESOFB128KEY,16,AESOFB128IV,PT,AESOFB128CT,32,MODE_OFB,0);
	if(memcmp(PT,AESOFB128PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(AESOFB128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESOFB128CT[16*2],32,MODE_OFB,LAST);
	if(memcmp(PT,&AESOFB128PT[16*2],32) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(&AESOFB128PT[16*2],16);	
	}

	

}
	{
	unsigned char AESOFB256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESOFB256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESOFB256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESOFB256CT[] = { 0xdc,0x7e,0x84,0xbf,0xda,0x79,0x16,0x4b,0x7e,0xcd,0x84,0x86,0x98,0x5d,0x38,0x60,0x4f,0xeb,0xdc,0x67,0x40,0xd2,0x0b,0x3a,0xc8,0x8f,0x6a,0xd8,0x2a,0x4f,0xb0,0x8d,0x71,0xab,0x47,0xa0,0x86,0xe8,0x6e,0xed,0xf3,0x9d,0x1c,0x5b,0xba,0x97,0xc4,0x08,0x01,0x26,0x14,0x1d,0x67,0xf3,0x7b,0xe8,0x53,0x8f,0x5a,0x8b,0xe7,0x40,0xe4,0x84};
	printf("\r\n . AES MODE_OFB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESOFB256KEY,32,AESOFB256IV,CT,AESOFB256PT,32,MODE_OFB,0);
	if(memcmp(CT,AESOFB256CT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(AESOFB256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESOFB256PT[16*2],32,MODE_OFB,LAST);
	if(memcmp(CT,&AESOFB256CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(&AESOFB256CT[16*2],16);	
	}
	
		
	printf("\r\n . AES MODE_OFB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESOFB256KEY,32,AESOFB256IV,PT,AESOFB256CT,32,MODE_OFB,0);
	if(memcmp(PT,AESOFB256PT,32) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(AESOFB256PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESOFB256CT[16*2],32,MODE_OFB,LAST);
	if(memcmp(PT,&AESOFB256PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(&AESOFB256PT[16*2],16);	
	}

	

}

		{
		unsigned char AESCTR128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
		unsigned char AESCTR128IV[] = {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
		unsigned char AESCTR128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
		unsigned char AESCTR128CT[] = { 0x87,0x4d,0x61,0x91,0xb6,0x20,0xe3,0x26,0x1b,0xef,0x68,0x64,0x99,0x0d,0xb6,0xce,0x98,0x06,0xf6,0x6b,0x79,0x70,0xfd,0xff,0x86,0x17,0x18,0x7b,0xb9,0xff,0xfd,0xff,0x5a,0xe4,0xdf,0x3e,0xdb,0xd5,0xd3,0x5e,0x5b,0x4f,0x09,0x02,0x0d,0xb0,0x3e,0xab,0x1e,0x03,0x1d,0xda,0x2f,0xbe,0x03,0xd1,0x79,0x21,0x70,0xa0,0xf3,0x00,0x9c,0xee};
		printf("\r\n . AES MODE_CTR 128 ENC TEST");
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCTR128KEY,16,AESCTR128IV,CT,AESCTR128PT,32,MODE_CTR,0);
		if(memcmp(CT,AESCTR128CT,32) == 0)
		   printf(" PASS");
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(AESCTR128CT,16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCTR128PT[16*2],32,MODE_CTR,LAST);
		if(memcmp(CT,&AESCTR128CT[16*2],32) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(&AESCTR128CT[16*2],16);	
		}
		
			
		printf("\r\n . AES MODE_CTR 128 DEC TEST");
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCTR128KEY,16,AESCTR128IV,PT,AESCTR128CT,32,MODE_CTR,0);
		if(memcmp(PT,AESCTR128PT,32) == 0)
		   printf(" PASS");
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(AESCTR128PT,16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCTR128CT[16*2],32,MODE_CTR,LAST);
		if(memcmp(PT,&AESCTR128PT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(&AESCTR128PT[16*2],16);	
		}
	
	
	}
		{
		unsigned char AESCTR256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
		unsigned char AESCTR256IV[] = {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
		unsigned char AESCTR256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
		unsigned char AESCTR256CT[] = { 0x60,0x1e,0xc3,0x13,0x77,0x57,0x89,0xa5,0xb7,0xa7,0xf5,0x04,0xbb,0xf3,0xd2,0x28,0xf4,0x43,0xe3,0xca,0x4d,0x62,0xb5,0x9a,0xca,0x84,0xe9,0x90,0xca,0xca,0xf5,0xc5,0x2b,0x09,0x30,0xda,0xa2,0x3d,0xe9,0x4c,0xe8,0x70,0x17,0xba,0x2d,0x84,0x98,0x8d,0xdf,0xc9,0xc5,0x8d,0xb6,0x7a,0xad,0xa6,0x13,0xc2,0xdd,0x08,0x45,0x79,0x41,0xa6};
		printf("\r\n . AES MODE_CTR 256 ENC TEST");
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCTR256KEY,32,AESCTR256IV,CT,AESCTR256PT,32,MODE_CTR,0);
		if(memcmp(CT,AESCTR256CT,16) == 0)
		   printf(" PASS");
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(AESCTR256CT,16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCTR256PT[16*2],32,MODE_CTR,LAST);
		if(memcmp(CT,&AESCTR256CT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(&AESCTR256CT[16*2],16);	
		}
		
		printf("\r\n . AES MODE_CTR 256 DEC TEST");
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCTR256KEY,32,AESCTR256IV,PT,AESCTR256CT,32,MODE_CTR,0);
		if(memcmp(PT,AESCTR256PT,32) == 0)
		   printf(" PASS");
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(AESCTR256PT,16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCTR256CT[16*2],32,MODE_CTR,LAST);
		if(memcmp(PT,&AESCTR256PT[16*2],32) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(&AESCTR256PT[16*2],16);	
		}
	
		
	
	}

	Hexstr2Bytes("00112233445566778899aabbccddeeff",ARIA128KEY);
	Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb",ARIA128PT);	
	Hexstr2Bytes("c6ecd08e22c30abdb215cf74e2075e6ec6ecd08e22c30abdb215cf74e2075e6ec6ecd08e22c30abdb215cf74e2075e6ec6ecd08e22c30abdb215cf74e2075e6e",ARIA128CT);		

	Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",ARIA256KEY);
	Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb11111111aaaaaaaa11111111bbbbbbbb",ARIA256PT);	
	Hexstr2Bytes("58a875e6044ad7fffa4f58420f7f442d58a875e6044ad7fffa4f58420f7f442d58a875e6044ad7fffa4f58420f7f442d58a875e6044ad7fffa4f58420f7f442d",ARIA256CT);	

	printf("\r\n . ARIA MODE_ECB 128 ENC TEST");                                           
	Dorca3CipherDecipher(RG_ENC,2/*ARIA*/,ARIA128KEY,16,0,CT,ARIA128PT,32,MODE_ECB,0); 
	if(memcmp(CT,ARIA128CT,32) == 0)                                                       
	   printf(" PASS");                                                                   
	Dorca3CipherDecipher(RG_ENC,2/*ARIA*/,0,16,0,CT,ARIA128PT+32,32,MODE_ECB,LAST);
	if(memcmp(CT,ARIA128CT+32,32) == 0)                                                    
	   printf(" PASS");	                                                                  
	                                                                                      
	printf("\r\n . ARIA MODE_ECB 128 DEC TEST");                                           
	Dorca3CipherDecipher(RG_DEC,2/*ARIA*/,ARIA128KEY,16,0,PT,ARIA128CT,32,MODE_ECB,0); 
	if(memcmp(PT,ARIA128PT,32) == 0)                                                       
	   printf(" PASS");                                                                   
	else {                                                                                
		printf("\r\n PT");                                                                  
		PrintByte(PT,32);                                                                   
		printf("\r\n ARIA128PT");                                                            
		PrintByte(ARIA128PT,32);	                                                            
	}                                                                                     
	Dorca3CipherDecipher(RG_DEC,2/*ARIA*/,0,16,0,PT,ARIA128CT+32,32,MODE_ECB,LAST);
	if(memcmp(PT,ARIA128PT+32,32) == 0)                                                    
	   printf(" PASS");                                                                   
	else {                                                                                
		printf("\r\n PT");                                                                  
		PrintByte(PT,32);                                                                   
		printf("\r\n ARIA128PT");                                                            
		PrintByte(ARIA128PT+32,32);	                                                        
	}                                                                                     
	                                                                                      
                                                                                        
	printf("\r\n . ARIA MODE_ECB 256 ENC TEST");                                           
	Dorca3CipherDecipher(RG_ENC,2/*ARIA*/,ARIA256KEY,32,0,CT,ARIA256PT,32,MODE_ECB,0); 
	if(memcmp(CT,ARIA256CT,32) == 0)                                                       
	   printf(" PASS");                                                                   
	Dorca3CipherDecipher(RG_ENC,2/*ARIA*/,0,32,0,CT,ARIA256PT+32,32,MODE_ECB,LAST);
	if(memcmp(CT,ARIA256CT+32,32) == 0)                                                    
	   printf(" PASS");                                                                   
	                                                                                      
	printf("\r\n . ARIA MODE_ECB 256 DEC TEST");                                           
	Dorca3CipherDecipher(RG_DEC,2/*ARIA*/,ARIA256KEY,32,0,PT,ARIA256CT,32,MODE_ECB,0); 
	if(memcmp(PT,ARIA256PT,32) == 0)                                                       
	   printf(" PASS");                                                                   
	else {                                                                                
		printf("\r\n PT");                                                                  
		PrintByte(PT,32);                                                                   
		printf("\r\n ARIA256PT");                                                            
		PrintByte(ARIA256PT,32);	                                                            
	}	                                                                            
                                                                                
	Dorca3CipherDecipher(RG_DEC,2/*ARIA*/,0,32,0,PT,ARIA256CT,32,MODE_ECB,LAST); 
	if(memcmp(PT,ARIA256PT,32) == 0)                                               
	   printf(" PASS");                                                           
	else {                                                                        
		printf("\r\n PT");                                                          
		PrintByte(PT,32);                                                           
		printf("\r\n ARIA256PT");                                                    
		PrintByte(ARIA256PT,32);	                                                    
		}                      



	

	{

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("49d61860b14909109cef0d22a9268134fadf9fb23151e9645fba75018bdb1538b53334634bbf7d4cd4b5377033060c155fe3948ca75de1031e1d85619e0ad61eb419a866b3c2dbfd10a4ed18b22149f75897f0b8668b0c1c542c687778835fb7cd46e45f85eaa7072437dd9fa6793d6f8d4ccefc4eb1ac641ac1bd30b18c6d64c49bca137eb21c2e04da62712ca2b4f540c57112c38791852cfac7a5d19ed83a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);
			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	
			printf("\r\n . ARIA MODE_CBC 128 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],32,MODE_CBC,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CBC,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CBC 128 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],32,MODE_CBC,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CBC,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("523a8a806ae621f155fdd28dbc34e1ab7b9b42432ad8b2efb96e23b13f0a6e52f36185d50ad002c5f601bee5493f118b243ee2e313642bffc3902e7b2efd9a12fa682edd2d23c8b9c5f043c18b17c1ec4b5867918270fbec1027c19ed6af833da5d620994668ca22f599791d292dd6273b2959082aafb7a996167cce1eec5f0cfd15f610d87e2dda9ba68ce1260ca54b222491418374294e7909b1e8551cd8de",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);			
			printf("\r\n. ARIA MODE_CBC 256 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],32,MODE_CBC,0);
				}	
				else if( 8  == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CBC,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CBC 256 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],32,MODE_CBC,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CBC,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a200c07c21e6370f413a5d132500a68285017c61b434c7b7ca9685a51071861e4d4bb873b599b479e2d573dddeafba89f812ac6a9e44d554078eb3be94839db4b33da3f59c063123a7ef6f20e10579fa4fd239100ca73b52d4fcafeadee73f139f78f9b7614c2b3b9dbe010f87db06a89a9435f79ce8121431371f4e87b984e0230c22a6dacb32fc42dcc6accef33285bf11",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);	
			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);
			printf("\r\n. ARIA MODE_CFB 128 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],32,MODE_CFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CFB,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CFB 128 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],32,MODE_CFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CFB,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f09009635f28bb93d8c31f870ec1e0bdb082b66fa402dd9c202be300c4517d196b14d4ce11dce97f7aaba54341b0d872cc9b63753a3e8556a14be6f7b3e27e3cfc39caf80f2a355aa50dc83c09c7b11828694f8e4aa726c528976b53f2c877f4991a3a8d28adb63bd751846ffb2350265e179d4990753ae8485ff9b4133ddad5875b84a90cbcfa62a045d726df71b6bda0eeca0be",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);			
			printf("\r\n. ARIA MODE_CFB 256 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],32,MODE_CFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CFB,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CFB 256 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],32,MODE_CFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CFB,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a2000063063f0560083483faeb041c8adecef30cf80cefb002a0d280759168ec01db3d49f61aced260bd43eec0a2731730eec6fa4f2304319cf8ccac2d7be7833e4f8ae6ce967012c1c6badc5d28e7e4144f6bf5cebe01253ee202afce4bc61f28dec069a6f16f6c8a7dd2afae44148f6ff4d0029d5c607b5fa6b8c8a6301cde5c7033565cd0b8f0974ab490b236197ba04a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);
			printf("\r\n . ARIA MODE_OFB 128 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],32,MODE_OFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_OFB,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_OFB 128 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],32,MODE_OFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_OFB,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f0900963584c256815c4292b59f8d3f966a75b52345b4f5f98c785d3f368a8d5ff89b7f950ceab3cd63773c2621d652b8ef98b4196afb2c2b30496bc5b7d9e7f9084f9d855f63a511751c8909e7a6deadbe0a67a4fb89383ca5d209c6f66f793fc471195c476fb9c1eab2ac91e680e454b4f3ed9a67fb52f09c29b965b23cfa6f3f6bbb2a86c6cdbaa2857bf2486f543231892a52",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);	
			printf("\r\n . ARIA MODE_OFB 256 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],32,MODE_OFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_OFB,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_OFB 256 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],32,MODE_OFB,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_OFB,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("ac5d7de805a0bf1c57c854501af60fa11497e2a34519dea1569e91e5b5ccae2ff3bfa1bf975f4571f48be191613546c3911163c085f871f0e7ae5f2a085b81851c2a3ddf20ecb8fa51901aec8ee4ba32a35dab67bb72cd9140ad188a967ac0fbbdfa94ea6cce47dcf8525ab5a814cfeb2bb60ee2b126e2d9d847c1a9e96f9019e3e6a7fe40d3829afb73db1cc245646addb62d9b907baaafbe46a73dbc131d3d",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);
			memset(IV,0,16);

			printf("\r\n . ARIA MODE_CTR 128 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],32,MODE_CTR,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],32,MODE_CTR,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CTR 128 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],32,MODE_CTR,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],32,MODE_CTR,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("30026c329666141721178b99c0a1f1b2f06940253f7b3089e2a30ea86aa3c88f5940f05ad7ee41d71347bb7261e348f18360473fdf7d4e7723bffb4411cc13f6cdd89f3bc7b9c768145022c7a74f14d7c305cd012a10f16050c23f1ae5c23f45998d13fbaa041e51619577e0772764896a5d4516d8ffceb3bf7e05f613edd9a60cdcedaff9cfcaf4e00d445a54334f73ab2cad944e51d266548e61c6eb0aa1cd",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);	
			printf("\r\n . ARIA MODE_CTR 256 ENC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],32,MODE_CTR,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],32,MODE_CTR,0);

				if(memcmp(CT,&RESULT[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CTR 256 DEC TEST");	
			for(i = 0; i < 10; i += 2 ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],32,MODE_CTR,0);
				}	
				else if( 8 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],32,MODE_CTR,0);

				if(memcmp(PT,&SOURCE[16*i],32) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}
  }
#if 0			
			printf(". ARIA MODE_CBC 128 DEC TEST");				
			
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("523a8a806ae621f155fdd28dbc34e1ab7b9b42432ad8b2efb96e23b13f0a6e52f36185d50ad002c5f601bee5493f118b243ee2e313642bffc3902e7b2efd9a12fa682edd2d23c8b9c5f043c18b17c1ec4b5867918270fbec1027c19ed6af833da5d620994668ca22f599791d292dd6273b2959082aafb7a996167cce1eec5f0cfd15f610d87e2dda9ba68ce1260ca54b222491418374294e7909b1e8551cd8de",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
			printf(". ARIA MODE_CBC 256 ENC TEST");
			printf(". ARIA MODE_CBC 256 DEC TEST");			


	}
	if(Mode == MODE_CFB) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f09009635f28bb93d8c31f870ec1e0bdb082b66fa402dd9c202be300c4517d196b14d4ce11dce97f7aaba54341b0d872cc9b63753a3e8556a14be6f7b3e27e3cfc39caf80f2a355aa50dc83c09c7b11828694f8e4aa726c528976b53f2c877f4991a3a8d28adb63bd751846ffb2350265e179d4990753ae8485ff9b4133ddad5875b84a90cbcfa62a045d726df71b6bda0eeca0be",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a200c07c21e6370f413a5d132500a68285017c61b434c7b7ca9685a51071861e4d4bb873b599b479e2d573dddeafba89f812ac6a9e44d554078eb3be94839db4b33da3f59c063123a7ef6f20e10579fa4fd239100ca73b52d4fcafeadee73f139f78f9b7614c2b3b9dbe010f87db06a89a9435f79ce8121431371f4e87b984e0230c22a6dacb32fc42dcc6accef33285bf11",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	

	}
	if(Mode == MODE_OFB) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f0900963584c256815c4292b59f8d3f966a75b52345b4f5f98c785d3f368a8d5ff89b7f950ceab3cd63773c2621d652b8ef98b4196afb2c2b30496bc5b7d9e7f9084f9d855f63a511751c8909e7a6deadbe0a67a4fb89383ca5d209c6f66f793fc471195c476fb9c1eab2ac91e680e454b4f3ed9a67fb52f09c29b965b23cfa6f3f6bbb2a86c6cdbaa2857bf2486f543231892a52",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a2000063063f0560083483faeb041c8adecef30cf80cefb002a0d280759168ec01db3d49f61aced260bd43eec0a2731730eec6fa4f2304319cf8ccac2d7be7833e4f8ae6ce967012c1c6badc5d28e7e4144f6bf5cebe01253ee202afce4bc61f28dec069a6f16f6c8a7dd2afae44148f6ff4d0029d5c607b5fa6b8c8a6301cde5c7033565cd0b8f0974ab490b236197ba04a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	

	}	
	if(Mode == MODE_CTR) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("30026c329666141721178b99c0a1f1b2f06940253f7b3089e2a30ea86aa3c88f5940f05ad7ee41d71347bb7261e348f18360473fdf7d4e7723bffb4411cc13f6cdd89f3bc7b9c768145022c7a74f14d7c305cd012a10f16050c23f1ae5c23f45998d13fbaa041e51619577e0772764896a5d4516d8ffceb3bf7e05f613edd9a60cdcedaff9cfcaf4e00d445a54334f73ab2cad944e51d266548e61c6eb0aa1cd",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("ac5d7de805a0bf1c57c854501af60fa11497e2a34519dea1569e91e5b5ccae2ff3bfa1bf975f4571f48be191613546c3911163c085f871f0e7ae5f2a085b81851c2a3ddf20ecb8fa51901aec8ee4ba32a35dab67bb72cd9140ad188a967ac0fbbdfa94ea6cce47dcf8525ab5a814cfeb2bb60ee2b126e2d9d847c1a9e96f9019e3e6a7fe40d3829afb73db1cc245646addb62d9b907baaafbe46a73dbc131d3d",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		memset(IV,0,16);


	}
#endif
    return;	

}

void AesAriaOperationModeTest()
{
	//AES TEST
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char SOURCE[16*10];
	unsigned char RESULT[16*10];
	unsigned char IV[16];
	unsigned char KEY[32];	
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char CT[16];
	unsigned char PT[16];	
	unsigned char AES128KEY[16];
	unsigned char AES128CT[16];
	unsigned char AES128PT[16];
	unsigned char AES256KEY[32];
	unsigned char AES256CT[16];
	unsigned char AES256PT[16];

	unsigned char ARIA128KEY[16];
	unsigned char ARIA128CT[16];
	unsigned char ARIA128PT[16];
	unsigned char ARIA256KEY[32];
	unsigned char ARIA256CT[16];
	unsigned char ARIA256PT[16];

	unsigned char *pKEY;
	unsigned char *pPT;
	unsigned char *pCT;
	unsigned char KEYBUFFER[64];
	memset(KEYBUFFER,0,64);
#if 1	
	
	Hexstr2Bytes("000102030405060708090a0b0c0d0e0f",AES128KEY);
	Hexstr2Bytes("69c4e0d86a7b0430d8cdb78070b4c55a",AES128CT);	
	Hexstr2Bytes("00112233445566778899aabbccddeeff",AES128PT);		

	Hexstr2Bytes("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",AES256KEY);
	Hexstr2Bytes("8ea2b7ca516745bfeafc49904b496089",AES256CT);	
	Hexstr2Bytes("00112233445566778899aabbccddeeff",AES256PT);	

	

	
	printf("\r\n . AES MODE_ECB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AES128KEY,16,0,CT,AES128PT,16,MODE_ECB,LAST);
	if(memcmp(CT,AES128CT,16) == 0)
	   printf(" PASS");
	printf("\r\n . AES MODE_ECB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AES128KEY,16,0,PT,AES128CT,16,MODE_ECB,LAST);
	if(memcmp(PT,AES128PT,16) == 0)
	   printf(" PASS");
	
	printf("\r\n . AES MODE_ECB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AES256KEY,32,0,CT,AES256PT,16,MODE_ECB,LAST);
	if(memcmp(CT,AES256CT,16) == 0)
	   printf(" PASS");
	printf("\r\n . AES MODE_ECB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AES256KEY,32,0,PT,AES256CT,16,MODE_ECB,LAST);
	if(memcmp(PT,AES256PT,16) == 0)
	   printf(" PASS");


#endif
	{
	unsigned char AESCBC128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESCBC128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCBC128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCBC128CT[] = { 0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d,0x50,0x86,0xcb,0x9b,0x50,0x72,0x19,0xee,0x95,0xdb,0x11,0x3a,0x91,0x76,0x78,0xb2,0x73,0xbe,0xd6,0xb8,0xe3,0xc1,0x74,0x3b,0x71,0x16,0xe6,0x9e,0x22,0x22,0x95,0x16,0x3f,0xf1,0xca,0xa1,0x68,0x1f,0xac,0x09,0x12,0x0e,0xca,0x30,0x75,0x86,0xe1,0xa7};
	printf("\r\n . AES MODE_CBC 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCBC128KEY,16,AESCBC128IV,CT,AESCBC128PT,16,MODE_CBC,0);
	if(memcmp(CT,AESCBC128CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC128CT");
		PrintByte(AESCBC128CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCBC128PT[16*1],16,MODE_CBC,0);
	if(memcmp(CT,&AESCBC128CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC128CT");
		PrintByte(&AESCBC128CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCBC128PT[16*2],16,MODE_CBC,0);
	if(memcmp(CT,&AESCBC128CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC128CT");
		PrintByte(&AESCBC128CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCBC128PT[16*3],16,MODE_CBC,LAST);
	if(memcmp(CT,&AESCBC128CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC128CT");
		PrintByte(&AESCBC128CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_CBC 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCBC128KEY,16,AESCBC128IV,PT,AESCBC128CT,16,MODE_CBC,0);
	if(memcmp(PT,AESCBC128PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(AESCBC128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCBC128CT[16*1],16,MODE_CBC,0);
	if(memcmp(PT,&AESCBC128PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(&AESCBC128PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCBC128CT[16*2],16,MODE_CBC,0);
	if(memcmp(PT,&AESCBC128PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(&AESCBC128PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCBC128CT[16*3],16,MODE_CBC,LAST);
	if(memcmp(PT,&AESCBC128PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC128PT");
		PrintByte(&AESCBC128CT[16*3],16);	
	}
	

}
	{
	unsigned char AESCBC256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESCBC256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCBC256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCBC256CT[] = { 0xf5,0x8c,0x4c,0x04,0xd6,0xe5,0xf1,0xba,0x77,0x9e,0xab,0xfb,0x5f,0x7b,0xfb,0xd6,0x9c,0xfc,0x4e,0x96,0x7e,0xdb,0x80,0x8d,0x67,0x9f,0x77,0x7b,0xc6,0x70,0x2c,0x7d,0x39,0xf2,0x33,0x69,0xa9,0xd9,0xba,0xcf,0xa5,0x30,0xe2,0x63,0x04,0x23,0x14,0x61,0xb2,0xeb,0x05,0xe2,0xc3,0x9b,0xe9,0xfc,0xda,0x6c,0x19,0x07,0x8c,0x6a,0x9d,0x1b};
	printf("\r\n . AES MODE_CBC 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCBC256KEY,32,AESCBC256IV,CT,AESCBC256PT,16,MODE_CBC,0);
	if(memcmp(CT,AESCBC256CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(AESCBC256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCBC256PT[16*1],16,MODE_CBC,0);
	if(memcmp(CT,&AESCBC256CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(&AESCBC256CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCBC256PT[16*2],16,MODE_CBC,0);
	if(memcmp(CT,&AESCBC256CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(&AESCBC256CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCBC256PT[16*3],16,MODE_CBC,LAST);
	if(memcmp(CT,&AESCBC256CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCBC256CT");
		PrintByte(&AESCBC256CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_CBC 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCBC256KEY,32,AESCBC256IV,PT,AESCBC256CT,16,MODE_CBC,0);
	if(memcmp(PT,AESCBC256PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(AESCBC256PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCBC256CT[16*1],16,MODE_CBC,0);
	if(memcmp(PT,&AESCBC256PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(&AESCBC256PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCBC256CT[16*2],16,MODE_CBC,0);
	if(memcmp(PT,&AESCBC256PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(&AESCBC256PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCBC256CT[16*3],16,MODE_CBC,LAST);
	if(memcmp(PT,&AESCBC256PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCBC256PT");
		PrintByte(&AESCBC256CT[16*3],16);	
	}
	

}

	{
	unsigned char AESCFB128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESCFB128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCFB128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCFB128CT[] = { 0x3b,0x3f,0xd9,0x2e,0xb7,0x2d,0xad,0x20,0x33,0x34,0x49,0xf8,0xe8,0x3c,0xfb,0x4a,0xc8,0xa6,0x45,0x37,0xa0,0xb3,0xa9,0x3f,0xcd,0xe3,0xcd,0xad,0x9f,0x1c,0xe5,0x8b,0x26,0x75,0x1f,0x67,0xa3,0xcb,0xb1,0x40,0xb1,0x80,0x8c,0xf1,0x87,0xa4,0xf4,0xdf,0xc0,0x4b,0x05,0x35,0x7c,0x5d,0x1c,0x0e,0xea,0xc4,0xc6,0x6f,0x9f,0xf7,0xf2,0xe6};
	printf("\r\n . AES MODE_CFB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCFB128KEY,16,AESCFB128IV,CT,AESCFB128PT,16,MODE_CFB,0);
	if(memcmp(CT,AESCFB128CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(AESCFB128CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCFB128PT[16*1],16,MODE_CFB,0);
	if(memcmp(CT,&AESCFB128CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(&AESCFB128CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCFB128PT[16*2],16,MODE_CFB,0);
	if(memcmp(CT,&AESCFB128CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(&AESCFB128CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCFB128PT[16*3],16,MODE_CFB,LAST);
	if(memcmp(CT,&AESCFB128CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB128CT");
		PrintByte(&AESCFB128CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_CFB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCFB128KEY,16,AESCFB128IV,PT,AESCFB128CT,16,MODE_CFB,0);
	if(memcmp(PT,AESCFB128PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(AESCFB128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCFB128CT[16*1],16,MODE_CFB,0);
	if(memcmp(PT,&AESCFB128PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(&AESCFB128PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCFB128CT[16*2],16,MODE_CFB,0);
	if(memcmp(PT,&AESCFB128PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(&AESCFB128PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCFB128CT[16*3],16,MODE_CFB,LAST);
	if(memcmp(PT,&AESCFB128PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB128PT");
		PrintByte(&AESCFB128CT[16*3],16);	
	}
	

}
	{
	unsigned char AESCFB256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESCFB256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESCFB256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESCFB256CT[] = { 0xdc,0x7e,0x84,0xbf,0xda,0x79,0x16,0x4b,0x7e,0xcd,0x84,0x86,0x98,0x5d,0x38,0x60,0x39,0xff,0xed,0x14,0x3b,0x28,0xb1,0xc8,0x32,0x11,0x3c,0x63,0x31,0xe5,0x40,0x7b,0xdf,0x10,0x13,0x24,0x15,0xe5,0x4b,0x92,0xa1,0x3e,0xd0,0xa8,0x26,0x7a,0xe2,0xf9,0x75,0xa3,0x85,0x74,0x1a,0xb9,0xce,0xf8,0x20,0x31,0x62,0x3d,0x55,0xb1,0xe4,0x71};
	printf("\r\n . AES MODE_CFB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCFB256KEY,32,AESCFB256IV,CT,AESCFB256PT,16,MODE_CFB,0);
	if(memcmp(CT,AESCFB256CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(AESCFB256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCFB256PT[16*1],16,MODE_CFB,0);
	if(memcmp(CT,&AESCFB256CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(&AESCFB256CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCFB256PT[16*2],16,MODE_CFB,0);
	if(memcmp(CT,&AESCFB256CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(&AESCFB256CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCFB256PT[16*3],16,MODE_CFB,LAST);
	if(memcmp(CT,&AESCFB256CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESCFB256CT");
		PrintByte(&AESCFB256CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_CFB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCFB256KEY,32,AESCFB256IV,PT,AESCFB256CT,16,MODE_CFB,0);
	if(memcmp(PT,AESCFB256PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(AESCFB256PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCFB256CT[16*1],16,MODE_CFB,0);
	if(memcmp(PT,&AESCFB256PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(&AESCFB256PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCFB256CT[16*2],16,MODE_CFB,0);
	if(memcmp(PT,&AESCFB256PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(&AESCFB256PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCFB256CT[16*3],16,MODE_CFB,LAST);
	if(memcmp(PT,&AESCFB256PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESCFB256PT");
		PrintByte(&AESCFB256CT[16*3],16);	
	}
	

}
	{
	unsigned char AESOFB128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	unsigned char AESOFB128IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESOFB128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESOFB128CT[] = { 0x3b,0x3f,0xd9,0x2e,0xb7,0x2d,0xad,0x20,0x33,0x34,0x49,0xf8,0xe8,0x3c,0xfb,0x4a,0x77,0x89,0x50,0x8d,0x16,0x91,0x8f,0x03,0xf5,0x3c,0x52,0xda,0xc5,0x4e,0xd8,0x25,0x97,0x40,0x05,0x1e,0x9c,0x5f,0xec,0xf6,0x43,0x44,0xf7,0xa8,0x22,0x60,0xed,0xcc,0x30,0x4c,0x65,0x28,0xf6,0x59,0xc7,0x78,0x66,0xa5,0x10,0xd9,0xc1,0xd6,0xae,0x5e};
	printf("\r\n . AES MODE_OFB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESOFB128KEY,16,AESOFB128IV,CT,AESOFB128PT,16,MODE_OFB,0);
	if(memcmp(CT,AESOFB128CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(AESOFB128CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESOFB128PT[16*1],16,MODE_OFB,0);
	if(memcmp(CT,&AESOFB128CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(&AESOFB128CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESOFB128PT[16*2],16,MODE_OFB,0);
	if(memcmp(CT,&AESOFB128CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(&AESOFB128CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESOFB128PT[16*3],16,MODE_OFB,LAST);
	if(memcmp(CT,&AESOFB128CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB128CT");
		PrintByte(&AESOFB128CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_OFB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESOFB128KEY,16,AESOFB128IV,PT,AESOFB128CT,16,MODE_OFB,0);
	if(memcmp(PT,AESOFB128PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(AESOFB128PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESOFB128CT[16*1],16,MODE_OFB,0);
	if(memcmp(PT,&AESOFB128PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(&AESOFB128PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESOFB128CT[16*2],16,MODE_OFB,0);
	if(memcmp(PT,&AESOFB128PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(&AESOFB128PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESOFB128CT[16*3],16,MODE_OFB,LAST);
	if(memcmp(PT,&AESOFB128PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB128PT");
		PrintByte(&AESOFB128CT[16*3],16);	
	}
	

}
	{
	unsigned char AESOFB256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	unsigned char AESOFB256IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	unsigned char AESOFB256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
	unsigned char AESOFB256CT[] = { 0xdc,0x7e,0x84,0xbf,0xda,0x79,0x16,0x4b,0x7e,0xcd,0x84,0x86,0x98,0x5d,0x38,0x60,0x4f,0xeb,0xdc,0x67,0x40,0xd2,0x0b,0x3a,0xc8,0x8f,0x6a,0xd8,0x2a,0x4f,0xb0,0x8d,0x71,0xab,0x47,0xa0,0x86,0xe8,0x6e,0xed,0xf3,0x9d,0x1c,0x5b,0xba,0x97,0xc4,0x08,0x01,0x26,0x14,0x1d,0x67,0xf3,0x7b,0xe8,0x53,0x8f,0x5a,0x8b,0xe7,0x40,0xe4,0x84};
	printf("\r\n . AES MODE_OFB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESOFB256KEY,32,AESOFB256IV,CT,AESOFB256PT,16,MODE_OFB,0);
	if(memcmp(CT,AESOFB256CT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(AESOFB256CT,16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESOFB256PT[16*1],16,MODE_OFB,0);
	if(memcmp(CT,&AESOFB256CT[16*1],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(&AESOFB256CT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESOFB256PT[16*2],16,MODE_OFB,0);
	if(memcmp(CT,&AESOFB256CT[16*2],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(&AESOFB256CT[16*2],16);	
	}
	
	Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESOFB256PT[16*3],16,MODE_OFB,LAST);
	if(memcmp(CT,&AESOFB256CT[16*3],16) == 0)
	   printf(" PASS");	
	else{
		printf("\r\n CT");
		PrintByte(CT,16);
	
		printf("\r\n AESOFB256CT");
		PrintByte(&AESOFB256CT[16*3],16);	
	}
		
	printf("\r\n . AES MODE_OFB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESOFB256KEY,32,AESOFB256IV,PT,AESOFB256CT,16,MODE_OFB,0);
	if(memcmp(PT,AESOFB256PT,16) == 0)
	   printf(" PASS");
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(AESOFB256PT,16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESOFB256CT[16*1],16,MODE_OFB,0);
	if(memcmp(PT,&AESOFB256PT[16*1],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(&AESOFB256PT[16*1],16);	
	}
	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESOFB256CT[16*2],16,MODE_OFB,0);
	if(memcmp(PT,&AESOFB256PT[16*2],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(&AESOFB256PT[16*2],16);	
	}

	Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESOFB256CT[16*3],16,MODE_OFB,LAST);
	if(memcmp(PT,&AESOFB256PT[16*3],16) == 0)
	   printf(" PASS"); 
	else{
		printf("\r\n PT");
		PrintByte(PT,16);

		printf("\r\n AESOFB256PT");
		PrintByte(&AESOFB256CT[16*3],16);	
	}
	

}

		{
		unsigned char AESCTR128KEY[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
		unsigned char AESCTR128IV[] = {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
		unsigned char AESCTR128PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
		unsigned char AESCTR128CT[] = { 0x87,0x4d,0x61,0x91,0xb6,0x20,0xe3,0x26,0x1b,0xef,0x68,0x64,0x99,0x0d,0xb6,0xce,0x98,0x06,0xf6,0x6b,0x79,0x70,0xfd,0xff,0x86,0x17,0x18,0x7b,0xb9,0xff,0xfd,0xff,0x5a,0xe4,0xdf,0x3e,0xdb,0xd5,0xd3,0x5e,0x5b,0x4f,0x09,0x02,0x0d,0xb0,0x3e,0xab,0x1e,0x03,0x1d,0xda,0x2f,0xbe,0x03,0xd1,0x79,0x21,0x70,0xa0,0xf3,0x00,0x9c,0xee};
		printf("\r\n . AES MODE_CTR 128 ENC TEST");
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCTR128KEY,16,AESCTR128IV,CT,AESCTR128PT,16,MODE_CTR,0);
		if(memcmp(CT,AESCTR128CT,16) == 0)
		   printf(" PASS");
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(AESCTR128CT,16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCTR128PT[16*1],16,MODE_CTR,0);
		if(memcmp(CT,&AESCTR128CT[16*1],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(&AESCTR128CT[16*1],16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCTR128PT[16*2],16,MODE_CTR,0);
		if(memcmp(CT,&AESCTR128CT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(&AESCTR128CT[16*2],16);	
		}
		
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,16,0,CT,&AESCTR128PT[16*3],16,MODE_CTR,LAST);
		if(memcmp(CT,&AESCTR128CT[16*3],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR128CT");
			PrintByte(&AESCTR128CT[16*3],16);	
		}
			
		printf("\r\n . AES MODE_CTR 128 DEC TEST");
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCTR128KEY,16,AESCTR128IV,PT,AESCTR128CT,16,MODE_CTR,0);
		if(memcmp(PT,AESCTR128PT,16) == 0)
		   printf(" PASS");
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(AESCTR128PT,16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCTR128CT[16*1],16,MODE_CTR,0);
		if(memcmp(PT,&AESCTR128PT[16*1],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(&AESCTR128PT[16*1],16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCTR128CT[16*2],16,MODE_CTR,0);
		if(memcmp(PT,&AESCTR128PT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(&AESCTR128PT[16*2],16);	
		}
	
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,16,0,PT,&AESCTR128CT[16*3],16,MODE_CTR,LAST);
		if(memcmp(PT,&AESCTR128PT[16*3],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR128PT");
			PrintByte(&AESCTR128CT[16*3],16);	
		}
		
	
	}
		{
		unsigned char AESCTR256KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
		unsigned char AESCTR256IV[] = {0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
		unsigned char AESCTR256PT[] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};
		unsigned char AESCTR256CT[] = { 0x60,0x1e,0xc3,0x13,0x77,0x57,0x89,0xa5,0xb7,0xa7,0xf5,0x04,0xbb,0xf3,0xd2,0x28,0xf4,0x43,0xe3,0xca,0x4d,0x62,0xb5,0x9a,0xca,0x84,0xe9,0x90,0xca,0xca,0xf5,0xc5,0x2b,0x09,0x30,0xda,0xa2,0x3d,0xe9,0x4c,0xe8,0x70,0x17,0xba,0x2d,0x84,0x98,0x8d,0xdf,0xc9,0xc5,0x8d,0xb6,0x7a,0xad,0xa6,0x13,0xc2,0xdd,0x08,0x45,0x79,0x41,0xa6};
		printf("\r\n . AES MODE_CTR 256 ENC TEST");
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,AESCTR256KEY,32,AESCTR256IV,CT,AESCTR256PT,16,MODE_CTR,0);
		if(memcmp(CT,AESCTR256CT,16) == 0)
		   printf(" PASS");
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(AESCTR256CT,16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCTR256PT[16*1],16,MODE_CTR,0);
		if(memcmp(CT,&AESCTR256CT[16*1],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(&AESCTR256CT[16*1],16);	
		}
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCTR256PT[16*2],16,MODE_CTR,0);
		if(memcmp(CT,&AESCTR256CT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(&AESCTR256CT[16*2],16);	
		}
		
		Dorca3CipherDecipher(RG_ENC,1 /*AES*/,0,32,0,CT,&AESCTR256PT[16*3],16,MODE_CTR,LAST);
		if(memcmp(CT,&AESCTR256CT[16*3],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n CT");
			PrintByte(CT,16);
		
			printf("\r\n AESCTR256CT");
			PrintByte(&AESCTR256CT[16*3],16);	
		}
			
		printf("\r\n . AES MODE_CTR 256 DEC TEST");
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,AESCTR256KEY,32,AESCTR256IV,PT,AESCTR256CT,16,MODE_CTR,0);
		if(memcmp(PT,AESCTR256PT,16) == 0)
		   printf(" PASS");
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(AESCTR256PT,16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCTR256CT[16*1],16,MODE_CTR,0);
		if(memcmp(PT,&AESCTR256PT[16*1],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(&AESCTR256PT[16*1],16);	
		}
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCTR256CT[16*2],16,MODE_CTR,0);
		if(memcmp(PT,&AESCTR256PT[16*2],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(&AESCTR256PT[16*2],16);	
		}
	
		Dorca3CipherDecipher(RG_DEC,1 /*AES*/,0,32,0,PT,&AESCTR256CT[16*3],16,MODE_CTR,LAST);
		if(memcmp(PT,&AESCTR256PT[16*3],16) == 0)
		   printf(" PASS"); 
		else{
			printf("\r\n PT");
			PrintByte(PT,16);
	
			printf("\r\n AESCTR256PT");
			PrintByte(&AESCTR256CT[16*3],16);	
		}
		
	
	}

	Hexstr2Bytes("00112233445566778899aabbccddeeff",ARIA128KEY);
	Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb",ARIA128PT);	
	Hexstr2Bytes("c6ecd08e22c30abdb215cf74e2075e6e",ARIA128CT);		

	Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",ARIA256KEY);
	Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb",ARIA256PT);	
	Hexstr2Bytes("58a875e6044ad7fffa4f58420f7f442d",ARIA256CT);	

	printf("\r\n . ARIA MODE_ECB 128 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,ARIA128KEY,16,0,CT,ARIA128PT,16,MODE_ECB,LAST);
	if(memcmp(CT,ARIA128CT,16) == 0)
	   printf(" PASS");
	printf("\r\n . ARIA MODE_ECB 128 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,ARIA128KEY,16,0,PT,ARIA128CT,16,MODE_ECB,LAST);
	if(memcmp(PT,ARIA128PT,16) == 0)
	   printf(" PASS");
	printf("\r\n . ARIA MODE_ECB 256 ENC TEST");
	Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,ARIA256KEY,32,0,CT,ARIA256PT,16,MODE_ECB,LAST);
	if(memcmp(CT,ARIA256CT,16) == 0)
	   printf(" PASS");
	printf("\r\n . ARIA MODE_ECB 256 DEC TEST");
	Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,ARIA256KEY,32,0,PT,ARIA256CT,16,MODE_ECB,LAST);
	if(memcmp(PT,ARIA256PT,16) == 0)
	   printf(" PASS");

	{

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("49d61860b14909109cef0d22a9268134fadf9fb23151e9645fba75018bdb1538b53334634bbf7d4cd4b5377033060c155fe3948ca75de1031e1d85619e0ad61eb419a866b3c2dbfd10a4ed18b22149f75897f0b8668b0c1c542c687778835fb7cd46e45f85eaa7072437dd9fa6793d6f8d4ccefc4eb1ac641ac1bd30b18c6d64c49bca137eb21c2e04da62712ca2b4f540c57112c38791852cfac7a5d19ed83a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);
			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	
			printf("\r\n . ARIA MODE_CBC 128 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],16,MODE_CBC,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CBC,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CBC 128 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],16,MODE_CBC,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CBC,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("523a8a806ae621f155fdd28dbc34e1ab7b9b42432ad8b2efb96e23b13f0a6e52f36185d50ad002c5f601bee5493f118b243ee2e313642bffc3902e7b2efd9a12fa682edd2d23c8b9c5f043c18b17c1ec4b5867918270fbec1027c19ed6af833da5d620994668ca22f599791d292dd6273b2959082aafb7a996167cce1eec5f0cfd15f610d87e2dda9ba68ce1260ca54b222491418374294e7909b1e8551cd8de",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);			
			printf("\r\n. ARIA MODE_CBC 256 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],16,MODE_CBC,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CBC,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CBC 256 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],16,MODE_CBC,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CBC,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CBC,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a200c07c21e6370f413a5d132500a68285017c61b434c7b7ca9685a51071861e4d4bb873b599b479e2d573dddeafba89f812ac6a9e44d554078eb3be94839db4b33da3f59c063123a7ef6f20e10579fa4fd239100ca73b52d4fcafeadee73f139f78f9b7614c2b3b9dbe010f87db06a89a9435f79ce8121431371f4e87b984e0230c22a6dacb32fc42dcc6accef33285bf11",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);	
			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);
			printf("\r\n. ARIA MODE_CFB 128 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],16,MODE_CFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CFB,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CFB 128 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],16,MODE_CFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CFB,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f09009635f28bb93d8c31f870ec1e0bdb082b66fa402dd9c202be300c4517d196b14d4ce11dce97f7aaba54341b0d872cc9b63753a3e8556a14be6f7b3e27e3cfc39caf80f2a355aa50dc83c09c7b11828694f8e4aa726c528976b53f2c877f4991a3a8d28adb63bd751846ffb2350265e179d4990753ae8485ff9b4133ddad5875b84a90cbcfa62a045d726df71b6bda0eeca0be",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);			
			printf("\r\n. ARIA MODE_CFB 256 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],16,MODE_CFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CFB,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CFB 256 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],16,MODE_CFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CFB,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a2000063063f0560083483faeb041c8adecef30cf80cefb002a0d280759168ec01db3d49f61aced260bd43eec0a2731730eec6fa4f2304319cf8ccac2d7be7833e4f8ae6ce967012c1c6badc5d28e7e4144f6bf5cebe01253ee202afce4bc61f28dec069a6f16f6c8a7dd2afae44148f6ff4d0029d5c607b5fa6b8c8a6301cde5c7033565cd0b8f0974ab490b236197ba04a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

			Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);
			printf("\r\n . ARIA MODE_OFB 128 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],16,MODE_OFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_OFB,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_OFB 128 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],16,MODE_OFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_OFB,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f0900963584c256815c4292b59f8d3f966a75b52345b4f5f98c785d3f368a8d5ff89b7f950ceab3cd63773c2621d652b8ef98b4196afb2c2b30496bc5b7d9e7f9084f9d855f63a511751c8909e7a6deadbe0a67a4fb89383ca5d209c6f66f793fc471195c476fb9c1eab2ac91e680e454b4f3ed9a67fb52f09c29b965b23cfa6f3f6bbb2a86c6cdbaa2857bf2486f543231892a52",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);	
			printf("\r\n . ARIA MODE_OFB 256 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],16,MODE_OFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_OFB,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_OFB 256 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],16,MODE_OFB,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_OFB,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_OFB,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("ac5d7de805a0bf1c57c854501af60fa11497e2a34519dea1569e91e5b5ccae2ff3bfa1bf975f4571f48be191613546c3911163c085f871f0e7ae5f2a085b81851c2a3ddf20ecb8fa51901aec8ee4ba32a35dab67bb72cd9140ad188a967ac0fbbdfa94ea6cce47dcf8525ab5a814cfeb2bb60ee2b126e2d9d847c1a9e96f9019e3e6a7fe40d3829afb73db1cc245646addb62d9b907baaafbe46a73dbc131d3d",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);
			memset(IV,0,16);

			printf("\r\n . ARIA MODE_CTR 128 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,16,IV,CT,&SOURCE[16*i],16,MODE_CTR,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,16,0,CT,&SOURCE[16*i],16,MODE_CTR,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CTR 128 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,16,IV,PT,&RESULT[16*i],16,MODE_CTR,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,16,0,PT,&RESULT[16*i],16,MODE_CTR,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}


			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("30026c329666141721178b99c0a1f1b2f06940253f7b3089e2a30ea86aa3c88f5940f05ad7ee41d71347bb7261e348f18360473fdf7d4e7723bffb4411cc13f6cdd89f3bc7b9c768145022c7a74f14d7c305cd012a10f16050c23f1ae5c23f45998d13fbaa041e51619577e0772764896a5d4516d8ffceb3bf7e05f613edd9a60cdcedaff9cfcaf4e00d445a54334f73ab2cad944e51d266548e61c6eb0aa1cd",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);	
			printf("\r\n . ARIA MODE_CTR 256 ENC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,KEY,32,IV,CT,&SOURCE[16*i],16,MODE_CTR,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_ENC,2 /*ARIA*/,0,32,0,CT,&SOURCE[16*i],16,MODE_CTR,0);

				if(memcmp(CT,&RESULT[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n CT");
					PrintByte(CT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&RESULT[16*i],16);	
				}
			}

			printf("\r\n . ARIA MODE_CTR 256 DEC TEST");	
			for(i = 0; i < 10; i++ ){
				
				if(0 == i){
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,KEY,32,IV,PT,&RESULT[16*i],16,MODE_CTR,0);
				}	
				else if( 9 == i)
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CTR,LAST);
				else
					Dorca3CipherDecipher(RG_DEC,2 /*ARIA*/,0,32,0,PT,&RESULT[16*i],16,MODE_CTR,0);

				if(memcmp(PT,&SOURCE[16*i],16) == 0)
				   printf(" PASS"); 
				else{
					printf("\r\n PT");
					PrintByte(PT,16);
			
					printf("\r\n &RESULT");
					PrintByte(&SOURCE[16*i],16);	
				}
			}
  }
#if 0			
			printf(". ARIA MODE_CBC 128 DEC TEST");				
			
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("523a8a806ae621f155fdd28dbc34e1ab7b9b42432ad8b2efb96e23b13f0a6e52f36185d50ad002c5f601bee5493f118b243ee2e313642bffc3902e7b2efd9a12fa682edd2d23c8b9c5f043c18b17c1ec4b5867918270fbec1027c19ed6af833da5d620994668ca22f599791d292dd6273b2959082aafb7a996167cce1eec5f0cfd15f610d87e2dda9ba68ce1260ca54b222491418374294e7909b1e8551cd8de",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
			printf(". ARIA MODE_CBC 256 ENC TEST");
			printf(". ARIA MODE_CBC 256 DEC TEST");			


	}
	if(Mode == MODE_CFB) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f09009635f28bb93d8c31f870ec1e0bdb082b66fa402dd9c202be300c4517d196b14d4ce11dce97f7aaba54341b0d872cc9b63753a3e8556a14be6f7b3e27e3cfc39caf80f2a355aa50dc83c09c7b11828694f8e4aa726c528976b53f2c877f4991a3a8d28adb63bd751846ffb2350265e179d4990753ae8485ff9b4133ddad5875b84a90cbcfa62a045d726df71b6bda0eeca0be",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a200c07c21e6370f413a5d132500a68285017c61b434c7b7ca9685a51071861e4d4bb873b599b479e2d573dddeafba89f812ac6a9e44d554078eb3be94839db4b33da3f59c063123a7ef6f20e10579fa4fd239100ca73b52d4fcafeadee73f139f78f9b7614c2b3b9dbe010f87db06a89a9435f79ce8121431371f4e87b984e0230c22a6dacb32fc42dcc6accef33285bf11",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	

	}
	if(Mode == MODE_OFB) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("26834705b0f2c0e2588d4a7f0900963584c256815c4292b59f8d3f966a75b52345b4f5f98c785d3f368a8d5ff89b7f950ceab3cd63773c2621d652b8ef98b4196afb2c2b30496bc5b7d9e7f9084f9d855f63a511751c8909e7a6deadbe0a67a4fb89383ca5d209c6f66f793fc471195c476fb9c1eab2ac91e680e454b4f3ed9a67fb52f09c29b965b23cfa6f3f6bbb2a86c6cdbaa2857bf2486f543231892a52",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("3720e53ba7d615383406b09f0a05a2000063063f0560083483faeb041c8adecef30cf80cefb002a0d280759168ec01db3d49f61aced260bd43eec0a2731730eec6fa4f2304319cf8ccac2d7be7833e4f8ae6ce967012c1c6badc5d28e7e4144f6bf5cebe01253ee202afce4bc61f28dec069a6f16f6c8a7dd2afae44148f6ff4d0029d5c607b5fa6b8c8a6301cde5c7033565cd0b8f0974ab490b236197ba04a",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		Hexstr2Bytes("0f1e2d3c4b5a69788796a5b4c3d2e1f0",IV);	

	}	
	if(Mode == MODE_CTR) {
		if(RG256 == RG_128_256){

			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("30026c329666141721178b99c0a1f1b2f06940253f7b3089e2a30ea86aa3c88f5940f05ad7ee41d71347bb7261e348f18360473fdf7d4e7723bffb4411cc13f6cdd89f3bc7b9c768145022c7a74f14d7c305cd012a10f16050c23f1ae5c23f45998d13fbaa041e51619577e0772764896a5d4516d8ffceb3bf7e05f613edd9a60cdcedaff9cfcaf4e00d445a54334f73ab2cad944e51d266548e61c6eb0aa1cd",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",KEY);
		}
		else
		{
			Hexstr2Bytes("11111111aaaaaaaa11111111bbbbbbbb11111111cccccccc11111111dddddddd22222222aaaaaaaa22222222bbbbbbbb22222222cccccccc22222222dddddddd33333333aaaaaaaa33333333bbbbbbbb33333333cccccccc33333333dddddddd44444444aaaaaaaa44444444bbbbbbbb44444444cccccccc44444444dddddddd55555555aaaaaaaa55555555bbbbbbbb55555555cccccccc55555555dddddddd",SOURCE);
			Hexstr2Bytes("ac5d7de805a0bf1c57c854501af60fa11497e2a34519dea1569e91e5b5ccae2ff3bfa1bf975f4571f48be191613546c3911163c085f871f0e7ae5f2a085b81851c2a3ddf20ecb8fa51901aec8ee4ba32a35dab67bb72cd9140ad188a967ac0fbbdfa94ea6cce47dcf8525ab5a814cfeb2bb60ee2b126e2d9d847c1a9e96f9019e3e6a7fe40d3829afb73db1cc245646addb62d9b907baaafbe46a73dbc131d3d",RESULT);
			Hexstr2Bytes("00112233445566778899aabbccddeeff",KEY);

		}
		memset(IV,0,16);


	}
#endif
    
return;	
}

void EcdsaVerifySignatureTestFail()
{
		unsigned char  r[32];
		unsigned char  s[32];
		unsigned char  h[32];
		point public_key;
		
	
		Hexstr2Bytes("cf4897766131aa8b7f80453a15bf90f7517878579d5a4f973aea5bb11542e07f",public_key.y);
		Hexstr2Bytes("d6606271131e7e7e617a81aa11f09e7ed56311828823367a869b454040b3f905",public_key.x);
		Hexstr2Bytes("0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef90000",h);
	
	//	Hexstr2Bytes("0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b51",h);
	//0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b56
		Hexstr2Bytes("b5b417619bf9fa89d50b3e22782a2de80a86db67e728114e6e0e91cab1a40000",r);
	//		Hexstr2Bytes("b5b417619bf9fa89d50b3e22782a2de80a86db67e728114e6e0e91cab1a41612",r);
		Hexstr2Bytes("e43e8111258bea6f5c96bd6d66715748fbee756da418de90f64066c6b3e072f1",s);
		
		if (EcdsaVerifySignature(&public_key,r,s,h) == 0)
			printf("\r\n TEST PASS");
		else
			printf("\r\n TEST FAIL");
		return;	
}
void EcdsaGenSignatureTest()
{
	unsigned char  h[32];
	unsigned char  k[32];			
	unsigned char  d[32];			
	unsigned char  r[32];
	unsigned char  s[32];
	unsigned char  expected_r[32];
	unsigned char  expected_s[32];	
	Hexstr2Bytes("00d007e1b9afcc312eec9cecffa0280752bbd1953182edef12f3fc366e8f4356",d);	
	Hexstr2Bytes("00c03c3b8b1e40cb328a61d51783356935625884399e26a5828f387c2bde6ebc",k);
	Hexstr2Bytes("0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b56",h);
	
	EcdsaGenSignature(d,k,h,r,s);

	
	Hexstr2Bytes("b5b417619bf9fa89d50b3e22782a2de80a86db67e728114e6e0e91cab1a41612",expected_r);
	Hexstr2Bytes("e43e8111258bea6f5c96bd6d66715748fbee756da418de90f64066c6b3e072f1",expected_s);
	if(memcmp(expected_r,r,32) == 0)
		printf("\r\n Gen Sig R pass");
	else {
		printf("\r\n Expected r");
		PrintByte(expected_r, 32);
	    printf("\r\n Result");
		PrintByte(r,32);
	}
		
	if(memcmp(expected_s,s,32) == 0)
		printf("\r\n Gen Sig S pass");	
	else {
		printf("\r\n Expected s");
		PrintByte(expected_r, 32);
	    printf("\r\n Result");
		PrintByte(s,32);
	}	
return;	
}
void EcdsaVerifySignatureTest()
{
	unsigned char  r[32];
	unsigned char  s[32];
	unsigned char  h[32];
    point public_key;
	

	Hexstr2Bytes("cf4897766131aa8b7f80453a15bf90f7517878579d5a4f973aea5bb11542e07f",public_key.y);
	Hexstr2Bytes("d6606271131e7e7e617a81aa11f09e7ed56311828823367a869b454040b3f905",public_key.x);
//	Hexstr2Bytes("0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b56",h);

	Hexstr2Bytes("0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b56",h);
//0000000000000000000000000f7b55549fab573c0361b832ad0be8cdeef91b56
	Hexstr2Bytes("b5b417619bf9fa89d50b3e22782a2de80a86db67e728114e6e0e91cab1a41612",r);
	Hexstr2Bytes("e43e8111258bea6f5c96bd6d66715748fbee756da418de90f64066c6b3e072f1",s);
	
	if (EcdsaVerifySignature(&public_key,r,s,h) == 0)
		printf("\r\n TEST PASS");
	else
		printf("\r\n TEST FAIL");
	return;	
}
void TestEcdhPub(void)
{
	unsigned char  sk[32];
	unsigned char  common_key[32];	
	unsigned char  temp_buffer[32];
	size_t  key_length;
	point p1;
	printf("\r\n TEST EcdhGenPubKey");
	Hexstr2Bytes("c64d654e263cda95d6dc719d3cfd6c3b932b1fea6021b9e2ac36995c4d96ae3d",sk);
	EcdhGenPubKey(sk,&p1);
	printf("\r\nGet_ECDH_PublicKey_X\r\n");
	PrintByte(p1.x,32);

	printf("\r\n Expected ECDH_PublicKey_X\r\n");
	printf("\r\nefb50f68f26558c1d42847e82dc552607965049cc6f65d7ed8b8d02a1d8825f9");
	Hexstr2Bytes("efb50f68f26558c1d42847e82dc552607965049cc6f65d7ed8b8d02a1d8825f9",temp_buffer);
	if(memcmp(p1.x,temp_buffer,32) == 0)
		printf("\r\n PASS");
	else
		printf("\r\n FAIL");

	printf("\r\nGet_ECDH_PublicKey_Y\r\n");
	PrintByte(p1.y,32);

	printf("\r\n Expected ECDH_PublicKey_Y\r\n");
	printf("\r\n3ca531980f67c4843db00419470860d736577867c5eab3e3ed304fd253949202");

	Hexstr2Bytes("3ca531980f67c4843db00419470860d736577867c5eab3e3ed304fd253949202",temp_buffer);

	if(memcmp(p1.y,temp_buffer,32) == 0)
		printf("\r\n PASS");
	else
		printf("\r\n FAIL");	
	return;	
}
void TestEcdhPubPuf(void)
{
	unsigned char  sk[32];
	unsigned char  common_key[32];	
	unsigned char  temp_buffer[32];
	size_t  key_length;
	point p1;
	printf("\r\n TEST EcdhGenPubKey");
	EcdhGenPubKeyPuf(&p1);
	printf("\r\n Public key:");
	PrintByte(p1.x,32);
	return;	
}

void TestEcdhSession(void)
{
	unsigned char  sk[32];
	unsigned char  common_key[32];	
	unsigned char  temp_buffer[32];
	size_t  key_length;
	point p1;
	printf("\r\n TEST _EcdhGenSessionKey");
	Hexstr2Bytes("c64d654e263cda95d6dc719d3cfd6c3b932b1fea6021b9e2ac36995c4d96ae3d",sk);
	Hexstr2Bytes("fb526fbfae10d2a0d8fab4d4bdcc883bbfadee2a73ea66a1a1fe816c282d2ce9",p1.y);
	Hexstr2Bytes("764ea0ef1a596b196e8b7316e60de4edccbae87821e767b50f6f36656e7ebe2a",p1.x);
	key_length = 32;
	EcdhGenSessionKey(sk,&p1,common_key,&key_length);


	Hexstr2Bytes("9e29727653fe830e9709045ead243fa44acec4efb7322048894c4d06b484ce58",temp_buffer);
	if(0 != memcmp(temp_buffer,common_key,32) )
		printf("TestEcdhSession COMPARE FAIL");
	else
		printf("TestEcdhSession COMPARE PASS");

	PrintByte(common_key,key_length);

return;	
}
void TestEcdhSessionPuf()
{
	unsigned char  sk[32];
	unsigned char  common_key[32];	
	unsigned char  temp_buffer[32];
	size_t  key_length;
	point p1;
	printf("\r\n TEST _EcdhGenSessionKey");
	Hexstr2Bytes("fb526fbfae10d2a0d8fab4d4bdcc883bbfadee2a73ea66a1a1fe816c282d2ce9",p1.y);
	Hexstr2Bytes("764ea0ef1a596b196e8b7316e60de4edccbae87821e767b50f6f36656e7ebe2a",p1.x);
			    //764ea0ef1a596b196100150020fb526fbfae10d2a0d8fab4d4bdcc883bbfadee
	EcdhGenSessionKeyPuf(&p1,common_key,&key_length);
	printf("\r\n Common key:");
	PrintByte(common_key,32);

return;	

}

void EcdsaGenPublicKeyTest(void)
{
	unsigned char private_key[32];
	unsigned char temp[32];	
	point  public_key;
	Hexstr2Bytes("00d007e1b9afcc312eec9cecffa0280752bbd1953182edef12f3fc366e8f4356",private_key);
	EcdsaGenPublicKey(private_key,&public_key);
	printf("\r\nGet_ECDSA_Public_Key_Yq\r\n");
	PrintByte(public_key.y,32); 
	printf("\r\n Expected\r\n");
	printf("\r\n cf4897766131aa8b7f80453a15bf90f7517878579d5a4f973aea5bb11542e07f");
	Hexstr2Bytes("cf4897766131aa8b7f80453a15bf90f7517878579d5a4f973aea5bb11542e07f",temp);
	if(memcmp(temp,public_key.y,32) == 0)
		printf("\r\n PASS");

	printf("\r\nGet_ECDSA_Public_Key_Xq\r\n");
	PrintByte(public_key.x,32); 
	printf("\r\n Expected\r\n");
	printf("\r\n d6606271131e7e7e617a81aa11f09e7ed56311828823367a869b454040b3f905");
	Hexstr2Bytes("d6606271131e7e7e617a81aa11f09e7ed56311828823367a869b454040b3f905",temp);
	if(memcmp(temp,public_key.x,32) == 0)
		printf("\r\n PASS");
return;		
}




void TestRsaEncrypt()
{
	unsigned char PlainText[256];
	unsigned char ModulusN[256];
	unsigned char PrivateKeyD[256];
	unsigned char CypherText[256];
	unsigned char PublicExpo[256];

	unsigned char ExpectedResult[256];	
	//PlainText
	Hexstr2Bytes("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011223344", PlainText);//RSA_msg
	//ModulusN
	Hexstr2Bytes("F748D8D98ED057CF398C437FEFC615D757D3F8ECE6F2C580AE0780768F9EC83AAA081FF09E5317ED6099C63FD15CFE11172F78908CD58C03AEC93A481FF50E172204AFEDFC1F16AFDB990AAB45BE190BC19259BD4A1BFCDFBE2A298B3C0E318F78A33919882328DACAC85CB35A0DE537B16376975217E5A5EAAF98266B588C2DBAFD0BE371C34989CB36E623D75EFFEDBE4A951A6840982BC279B30FCD41DAC87C0074D462F1012900B8973B46ADC7EAC01770DFC632EA967F9471E9789831F3A410730FF914348BE111863C13376301079756A147D80103CE9FA688A338E22B2D916CAD42D673C9D00F08214DE544F5DE812A9A949189078B2BDA14B28CA62F", ModulusN);//Modulus N
	//PulibcExpo
	Hexstr2Bytes("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001", PublicExpo);//RSA_expo

	RsaPubEnc2048(ModulusN,PublicExpo,CypherText,PlainText,256,0);

	Hexstr2Bytes("EE69099AFD9F99D6065D65E15F90B9237C16987D4872E2B994ED2B9E5685F9BA489AB936CC1E3DFD15B35FEE21536F8C2220AE43217D91D81C9ED01DE5BAEEF4EFC721D70D67B5166E43D82724F39BF0BD197C31E748518DEE63EC10987A08390B15CC4157677C54226A8B04B47684AEDD02B48C8ED48A44BD135397AC2869769B68C7D3BFACDB72AFCD7442C22517E044996CB68E0A311DF5D6D2D286372556F0193166CC364E654EF405DD22FBE584DBF60F0552960668FB69522C1B5264F194FAC9F35622E98227638FF28B910D8CC90E5011021212C96C64C85820877A7D1559235E99C32ABEF33D95E28E18CCA3442E6E3A432FFFEA10104A8EEE94C362", ExpectedResult);	
	if(memcmp(ExpectedResult,CypherText,256) == 0)
		printf("\r\n PASS RSA ENCRYPTION TEST");
	else
		printf("\r\n FAIL RSA ENCRYPTION TEST");
return;		
}

void TestRsaDecrypt()
{

	unsigned char PlainText[256];
	unsigned char ModulusN[256];

	unsigned char CypherText[256];
	unsigned char PrivateKeyD[256];	

	unsigned char ExpectedResult[256];	
	//ModulusN
	Hexstr2Bytes("AE45ED5601CEC6B8CC05F803935C674DDBE0D75C4C09FD7951FC6B0CAEC313A8DF39970C518BFFBA5ED68F3F0D7F22A4029D413F1AE07E4EBE9E4177CE23E7F5404B569E4EE1BDCF3C1FB03EF113802D4F855EB9B5134B5A7C8085ADCAE6FA2FA1417EC3763BE171B0C62B760EDE23C12AD92B980884C641F5A8FAC26BDAD4A03381A22FE1B754885094C82506D4019A535A286AFEB271BB9BA592DE18DCF600C2AEEAE56E02F7CF79FC14CF3BDC7CD84FEBBBF950CA90304B2219A7AA063AEFA2C3C1980E560CD64AFE779585B6107657B957857EFDE6010988AB7DE417FC88D8F384C4E6E72C3F943E0C31C0C4A5CC36F879D8A3AC9D7D59860EAADA6B83BB", ModulusN);//Modulus N
    //Cypher Text		
	Hexstr2Bytes("53EA5DC08CD260FB3B858567287FA91552C30B2FEBFBA213F0AE87702D068D19BAB07FE574523DFB42139D68C3C5AFEEE0BFE4CB7969CBF382B804D6E61396144E2D0E60741F8993C3014B58B9B1957A8BABCD23AF854F4C356FB1662AA72BFCC7E586559DC4280D160C126785A723EBEEBEFF71F11594440AAEF87D10793A8774A239D4A04C87FE1467B9DAF85208EC6C7255794A96CC29142F9A8BD418E3C1FD67344B0CD0829DF3B2BEC60253196293C6B34D3F75D32F213DD45C6273D505ADF4CCED1057CB758FC26AEEFA441255ED4E64C199EE075E7F16646182FDB464739B68AB5DAFF0E63E9552016824F054BF4D3C8C90A97BB6B6553284EB429FCC", CypherText);//Cypher Text	
    //PrivateKeyD		
	Hexstr2Bytes("056B04216FE5F354AC77250A4B6B0C8525A85C59B0BD80C56450A22D5F438E596A333AA875E291DD43F48CB88B9D5FC0D499F9FCD1C397F9AFC070CD9E398C8D19E61DB7C7410A6B2675DFBF5D345B804D201ADD502D5CE2DFCB091CE9997BBEBE57306F383E4D588103F036F7E85D1934D152A323E4A8DB451D6F4A5B1B0F102CC150E02FEEE2B88DEA4AD4C1BACCB24D84072D14E1D24A6771F7408EE30564FB86D4393A34BCF0B788501D193303F13A2284B001F0F649EAF79328D4AC5C430AB4414920A9460ED1B7BC40EC653E876D09ABC509AE45B525190116A0C26101848298509C1C3BF3A483E7274054E15E97075036E989F60932807B5257751E79", PrivateKeyD);//PrivateKeyD
	RsaPubDec2048(PrivateKeyD,ModulusN,PlainText,CypherText,256,0);
	Hexstr2Bytes("009AEFF546462E50BFEC1DC191D5D0CE459069756F33635AD62317FFA3981D2B674ED6E83547E479CA90CEF1EB74CBA8F36004F73B477B159B4FE4F3B5BDA05E51D7C8C674C2B9BD2060C9574E661311F4AD7FFC4C0373F1D987505DE434A32DB898B0D167D188EB9645219D5222EB107A7FAAE431705E1A3DC8F47CD936B96A02D951E997199635E49B523FD01E1D4C00CBD551F395202F771007505E1DD48B7B04A82B892FE728E190B71E6D4128571C9BED19C06123DB3EEA1A4EC645419FC879B98F82B6563B7A2C6280DB9B0434A756502306E0B244459DD012CA7198A6300058121E70917B49F6402EE738A6C60BFEBD3CD130CDFB11392AB73DA9A8CA", ExpectedResult);//RSA_expo 

	if(memcmp(ExpectedResult,PlainText,256) == 0)
		printf("\r\n PASS RSA DECRYPTION TEST");
	else
		printf("\r\n FAIL RSA DECRYPTION TEST");
return;	
}

void ShaBasic()
{
	unsigned char input[3] = {0x61,0x62,0x63};
	unsigned char output[32];
	unsigned char answer[32];
	Hexstr2Bytes("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",answer);
	StandardShaMode(input, output, 3);
	if( 0 == memcmp(answer,output,32) )
		printf("SHA TEST PASS");
	else
		printf("SHA TEST FAIL");
return;		
}
int gSPI_MODE = 0;
#define SPI_0 0
#define SPI_1 1
void SetSPIMode(int SPI_MODE)
{
	if(gSPI_MODE != SPI_MODE)
	{
		if(SPI_MODE == 0)
		{
				Dorca3Cm0Close();
				Dorca3SpiInit(SPI0_SPEED);				
				GenINT0();
				Delay_ms(DALY_SWICHING);
				Reset();
				SetSPI0();

		}
		if(SPI_MODE == 1)
		{

				SET_SPI1();
				Dorca3Close();
				GenINT0();
				Delay_ms(DALY_SWICHING);
				Dorca3Cm0SpiInit(SPI1_SPEED);
		}
		gSPI_MODE = SPI_MODE;

	}
return;		
}
void SetPatternbyType(int type, unsigned char *pBuffer)
{
	int i = 0;
	switch(type)
	{
	case A_EE_CONFIG_NW:
		for(i = 0; i < 6; i++)
			pBuffer[i] = i;
		break;
	case A_EE_CONFIG_FAC:
		for(i = 0; i < 10; i++)
			pBuffer[i] = i +0x11;		
		break;
	case A_EE_CONFIG_UID:
		for(i = 0; i < 10; i++)
			pBuffer[i] = i +0x22;		
		break;
	case A_EE_SEED_KEY:
		for(i = 0; i < 16; i++)
			pBuffer[i] = i +0x33;		
		break;		
	case A_EE_CONFIG_USER:
			pBuffer[0]	= 0x44;
		break;
	case A_EE_CONFIG_LOCK:
		for(i = 0; i < 16; i++)
			pBuffer[i] = i +0x55;		
		break;				break;
	case A_EE_MEM_TEST:
		for(i = 0; i < 64; i++)
			pBuffer[i] = i +066;			
		break;
	case A_EE_MIDR:
		for(i = 0; i < 33; i++)
			pBuffer[i] = i +066;					break;
	default:
		PRINTLOG("\r\n wrong config type ");
		break;
	}
return;	
}
void TestConfig()
{
	unsigned char Buffer[64];
	unsigned char BufferRead[64];
	memset(Buffer,0,64);
	memset(BufferRead,0,64);
/*	
	GetPermission(RG_PERM_UID_PASS,gUID_PW_PT);
	printf("\r\n RG_PERM_UID_PASS");
	SetPatternbyType(A_EE_CONFIG_UID,Buffer);
	SetConifgArea(A_EE_CONFIG_UID,Buffer,6);
	GetConfigArea(A_EE_CONFIG_UID,BufferRead,6);
	if( 0 == memcmp(Buffer,BufferRead,6) )
		printf("\r\n PASS");
	else{
		printf("\r\n FAIL");
		PrintByte(BufferRead,6);
	}
	ReleasePermision();

	memset(Buffer,0,64);
	memset(BufferRead,0,64);
*/	
}
void API_TEST_MAIN()
{
		unsigned char temp ;
		unsigned char out_rand[16];
		int i = 0;
		int j = 0;		
		int iResult = 0;
		unsigned char buf_cipher[64];
		unsigned char buf_result[64];
		unsigned char buf_orginal[64];
		unsigned char law_data[64];	
		unsigned char Key[32];
		unsigned char CIPHERED_KEY[32];
		API_TEST_MAIN_START:
			
		PWWrite();
		
		while(1)
		{
			temp = 'z' ;
	
	
			printf("\r\n");
			printf("\r\n*****************************************************");
			printf("\r\n* 		   API	  TEST MAIN 					 ");
			printf("\r\n*****************************************************");
			printf("\r\n* 1 EcdhGenPubKey					  		 ");
			printf("\r\n* a EcdhGenPubKeyPuf					  		 ");			
			printf("\r\n* 2 EcdhGenSessionKey						 ");			
			printf("\r\n* b EcdhGenSessionKeyPuf					  		 ");	
			printf("\r\n* 4 RSA Encryption            ");
			printf("\r\n* 5 RSA Decryption            ");			
			printf("\r\n* 3 AES with key idx"                );						
			printf("\r\n* c AES/ARIA 16            ");									
			printf("\r\n* d AES/ARIA 32               ");		
			printf("\r\n* 6 ecdsa_gen_pub_key");			
			printf("\r\n* 7 EcdsaGenSignatureTest");
			printf("\r\n* 8 EcdsaVerifySignatureTest PASS is expected");	
			printf("\r\n* g EcdsaVerifySignatureTest FAIL is expected");			
			printf("\r\n* e Cipher Memory");	
			printf("\r\n* f SHA256 hash");				
			printf("\r\n* h RSCreate ");				
			printf("\r\n* i RS SHARead ");							
			printf("\r\n* j KeyMake Test ");				
			printf("\r\n* k RS-DirectRead");
			printf("\r\n* l get_rand_from_spi0");
			//printf("\r\n  * n. Set CFG Area;							");									
			printf("\r\n* p. permission test menu						");						
			printf("\r\n* o. test CFG Area 							");						
			printf("\r\n* t. make random seed key 							");									
			printf("\r\n* m. return to top menu							");			
			printf("\r\n* q. sha auth							");						

			
			printf("\r\n");
			printf("\r\n");
			printf("\r\n  * Select : ");
//	MAKE_RANDOM_SEED_KEY();
//	MAKE_RANDOM_SEED_KEY2();
	//getchar();
			while(temp == 'z')
			{
				int HitCnt = 0;
				int MissCnt = 0;
				temp = _uart_get_char();
	
				if ( temp != 'z' ) printf("%c\n", temp);
				printf("\r\n");
				if(temp == 0x0d)
					goto API_TEST_MAIN_START;
				if( temp == 'm')
					return;
	
				switch ( temp )
				{
					case 'q':
						SHAAUTH_FROM_DORCA();
						SHAAUTH_FROM_MCU();
						break;					
					case 'j':
						KeyMakeTest();
						break;
					case 't':
						MAKE_RANDOM_SEED_KEY();
						break;
					case 'o':
						{
							unsigned char buffer[64];
							unsigned char buffer_return[32];
							for( i = 0; i < 64; i++)
								buffer[i] = i;
							memset(buffer_return,0,32);
							SetConifgArea(RG_PERM_UID_PASS,A_EE_SEED_KEY,gUID_PW_PT,buffer,64);
							GetConfigArea(RG_PERM_UID_PASS,A_EE_SEED_KEY,gUID_PW_PT,buffer_return,32);
							printbyte(buffer,32);
							printbyte(buffer_return,32);
						}
						//MAKE_RANDOM_SEED_KEY();
						//MAKE_RANDOM_SEED_KEY2();
						//MAKE_RANDOM_SEED_KEY3();
					break;
					
					case 'n':
						TestConfig();
						break;
					case 'l':
						GetRandomNumber(out_rand);
						printf("\r\n Random number");
					    PrintByte(out_rand,16);
						break;					
				case 'p':
					GetPermissionMenu();
					break; 
				case 'k':
					{
						unsigned char RS[32];
						SetSPIMode(SPI_0);
						RSDirectRead13(RS,1,gUID_PW_PT);
						printf("\r\n RSx1");
						PrintByte(RS,32);
						RSDirectRead13(RS,3,gUID_PW_PT);
						printf("\r\n RSx3");
						PrintByte(RS,32);						
					  break;					
					}
				case '3':
				{
						unsigned char CT[16];
						unsigned char PT[16];
						unsigned char AES256KEY[32];
						unsigned char AES256CT[16];
						unsigned char AES256PT[16];
						int i = 0;

						SetSPIMode(SPI_0);	
						Hexstr2Bytes("9E35A186018EDF53B36E2A77A5F270EF",AES256CT);	
						Hexstr2Bytes("00112233445566778899aabbccddeeff",AES256PT);
						KeyFromEcdh();
						printf("\r\n . Dorca3CipherDecipherGivenIdx AES MODE_ECB 256 ENC TEST");
//int Dorca3CipherDecipherGivenIdx(int key_idx, int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, size_t len, int type,int last)						
						Dorca3CipherDecipherGivenIdx(0,RG_ENC,1 /*AES*/,0,32,0,CT,AES256PT,16,MODE_ECB,LAST);	
						if( 0 == memcmp(AES256CT,CT,16))
							printf("\r\n PASS");
						else
							printf("\r\n FAIL");

						}
				break;
				case 'i':
					{
					
					unsigned char OutSHA[32];
					SetSPIMode(SPI_0);
					for( i = 0; i <4; i++){
						for(j = 0; j < 2; j++){
							
							RSSHAReadIdx(i,OutSHA);
							printf("================ %d ====================",i);
							PrintByte(OutSHA,32);
							}
					   }
					break;
				   }
					case 'h':
						{
					
						unsigned char userValA[32];
						unsigned char userValB[32];	
						SetSPIMode(SPI_0);						
						for(i = 0; i < 32; i++)
						{
							userValA[i] = rand()&0xff;
							userValB[i] = rand()&0xff;
						}
							RSCreate01();
							RSCreate23(userValA,userValB);
							break;
						}
					case 'f':
						SetSPIMode(SPI_0);
						ShaBasic();
					     break;

					case 'g':
							SetSPIMode(SPI_1);
							EcdsaVerifySignatureTestFail();								
					     break;					

			case 'e' :
				{
				for(i = 0; i < 32; i++)
					Key[i] = i;
				for(i = 0; i <64; i++)
					law_data[i] = i;
				
				
				AES_KEY aes256_ekey,aes256_dkey;
				printf("\r\n  * e. EEPROM TEST  ");							
				AES_set_encrypt_key(Key, 256, &aes256_ekey);
				AES_set_decrypt_key(Key, 256, &aes256_dkey);
				SetSPIMode(SPI_0);

				for( i= 0; i < 16; i += 16)
					AES_ecb_encrypt(&law_data[i], &buf_cipher[i], &aes256_ekey, AES_ENCRYPT);

				EepromWrite(Key, 0,0,1,AES256,buf_cipher);
				EepromRead(Key, 0,0,1,AES256,buf_result);
				
				AES_ecb_encrypt(buf_result, buf_orginal, &aes256_dkey, AES_DECRYPT);	
				if( 0 == memcmp(buf_orginal,	law_data,16)	)
					printf("Test PASS");
				}
			
				break;
			case 'c':
				SetSPIMode(SPI_0);
				AesAriaOperationModeTest();
				break;
			case 'd':
				SetSPIMode(SPI_0);				
				AesAriaOperationModeTest32();
				break;
							case '1':
							printf("\r\n EcdhGenPubKey TEST");
							SetSPIMode(SPI_1);

							TestEcdhPub();
					break;
				case 'a':
							printf("\r\n TestEcdhPubPuf TEST");
							SetSPIMode(SPI_1);

							TestEcdhPubPuf();
					break;					
					case '2':
							printf("\r\n TestEcdhSession TEST");
							SetSPIMode(SPI_1);							
							TestEcdhSession();							
					break;	
					case 'b':
							printf("\r\n TestEcdhSessionPuf TEST");
							SetSPIMode(SPI_1);
							TestEcdhSessionPuf();
							break;
					break;											
					case '4':
						SetSPIMode(SPI_1);
							TestRsaEncrypt();
							break;
					case '5':
							printf("\r\n RSA TEST TestRsaDecrypt");
							SetSPIMode(SPI_1);
							TestRsaDecrypt();
							break;		
					case '6':
							printf("\r\n ecdsa_gen_pub_key TEST");
							SetSPIMode(SPI_1);
							EcdsaGenPublicKeyTest();
						break;

					case '7':
							printf("\r\n EcdsaGenSignatureTest TEST");
							SetSPIMode(SPI_1);							
							EcdsaGenSignatureTest();
						break;
					case '8':
							printf("\r\n EcdsaVerifySignatureTest TEST");
							SetSPIMode(SPI_1);							
							EcdsaVerifySignatureTest();
						break;											
				default : temp = 'p'; break;			
				}
	
			}
		}

		return;	
}

