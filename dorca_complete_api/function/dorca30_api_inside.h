

#ifndef DORCA30_INSIDE_API
#define DORCA30_INSIDE_API 



int AES_ARIA_INIT_KEY_IDX(int Index,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY,int RG_TWO_FRAME);
void SET_IV_KEY_IDX(int Index,unsigned char *IV,int AES_OPMODE,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY, int RG_TWO_FRAME);	
void SET_IV(unsigned char *IV,int AES_OPMODE,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY,int TWO_FRAME);
int AES_ARIA_INIT(int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY,int TWO_FRAME);
void AES_ARIA_CLOSE();
void AES_ARIA_Decrypt(unsigned char *pInput, unsigned char *pOutput);
void AES_ARIA_Encrypt(unsigned char *pInput, unsigned char *pOutput);
void AES_ARIA_Decrypt32(unsigned char *pInput, unsigned char *pOutput);
void AES_ARIA_Encrypt32(unsigned char *pInput, unsigned char *pOutput);
void EncodeAndRead(unsigned char *Key,int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT);
void DecodeAndWrite(unsigned char *Key, int SubFrame,int SubPage,int Page, int Algorithm,unsigned char *CT);
void StandardShaMode(unsigned char *txdata, unsigned char *rxdata, long long ByteNo);
int _EcdhGenPubKey(unsigned char * sk,point *p1);
int _EcdhGenSessionKey(unsigned char * sk,point *p1, unsigned char  *key,size_t* key_length);
int _EcdhGenSessionKeyPuf(point *p1, unsigned char  *key,size_t* key_length);
int _EcdhGenPubKeyPuf(point *p1);
int _EcdsaVerifySignature(point *public_key, unsigned char  *r,unsigned char  *s,unsigned char  *h);
int _EcdsaGenPublicKey(unsigned char *private_key, point *public_key);
int _EcdsaGenSignature(unsigned char  *d, unsigned char  *k, unsigned char  *h, unsigned char  *r, unsigned char  *s);
int _RsaPubEnc2048(unsigned char * pub_key_n,unsigned char * pub_key_e,unsigned char * out, unsigned char *in, size_t len,int padding);
int _RsaPubDec2048(unsigned char * priv_key,unsigned char * pub_key_n,unsigned char * out, unsigned char *in, size_t len,int padding);

void AES_ARIA_ECB_TEST_ETRI_MAIN();
void SHA_TEST_MAIN();

#endif //DORCA30_INSIDE_API