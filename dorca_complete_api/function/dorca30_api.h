#ifndef DORCA30_API
#define DORCA30_API 
#include <stdint.h>
#define LAST 1

typedef unsigned int size_t; 
typedef enum
{
	MODE_ECB = 0,
	MODE_CBC,
	MODE_OFB,
	MODE_CTR,
	MODE_CFB
}RG_AES_OPMODE;
	
typedef enum
{
	RG_256 = 0,
	RG_128
}RG_MODE;
typedef enum
{
	RG_ARIA = 0,
	RG_AES
}RG_ALGO;
typedef enum
{
	RG_ENC = 0,
	RG_DEC
}RG_ENCDEC;


typedef enum{
	MODE256 = 1,
	MODE128 = 0
}KEY_SAVE_MODE;

typedef struct _point
{
unsigned char  x[32];
unsigned char  y[32];
}point;

#define	AES128 3
#define AES256 1
#define	ARIA128 2
#define ARIA256 0

#if 1
int Dorca3CipherDecipher(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, size_t len, int type,int last);
#endif
int Dorca3CipherDecipherGivenIdx(int key_idx, int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, size_t len, int type,int last);
void EepromRead(unsigned char *Key,int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT) ;
void EepromWrite(unsigned char *Key, int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT);
int EcdsaVerifySignature(point *public_key, unsigned char  *r,unsigned char  *s,unsigned char  *h);
int EcdsaGenPublicKey(unsigned char *private_key, point *public_key);
int EcdsaGenSignature(unsigned char  *d, unsigned char  *k, unsigned char  *h, unsigned char  *r, unsigned char  *s);
int RsaPubEnc2048(unsigned char * pub_key_n,unsigned char * pub_key_e,unsigned char * out, unsigned char *in, size_t len,int padding);
int RsaPubDec2048(unsigned char * priv_key,unsigned char * pub_key_n,unsigned char * out, unsigned char *in, size_t len,int padding);
int EcdhGenPubKey(unsigned char * sk,point *p1);
int EcdhGenPubKeyPuf(point *p1);
int EcdhGenSessionKey(unsigned char * sk,point *p1, unsigned char  *key,size_t* key_length);
int EcdhGenSessionKeyPuf(point *p1, unsigned char  *key,size_t* key_length);
void StandardShaMode(unsigned char *txdata, unsigned char *rxdata, long long ByteNo);
void RSSHAReadIdx(int index, unsigned char *out);
void RSCreate23(unsigned char *pUservalue32A,unsigned char *pUservalue32B);
void RSCreate01(void);
void KeyCreatXn(int index,int mode,unsigned char *prevKey,unsigned char *key);
void KeyFromEcdh(void);
void RSDirectRead13(unsigned char *pRS, int index,unsigned char *UID_PW_PT);
void GetRandomNumber(unsigned char *pOut);
int WakeUp();
int GetPermissionByPW(unsigned char * SUPER_PW_CT, int TYPE, int index);
int GetPermission(int iType, unsigned char *pchPW, int index);
int ReleasePermision();
void SetConifgArea(int permType,int CfgName,unsigned char *pPW_PT, unsigned char *pBuffer, int length);
void GetConfigArea(int permType,int CfgName,unsigned char *pPW_PT, unsigned char *pBuffer, int length);


//#define DEBUG_API



#endif //DORCA30_API
