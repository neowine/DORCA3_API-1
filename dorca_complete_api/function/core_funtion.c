#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "dorca30_function.h"
#include "dorca30_api.h"
#include "dorca30_api_inside.h"
#include "dorca.h"
#include "define.h"
#include "global.h"
#include "include/aes.h"
#include "include/aes_locl.h"
#define ARM7 
#define DelayMS 30
//#define KeySetup(x) (void)0
#define CODE_DOWNLOAD
//#define DEBUG_DELAY
#if 1
#define PRINTLOG(...) printf(__VA_ARGS__)
#else
#define PRINTLOG(...) (void)0
#endif

int cs = 0;
int fd0;
int fd1;
extern unsigned char gAES_KEY_SEED[32];

void _KeyFromEcdh(void)
{
	//KeyLoadDemo(0/*KeyAseCtrl*/,1/*TextSel*/,0/*KeySel*/,1/*KeySaveSel*/,0/*KeyAseCtrl*/,0/*KeyAseCtrl*/);
	 KeyLoadIDX(0,1,0,1,0,MODE256, 0);
	//int KeyLoadDemo(int KeyAseCtrl,int TextSel,int KeySel, int KeySaveSel, unsigned char *LoadKEY ,int mode)	
	return;
}


void KeyFromEcdh(void){
	_KeyFromEcdh();
	return;
}


void KeySetup(unsigned char *KEY)
{		
	//KeyLoadDemo2(0,0,0,0,KEY,MODE256);
	KeyLoadIDX(0,0,0,0,KEY,MODE256, gAES_KEY_SEED);
	return;
}
void PrintByte(unsigned char *pData,int size )
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
unsigned char tspi_interface(unsigned int cs, unsigned char inst, unsigned char *addr,unsigned char count, unsigned char opcode,unsigned char param0, unsigned char param1, unsigned char *tx_data, unsigned char *rx_data, int Byte_num)
{
	int addr_temp = 0;
	addr_temp = (addr[0] << 8) | addr[1]; 
	if(inst == 0x31 || inst == 0x30){// write
		SpiWrite(fd0,inst,addr_temp,Byte_num,tx_data);
		//printf("\r\n SpiWrite(fd,inst,addr_temp,Byte_num,tx_data)");
	}
	else{//read
		SpiRead(fd0,inst,addr_temp,Byte_num,rx_data);
		//printf("\r\n SpiRead(fd,inst,addr_temp,Byte_num,rx_data)");    	
	}	
	return 0;
}

void Hexstr2Bytes( char * str, unsigned char  * result) {
	int i, n;
	char tmp[3] = "00";

	n=strlen(str)/2;
	for(i=0; i<n; i++)
	{
		memcpy(tmp, &str[i*2], 2);
		result[i] = (unsigned char ) strtoul(tmp, 0, 16);
	}
	return;
}




void delay_us(unsigned int us)	
{
	//printf("\r\n Delay");
	//usleep(us*2);
	usleep(us);
	return;
}
void Delay_us(unsigned int us)	
{
	//printf("\r\n Delay");
	//usleep(us*2);
	usleep(us);
	return;
}
void Delay_ms(unsigned int us)	
{
	//printf("\r\n Delay");
	//usleep(us*1000*2);
	usleep(us*1000);
	return;
}

void delay_ms(unsigned int i)
{
	//	Delay_ms(i*2);
	Delay_ms(i);
	return;
}


char get_char() 
{
	return getchar();
}

char _uart_get_char()
{
	return getchar();

}
void EndOperation(void)
{

	int i;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	int j = 0;


	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE, 0, 0, 0, 0, tx_data, rx_data, 1);
	Delay_ms(2);
	tx_data[0] = 0x00;	
	tspi_interface(cs, ADDR_NOR_W, RG_ACCESS, 0, 0, 0, 0, tx_data, rx_data, 1);


#if PRINTFMODE
	// PRINTLOG("\r\n=========================================================================");
	//PRINTLOG("\r\n==		 RG_ACCESS SET=> 0x00							   ==");
	// PRINTLOG("\r\n=========================================================================");
#endif	 
	return;
}

void reversebuffer(unsigned char *dest,unsigned char *org,int count)
{
	int i;
	int j;
	j = count -1;
	for(i = 0; i < count; i++) {
		dest[i] = org[j--];
	}

	return;
}


int AES_ARIA_INIT_KEY_IDX(int Index,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY,int RG_TWO_FRAME)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char key_buffer[32];

	tx_data[0] = Index;// KEY_0
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 
		(RG_TWO_FRAME<<3)|
		(RG_128_256<<1)|
		AES_ARIA;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x9;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x3;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	delay_us(30);
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x4;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}


int AES_ARIA_INIT(int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY,int RG_TWO_FRAME)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char key_buffer[32];
	// INIT KEY
#if 0
	if(RG_128_256 == RG_256)
	{
		for(i = 0; i < 32; i++)
		{
			AES_ARIA_KEY[i] = i;
		}
	}	
	else
	{
		for(i =0; i <16; i++)
		{
			AES_ARIA_KEY[i] = i;
		}
		for(i = 16; i < 32; i++)
			AES_ARIA_KEY[i] = 0;
	}
#endif

	if(RG_128_256 == RG_256)
	{
		memcpy(key_buffer,AES_ARIA_KEY+16,16);
		memcpy(key_buffer+16,AES_ARIA_KEY,16);		
		KeySetup(key_buffer);
#ifdef DEBUG_API			
		printf("\r\n RG_256");
		PrintByte(AES_ARIA_KEY,32);

#endif
	}
	else
	{
		memcpy(key_buffer+16,AES_ARIA_KEY,16);
		KeySetup(key_buffer);	
#ifdef DEBUG_API				
		printf("\r\n RG_128");		
		PrintByte(AES_ARIA_KEY,16);

#endif
	}
#ifdef DEBUG_API		

	printf("\r\n RG_128_256 %d",RG_128_256);
	printf("\r\n AES_ARIA %d",AES_ARIA);	
	printf("\r\n RG_TWO_FRAME %d",RG_TWO_FRAME);
#endif	
	tx_data[0] = 0x0;// KEY_0
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 
		(RG_TWO_FRAME<<3)|
		(RG_128_256<<1)|
		AES_ARIA;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x9;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x3;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	delay_us(30);
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x4;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

void AES_ARIA_Encrypt32(unsigned char *pInput,unsigned char *pOutput)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
#ifdef DEBUG_API		
	printf("\r\n AES_ARIA_Encrypt32 input");
	PrintByte(pInput,32);
#endif

	reversebuffer(tx_data, pInput, 16);
	reversebuffer(tx_data+16,pInput+16,16);
	tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300                   , 0, 0, 0, 0, tx_data, rx_data, 32);	
	Delay_us(2);

	tspi_interface(cs, ADDR_NOR_R,RG_EEBUF320                   , 0, 0, 0, 0, tx_data, rx_data, 32);	

	reversebuffer(pOutput, rx_data, 16);
	reversebuffer(pOutput+16,rx_data+16,16);	
#ifdef DEBUG_API		
	printf("\r\n AES_ARIA_Encrypt32 pOutput");
	PrintByte(pOutput,32);
#endif	
	return;

}
void AES_ARIA_Decrypt32(unsigned char *pInput,unsigned  char *pOutput)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];

	reversebuffer(tx_data, pInput, 16);
	reversebuffer(tx_data+16,pInput+16,16);
#ifdef DEBUG_API	
	printf("\r\n AES_ARIA_Decrypt32 input");
	PrintByte(pInput,32);
#endif	
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400      , 0, 0, 0, 0, tx_data, rx_data, 32);	
	delay_us(2);
	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF420      , 0, 0, 0, 0, tx_data, rx_data, 32);		
	//iEnd = pRSTC->RTTC_RTVR;
	reversebuffer(pOutput, rx_data, 16);
	reversebuffer(pOutput+16,rx_data+16,16);
#ifdef DEBUG_API		
	printf("\r\nAES_ARIA_Decrypt32 pOutput");
	PrintByte(pOutput,32);	
#endif	
	return;
}


void AES_ARIA_Encrypt(unsigned char *pInput,unsigned char *pOutput)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	reversebuffer(tx_data, pInput, 16);
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 16);	
	delay_us(2);
	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF320      , 0, 0, 0, 0, tx_data, rx_data, 16);		
	//iEnd = pRSTC->RTTC_RTVR;
	reversebuffer(pOutput, rx_data, 16);	

	return;
}
void AES_ARIA_Decrypt(unsigned char *pInput,unsigned  char *pOutput)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];

	reversebuffer(tx_data, pInput, 16);
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400      , 0, 0, 0, 0, tx_data, rx_data, 16);	
	delay_us(2);
	tspi_interface(cs, ADDR_NOR_R, RG_EEBUF420      , 0, 0, 0, 0, tx_data, rx_data, 16);		
	//iEnd = pRSTC->RTTC_RTVR;
	reversebuffer(pOutput, rx_data, 16);	
	return;
}
void AES_ARIA_CLOSE()
{

	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];

	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE    , 0, 0, 0, 0, tx_data, rx_data, 1);	  


	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE    , 0, 0, 0, 0, tx_data, rx_data, 1);	  


	EndOperation();				
	return;
}

void SET_IV_KEY_IDX(int Index,unsigned char *IV,int AES_OPMODE,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY, int RG_TWO_FRAME)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];	   
	int success = 1;
	unsigned char key_buffer[32];

	tx_data[0] = Index;// KEY_0
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 
		(AES_OPMODE<<4)|
		(RG_TWO_FRAME<<3)|
		(RG_128_256<<1)|
		AES_ARIA;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x9;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	reversebuffer(tx_data,IV,16);
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 16);	
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	tx_data[0] = 0x3;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	delay_us(30);
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x4;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return;
}

void SET_IV(unsigned char *IV,int AES_OPMODE,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY, int RG_TWO_FRAME)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];	   
	int success = 1;
	unsigned char key_buffer[32];
	memset(key_buffer,0,32);
	//printf("\r\n -----------------START OF SET IV-----------------");
	if(0 != AES_ARIA_KEY) {

		if(RG_128_256 == RG_256){		

			memcpy(key_buffer,AES_ARIA_KEY+16,16);
			memcpy(key_buffer+16,AES_ARIA_KEY,16);		
			KeySetup(key_buffer);
		}
		else{

			memcpy(key_buffer+16,AES_ARIA_KEY,16);
			KeySetup(key_buffer);	
		}
	}
	//else
	//	printf("\r\nKEY IS 0");
	//if(0 == IV)
	//	printf("\r\n IV IS 0");
	//else{
	//	printf("\r\nIV:");
	//	PrintByte(IV,16);
	//}

	tx_data[0] = 0x0;// KEY_0
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 
		(AES_OPMODE<<4)|
		(RG_TWO_FRAME<<3)|
		(RG_128_256<<1)|
		AES_ARIA;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x9;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	tx_data[0] = 0x2;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	reversebuffer(tx_data,IV,16);
	tspi_interface(cs, ADDR_NOR_W, RG_EEBUF300      , 0, 0, 0, 0, tx_data, rx_data, 16);	
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	

	tx_data[0] = 0x3;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	delay_us(30);
	tx_data[0] = 0x1;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0x4;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return;
}




int KeyLoadIDX(int KeyAseCtrl,int TextSel,int KeySel, int KeySaveSel, unsigned char *LoadKEY ,int mode, unsigned char *pPrevKey)
	//int KeyLoadDemo(int KeyAseCtrl,int TextSel,int KeySel, int KeySaveSel)
{
	int i;
	int j;
	unsigned int inst = 0;
	int pass = 1;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char temp ;
	int success = 1;

	unsigned char *cypkey;
	unsigned char ciphered_key[32];


	memset(tx_data,0,64);
	memset(rx_data,0,64);


	if(LoadKEY != 0)
	{
		AES_KEY aes256_ekey,aes256_dkey;
		AES_set_encrypt_key(pPrevKey, 256, &aes256_ekey);
		AES_set_decrypt_key(pPrevKey, 256, &aes256_dkey);

		AES_ecb_encrypt(LoadKEY, ciphered_key, &aes256_ekey, AES_ENCRYPT);
		if(mode == MODE256)		
			AES_ecb_encrypt(LoadKEY+16, ciphered_key+16, &aes256_ekey, AES_ENCRYPT);		
	}

#if PRINTFMODE_PERMISSION
	PRINTLOG("\r\n\n");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	PRINTLOG("\r\n==       KEYLOAD   TEST                                    ==");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif 

#if PRINTFMODE_PERMISSION
	PRINTLOG("\r\n\n");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	PRINTLOG("\r\n==       WRITE SEED KEY                                   ==");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif 
	////gPrintOut = 0;
	/*
	{
	//GetSuperWirePermission();
	if(eep_page_write(ADDR_EE_SEED_KEY[0], ADDR_EE_SEED_KEY[1], buf_data, 1) )
	PRINTLOG("\r\nSUCCESS TO WRITE SEEDKEY");
	else
	PRINTLOG("\r\nFAIL TO WRITE SEEDKEY");

	}
	*/
	delay_ms(10);

	//PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");			
	//PRINTLOG("\r\n++BEFORE KEY LOAD READ EE_KEY_AES_x");
#if 0
	switch(KeyAseCtrl)
	{
	case 0:	
		PRINTLOG("%d",KeyAseCtrl );
		eep_page_read(ADDR_EE_KEY_AES_x0[0],ADDR_EE_KEY_AES_x0[1],0,0);
		break;
	case 1:
		PRINTLOG("%d",KeyAseCtrl );			
		eep_page_read(ADDR_EE_KEY_AES_x1[0],ADDR_EE_KEY_AES_x1[1],0,0);
		break;
	case 2:
		PRINTLOG("%d",KeyAseCtrl );			
		eep_page_read(ADDR_EE_KEY_AES_x2[0],ADDR_EE_KEY_AES_x2[1],0,0);
		break;
	case 3:
		PRINTLOG("%d",KeyAseCtrl );			
		eep_page_read(ADDR_EE_KEY_AES_x3[0],ADDR_EE_KEY_AES_x3[1],0,0);	
		break;
	}
#endif
	//PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");			
#if PRINTFMODE
	PRINTLOG("\r\n\n");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	PRINTLOG("\r\n==       KEY LOAD DEMO START                                   ==");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif 
	//g
	//gPrintOut = 1
	//PRINTLOG("\r\n KeyAseCtrl: %d KeySel:%d  TextSel:%d,KeySaveSel:%d",	KeyAseCtrl,KeySel,TextSel,KeySaveSel);
	tx_data[0] = KeyAseCtrl;// EE_KEY_AES_x0
	tspi_interface(cs, ADDR_NOR_W, RG_EE_KEY_AES_CTRL  , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] =  (KeySaveSel<<4)//KL_KeySaveSel
		|(TextSel<<2)
		|(KeySel); //KL_KEYSEL
	//PRINTLOG("RG_KL_CTRL 0x%02x",tx_data[0]);
	tspi_interface(cs, ADDR_NOR_W, RG_KL_CTRL   , 0, 0, 0, 0, tx_data, rx_data, 1);
	/*
	if(TextSel == 2)
	{
	for( i = 0; i < 64; i++)
	tx_data[i] = i;
	WriteRGEBUF(tx_data);
	}
	*/
	tx_data[0] = 0x01;// AES_256
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL  , 0, 0, 0, 0, tx_data, rx_data, 1);


	tx_data[0] = 0x09;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE  , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x06;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE  , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x03;
	tspi_interface(cs, ADDR_NOR_W, RG_ST2_SYMCIP_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);
	delay_us(30);

	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W,  RG_ST2_SYMCIP_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x09;
	tspi_interface(cs, ADDR_NOR_W,  RG_ST2_SYMCIP_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x02;
	tspi_interface(cs, ADDR_NOR_W,  RG_ST3_SYMCIP_KEYLOAD_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W,  RG_ACCESS    , 0, 0, 0, 0, tx_data, rx_data, 1);
	j = 15;
#if 1
	if(TextSel == 0)
	{
		//		for(i = 0; i < 16; i++)
		//			tx_data[i] = cypkey[j--];


		if(LoadKEY != 0)
		{
			j = 31;
			for(i = 0; i < 16; i++)
				tx_data[i] = ciphered_key[j--];
		}
		tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400   , 0, 0, 0, 0, tx_data, rx_data, 16);
	}
#else
	if(TextSel == 0)
	{
		memset(tx_data,0,64);
		for(i = 16; i < 32; i++)
			tx_data[i] = cypkey[j--];
		tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400   , 0, 0, 0, 0, tx_data, rx_data, 16);
	}

#endif
	//	delay_us(70);
	delay_us(70*2);

	tx_data[0] = 0x03;
	tspi_interface(cs, ADDR_NOR_W,  	RG_ST3_SYMCIP_KEYLOAD_OPMODE     , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W,  RG_ACCESS    , 0, 0, 0, 0, tx_data, rx_data, 1);
	if(TextSel == 0)
	{


		if(LoadKEY != 0)
		{
			if(mode == MODE256)
			{
				j = 15;
				for(i = 0; i < 16; i++)
					tx_data[i] = ciphered_key[j--];
				tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400   , 0, 0, 0, 0, tx_data, rx_data, 16);				
			}
		}
		else
		{
			j = 15;
			for(i = 0; i < 16; i++)
				tx_data[i] = cypkey[j--];
			tspi_interface(cs, ADDR_NOR_W, RG_EEBUF400   , 0, 0, 0, 0, tx_data, rx_data, 16);
		}
	}
	//	delay_us(100);
	delay_ms(16);

	if(TextSel == 2)
	{
		tx_data[0] = 0x01;
		tspi_interface(cs, ADDR_NOR_W, RG_ST1_OKA_OPMODE, 0, 0, 0, 0, tx_data, rx_data, 1);
	}
	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W,  RG_ST3_SYMCIP_KEYLOAD_OPMODE      , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W,  RG_ST2_SYMCIP_OPMODE     , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST1_SYMCIP_OPMODE   , 0, 0, 0, 0, tx_data, rx_data, 1);

	tx_data[0] = 0x01;
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE  , 0, 0, 0, 0, tx_data, rx_data, 1);

#if PRINTFMODE
	PRINTLOG("\r\n\n");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	PRINTLOG("\r\n==       KEY LOAD DEMO END                                   ==");
	PRINTLOG("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif 

	//gPrintOut = 0;
	delay_ms(16);
	EndOperation();
#if 0	
	printf("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");			
	printf("\r\n++AFTER KEY LOAD READ EE_KEY_AES_x");
	eep_page_read(ADDR_EE_KEY_AES_x0[0],ADDR_EE_KEY_AES_x0[1],0,rx_data);
	eep_page_read(ADDR_EE_KEY_AES_x1[0],ADDR_EE_KEY_AES_x1[1],0,rx_data);
	eep_page_read(ADDR_EE_KEY_AES_x2[0],ADDR_EE_KEY_AES_x2[1],0,rx_data);
	eep_page_read(ADDR_EE_KEY_AES_x3[0],ADDR_EE_KEY_AES_x3[1],0,rx_data);		
	printf("\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");			
#endif	
	return success;
}





void ClearSleepTimer()
{
	unsigned int i;
	unsigned char inst = 0x00;
	unsigned char addr[2] = { 0x06, 0x04};
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	unsigned char value0 = 0; 
	unsigned char value1 = 1; 
	for( i=0; i<64; i++)
	{
		tx_data[i] = 0x00;
		rx_data[i] = 0x00;
	}

	inst = 0x31;
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W, RG_SLEEP_TIMER_MSB, 0, 0, 0, 0, tx_data, rx_data, 1);
	inst = 0x31;
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W, RG_SLEEP_TIMER_LSB, 0, 0, 0, 0, tx_data, rx_data, 1);

	return;
}


void SetSPI0()
{

	unsigned int i;
	unsigned int inst = 0x00;
	unsigned char addr[2] = { 0x06, 0x04};
	unsigned char tx_data[64];
	unsigned char rx_data[64];


	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL, 0, 0, 0, 0, tx_data, rx_data, 1);

	//	printf("\r\n Set RG_ST0_OPMODE Start"); 
	tx_data[0] = 1;
	tspi_interface(cs, ADDR_NOR_W,RG_ST1_STDSPI_OPMODE 				  , 0, 0, 0, 0, tx_data, rx_data, 1);	
	EndOperation();	
	//	printf("\r\n SET SPI 0");
	return;
}

unsigned char SET_SPI1()
{

	unsigned int i;
	unsigned char inst = 0x00;
	unsigned char addr[2] = { 0x06, 0x04};
	unsigned char tx_data[64];
	unsigned char rx_data[64];

	for( i=0; i<64; i++)
	{
		tx_data[i] = 0x00;
		rx_data[i] = 0x00;
	}
	//ClearSleepTimer();
	//printf("\r\n Set RG_AES_CTRL Start"); 

	tx_data[0] = 0x03;
	tspi_interface(cs, ADDR_NOR_W, RG_AES_CTRL, 0, 0, 0, 0, tx_data, rx_data, 1);
	//tspi_interface(cs, ADDR_NOR_R, RG_AES_CTRL, 0, 0, 0, 0, tx_data, rx_data, 1);

	//printf("\r\n Set RG_ST0_OPMODE Start"); 

	tx_data[0] = 0x05;	
	tspi_interface(cs, ADDR_NOR_W, RG_ST0_OPMODE, 0, 0, 0, 0, tx_data, rx_data, 1);
	//tspi_interface(cs, ADDR_NOR_R, RG_ST0_OPMODE, 0, 0, 0, 0, tx_data, rx_data, 1);

	//printf("\r\n RG_RNDGEN_USER");

	tx_data[0] = 0x00;		  
	tspi_interface(cs, ADDR_NOR_W, RG_RNDGEN_USER, 0, 0, 0, 0, tx_data, rx_data, 1);
	//tspi_interface(cs, ADDR_NOR_R, RG_RNDGEN_USER, 0, 0, 0, 0, tx_data, rx_data, 1);
	//PRINTLOG("\r\n spi_rx_data    :"); for ( i=0; i<1; i++ ){ if ( ( i !=0 ) & ( i % 16 == 0 ) ) PRINTLOG("\n                 "); PRINTLOG(" 0x%02x", rx_data[i]); }
	//PRINTLOG("\r\n Set RG_ST0_OPMODE End"); 
	return 0;
}
void KeyCreatXn(int index,int mode,unsigned char *prevKey,unsigned char *key)
{
	KeyLoadIDX(index,0,index,0,key,mode,prevKey);
	return;
}



void Reset(void)
{
	int i = 0;
	int j = 0;
	unsigned int inst = 0;
	int success =  1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	tx_data[0] = 0x1;
	tspi_interface(cs, ADDR_NOR_W,RG_SOFT_RESET                   , 0, 0, 0, 0, tx_data, rx_data, 1);	
	Delay_ms(16);
	tx_data[0] = 0x00;
	tspi_interface(cs, ADDR_NOR_W,RG_SOFT_RESET                   , 0, 0, 0, 0, tx_data, rx_data, 1);		
	Delay_ms(16);
	Delay_us(5);
	Delay_ms(16);
	printf("\r\n RESET");
	return;
}



