#include "dorca30_api.h"
#include "dorca30_api_inside.h"
#include "global.h"
#include <stdlib.h>
#if 1
int Dorca3CipherDecipher(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, size_t len, int type,int last)
{
	int dorca3_opmode = 0;
	int dorca3_enc_dec = 0;
	int dorca3_keylength = 0;
	int dorca3_aes_aria = 0;
	int dorca3_two_frame = 0;
	if(32 == len)
	  dorca3_two_frame = 1;
	  
	dorca3_opmode = type;
	dorca3_enc_dec = mode;
	if( 1 == arg_type)
		dorca3_aes_aria = RG_AES;
	else
		dorca3_aes_aria = RG_ARIA;
	if(32 == key_length)
		dorca3_keylength = RG_256;
	else
		dorca3_keylength = RG_128;
	
	#ifdef DEBUG_API
		printf("\r\n mode %d",mode);
		printf("\r\n arg_type %d",arg_type);
		if(0 != Key) {
			printf("\r\n Key");
			PrintByte(Key,key_length);
		}
		if(0 != iv){
			printf("\r\n iv");
			PrintByte(iv,16);
		}
		printf("\r\n in");
		PrintByte(in,16);
		printf("\r\n len %d", len);
		printf("\r\n type %d", type);
		printf("\r\n last %d", last);
		
		
	#endif
	if(0 != Key) {
			if(MODE_ECB == type) {	
				AES_ARIA_INIT(dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame);
			} 
			else{
				if(0 != iv)
					SET_IV(iv,dorca3_opmode,dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame);
			}		
	}
	else {
		if(0 != iv)	
				SET_IV(iv,dorca3_opmode,dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame);
	}
	
	if(16 == len) {
		if( RG_ENC == dorca3_enc_dec)
			AES_ARIA_Encrypt(in,out);
		else
			AES_ARIA_Decrypt(in,out);	
	}
	
	else {
		if( RG_ENC == dorca3_enc_dec)
			AES_ARIA_Encrypt32(in,out);
		else
			AES_ARIA_Decrypt32(in,out);	

	}

	
	if(last){

		AES_ARIA_CLOSE();
		
		if(32 == len){
			Reset();

		}
	}
	return 0;
}
#endif  
int Dorca3CipherDecipherGivenIdx(int key_idx, int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, size_t len, int type,int last)
{
	int dorca3_opmode = 0;
	int dorca3_enc_dec = 0;
	int dorca3_keylength = 0;
	int dorca3_aes_aria = 0;
	int dorca3_two_frame = 0;
	if(32 == len)
	  dorca3_two_frame = 1;
	  
	dorca3_opmode = type;
	dorca3_enc_dec = mode;
	if( 1 == arg_type)
		dorca3_aes_aria = RG_AES;
	else
		dorca3_aes_aria = RG_ARIA;
	if(32 == key_length)
		dorca3_keylength = RG_256;
	else
		dorca3_keylength = RG_128;
	
	#ifdef DEBUG_API
		printf("\r\n mode %d",mode);
		printf("\r\n arg_type %d",arg_type);
		if(0 != Key) {
			printf("\r\n Key");
			PrintByte(Key,key_length);
		}
		if(0 != iv){
			printf("\r\n iv");
			PrintByte(iv,16);
		}
		printf("\r\n in");
		PrintByte(in,16);
		printf("\r\n len %d", len);
		printf("\r\n type %d", type);
		printf("\r\n last %d", last);		
	#endif
	
	if(key_idx > -1 ) {

		if(MODE_ECB == type) {	
			AES_ARIA_INIT_KEY_IDX(key_idx,dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame);
		} 
		else{
			if(0 != iv)
				SET_IV_KEY_IDX(key_idx,iv,dorca3_opmode,dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame);
		}		
			
	}
		
	if(16 == len) {
		if( RG_ENC == dorca3_enc_dec)
			AES_ARIA_Encrypt(in,out);
		else
			AES_ARIA_Decrypt(in,out);	
	}
	
	else {
		if( RG_ENC == dorca3_enc_dec)
			AES_ARIA_Encrypt32(in,out);
		else
			AES_ARIA_Decrypt32(in,out);	

	}

	
	if(last){

		AES_ARIA_CLOSE();
		
		if(32 == len){
			Reset();

		}
	}
	
	else {

	}
		return 0;

}

void EepromRead(unsigned char *Key,int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT) 
{
	EncodeAndRead(Key,SubFrame,SubPage,Page,Algorithm,CT);
	return;
}
void EepromWrite(unsigned char *Key, int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT)
{
	DecodeAndWrite(Key,SubFrame,SubPage,Page,Algorithm,CT);
	return;
}


int  EcdsaVerifySignature(point *public_key, unsigned char  *r,unsigned char  *s,unsigned char  *h  )
{
	 return _EcdsaVerifySignature(public_key, r, s, h);

}

int  EcdsaGenPublicKey(unsigned char *private_key, point *public_key)
{

	_EcdsaGenPublicKey(private_key, public_key);
	return 0;  	
}

int EcdsaGenSignature(unsigned char  *d, unsigned char  *k, unsigned char  *h, unsigned char  *r, unsigned char  *s)
{
	_EcdsaGenSignature(d,k,h,r,s);
	return 0;  	
}
int EcdhGenPubKey(unsigned char * sk,point *p1)
{
	_EcdhGenPubKey(sk,p1);
	return 0;
}
int EcdhGenSessionKey(unsigned char * sk,point *p1, unsigned char  *key,size_t* key_length)
{
	_EcdhGenSessionKey(sk,p1,key,key_length);
	return 0;	
}
int EcdhGenSessionKeyPuf(point *p1, unsigned char  *key,size_t* key_length)
{
	_EcdhGenSessionKeyPuf(p1,key,key_length);
	return 0;
}
int EcdhGenPubKeyPuf(point *p1)
{
	_EcdhGenPubKeyPuf(p1);
	return 0;
}
int RsaPubEnc2048(unsigned char * pub_key_n,unsigned char * pub_key_e,unsigned char * out, unsigned char *in, size_t len,int padding)
{
	_RsaPubEnc2048(pub_key_n,pub_key_e,out,in,len,padding);
	return 0;
}
int RsaPubDec2048(unsigned char * priv_key,unsigned char * pub_key_n,unsigned char * out, unsigned char *in, size_t len,int padding)
{
	_RsaPubDec2048(priv_key,pub_key_n,out,in,len,padding);
	return 0;
}

int WakeUp()
{
	ClearSleepTimer();
	return 0;
}
