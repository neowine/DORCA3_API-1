unsigned char ADDR_NOR_W = { 0x31 };
unsigned char ADDR_NOR_R = { 0x21 }; 
unsigned char COMM_NOR_W = { 0x11 };
unsigned char COMM_NOR_R = { 0x01 };
unsigned char MIDR_INCDEC[2] = {0xED,0x20};
unsigned char MIDR_CNT0[2] = {0xED,0x00};
unsigned char MIDR_CNT1[2] = {0xED,0x10};
unsigned char ADDR_SUPER_PW[2] = {0xED,0x80};
unsigned char ADDR_DETOUR_PW[2] = {0xED,0xC0};
unsigned char ADDR_DESTORY0_PW[2] = {0xEE,0x00};
unsigned char ADDR_DESTORY1_PW[2] = {0xEE,0x40};
unsigned char ADDR_EEPROM_PW[2] = {0xEE,0x80};
unsigned char ADDR_UID_PW[2] = {0xEE,0xC0};
#define R402
#ifdef R402
unsigned char ADDR_SUPER_PW_CNT[2] = {0xEF,0xB0};
unsigned char ADDR_DETOUR_PW_CNT[2] = {0xEF,0xF0};
unsigned char ADDR_DESTORY0_PW_CNT[2] = {0xF0,0x30};
unsigned char ADDR_DESTORY1_PW_CNT[2] = {0xF0,0x70};
unsigned char ADDR_EEPROM_PW_CNT[2] = {0xF0,0xB0};
unsigned char ADDR_UID_PW_CNT[2] = {0xF0,0xF0};
#else
unsigned char ADDR_SUPER_PW_CNT[2] = {0xED,0x90};
unsigned char ADDR_DETOUR_PW_CNT[2] = {0xED,0xD0};
unsigned char ADDR_DESTORY0_PW_CNT[2] = {0xEE,0x10};
unsigned char ADDR_DESTORY1_PW_CNT[2] = {0xEE,0x50};
unsigned char ADDR_EEPROM_PW_CNT[2] = {0xEE,0x90};
unsigned char ADDR_UID_PW_CNT[2] = {0xEE,0x90};

#endif

unsigned char ADDR_SUPER_PW_CNT_PAGE[2] = {0xEF,0x80};
unsigned char ADDR_DETOUR_PW_CNT_PAGE[2] = {0xEF,0xC0};
unsigned char ADDR_DESTORY0_PW_CNT_PAGE[2] = {0xF0,0x00};
unsigned char ADDR_DESTORY1_PW_CNT_PAGE[2] = {0xF0,0x40};
unsigned char ADDR_EEPROM_PW_CNT_PAGE[2] = {0xF0,0x80};
unsigned char ADDR_UID_PW_CNT_PAGE[2] = {0xF0,0xC0};
unsigned char ADDR_EE_KEY_AES_x0[2] = {0xE9,0x00};
unsigned char ADDR_EE_KEY_AES_x1[2] = {0xE9,0x40};
unsigned char ADDR_EE_KEY_AES_x2[2] = {0xE9,0x80};
unsigned char ADDR_EE_KEY_AES_x3[2] = {0xE9,0xC0};
unsigned char ADDR_EE_KEY_RS_x3[2] = {0xEA,0xC0};// 
unsigned char ADDR_EE_MEM_BKUP[2] = {0xEF,0x40};
unsigned char ADDR_EE_SEED_KEY[2] = {0xEC,0x00};
unsigned char EE_MEM_BKUP_RSFLAG[2] = {0xEF,0x00};
//**************************************************************************
//* Instruction End
//**************************************************************************


//**************************************************************************
//* Register Address
//**************************************************************************

unsigned char RG_BT_LOGIC_SEL[2] = {0x0C,0xA1};

unsigned char RG_ST0_OPMODE[2]        = { 0x06, 0x04 };
unsigned char RG_EET_CTRL[2]          = { 0x06, 0xB0 };
unsigned char RG_EET_OPMODE[2]        = { 0x06, 0xB1 };
unsigned char RG_EET_BYOB_LEN[2]      = { 0x06, 0xB2 };
unsigned char RG_EET_BYOB_ADDR_LSB[2] = { 0x06, 0xB3 };
unsigned char RG_ST1_RND_OPMODE[2] = {0x06,0x08};
unsigned char RG_AES_CTRL[2] = {0x06,0x35};
unsigned char RG_OKA_CTRL[2] = {0x06,0x3C};
unsigned char RG_ST1_MIDR_OPMODE[2] = {0x06,0x0B};
unsigned char RG_RNDGEN_USER[2] = {0x07,0x00};
unsigned char RG_SUPER_WIRE_PW0[2] = {0x06,0x60};
unsigned char RG_EE_KEY_AES_CTRL [2] = {0x06,0x20};
unsigned char RG_PERM_GET_CTRL [2] = {0x06,0x26};
unsigned char RG_ST2_SYMCIP_OPMODE  [2] = {0x06,0x19};
unsigned char RG_PERM_GET_CTRL1   [2] = {0x06,0x27};
unsigned char RG_PERM_RELEASE   [2] = {0x06,0x28};
unsigned char RG_ST1_OKA_OPMODE[2] = {0x06,0x0A};
unsigned char RG_ST1_SYMCIP_OPMODE[2] = {0x06,0x09};
unsigned char RG_ST3_SYMCIP_RSCREATE_OPMODE[2] = {0x06,0x1D};
unsigned char RG_ST3_SYMCIP_KEYLOAD_OPMODE[2] = {0x06,0x1F};
unsigned char RG_FFFF[2] = {0x0F,0xFF};
unsigned char RG_EE_USER_ZONE_SEL[2] = {0x06,0x1A};
unsigned char RG_PERM_GET_EE_RD_PRE_SP[2] = {0x06,0x29};
unsigned char RG_EE_CFG_RD_RG_EEBUF_ST[2] = {0x06,0x1C};
unsigned char RG_MCUAuthResult [2] = {0x07,0x20};
unsigned char RG_ST2_SYMCIP_SHAAuth_CMP_DP [2] = {0x07,0x21};
unsigned char RG_EETEST_BYOB_ADDR_LSB[2] = {0x06,0xB3};

unsigned char RG_SHA_CTRL[2] = {0x06,0x38};
unsigned char RG_ST1_STDSPI_OPMODE [2] = {0x06,0x06};


unsigned char RG_EEBUF100[2] = {0x01,0x00};
unsigned char RG_EEBUF300[2] = {0x03,0x00};
unsigned char RG_EEBUF310[2] = {0x03,0x10};
unsigned char RG_EEBUF320[2] = {0x03,0x20};
unsigned char RG_EEBUF330[2] = {0x03,0x30};
unsigned char RG_EEBUF400[2] = {0x04,0x00};
unsigned char RG_EEBUF410[2] = {0x04,0x10};
unsigned char RG_EEBUF420[2] = {0x04,0x20};
unsigned char RG_EEBUF430[2] = {0x04,0x30};
unsigned char RG_EEBUF500[2] = {0x05,0x00};
unsigned char RG_EEBUF510[2] = {0x05,0x10};
//unsigned char RG_EEBUF510[2] = {0x05,0x10};
unsigned char	RG_ACCESS[2] = { 0x06,01};
unsigned char RG_SLEEP_TIMER_MSB[2] = {0x06,0x50};
unsigned char RG_SLEEP_TIMER_LSB[2] = {0x06,0x51};
unsigned char RG_KL_CTRL[2] ={0x06,0x22};
unsigned char RG_RSCREATE_CTRL[2] = {0x06,0x23};
unsigned char RG_MEM_TEST_OPMODE[2]   = { 0x06, 0xB5 };
unsigned char RG_ST1_MEM_TEST_OPMODE[2] = {0x06,0x0E};
unsigned char A_MIDR[2] = {0xED,0x00};
unsigned char A_EE_MEM_BKUP_RSFLAG[2] = {0xEF,0x00};
unsigned char A_EE_MEM_BKUP_CTRL[2] = {0xEF,0x01};
unsigned char RG_BIST_MODE[2] = {0x07,0x22};
unsigned char RG_EE_BI_NO[2] = {0x07,0x23};
unsigned char RG_MB_ERROR_BIT[2] = {0x07,0x24};
unsigned char RG_SRAM_BIST_RESULT[2] = {0x07,0x25};
unsigned char RG_SHAAUTH_CTRL[2] = {0x06,0x24};
unsigned char RG_SOFT_RESET[2] = {0x06,0x00};				
/////////////////////////////////////////////////DEBUG REGISTER/////////////////////////////////////////////////////
unsigned char RG_ST0_CUR[2]                         ={0x0C,0x00};
unsigned char RG_CHK_RSFLAG[2]                      ={0x0C,0x01};
unsigned char RG_ST1_CHK_RSFLAG_CUR[2]              ={0x0C,0x02};
unsigned char RG_ST1_PON_READ_CUR[2]                ={0x0C,0x03};
unsigned char RG_STCM0_CUR[2]                       ={0x0C,0x04};
unsigned char RG_ST1_STDSPI_CUR[2]                  ={0x0C,0x05};
unsigned char RG_ST1_EE_CFG_CUR[2]                  ={0x0C,0x06};
unsigned char RG_ST1_RND_CUR[2]                     ={0x0C,0x07};
unsigned char RG_ST1_SYMCIP_CUR[2]                  ={0x0C,0x08};
unsigned char RG_ST1_OKA_CUR[2]                     ={0x0C,0x09};
unsigned char RG_ST1_MIDR_CUR[2]                    ={0x0C,0x0A};
unsigned char RG_ST1_PERM_GET_CUR[2]                ={0x0C,0x0B};
unsigned char RG_ST1_EEP_OW_CTRL_CUR[2]             ={0x0C,0x0C};
unsigned char RG_ST1_MEM_TEST_CUR[2]                ={0x0C,0x0D};
unsigned char RG_ST2_EEP_OW_CTRL_CUR[2]             ={0x0C,0x0E};
unsigned char RG_ST2_SYMCIP_OPMODE_AES_CUR[2]       ={0x0C,0x0F};
unsigned char RG_ST2_OKA_OKA2_CUR[2]                ={0x0C,0x10};
unsigned char RG_ST2_STDSPI_SHA_CUR[2]              ={0x0C,0x11};
unsigned char RG_ST2_SYMCIP_SHAAuth_CUR[2]          ={0x0C,0x12};
unsigned char RG_ST2_RND_CUR[2]                     ={0x0C,0x13};
unsigned char RG_ST3_RND_CUR[2]                     ={0x0C,0x14};
unsigned char RG_ST3_SYMCIP_AES_CUR[2]              ={0x0C,0x15};
unsigned char RG_ST3_SYMCIP_KEYLOAD_CUR[2]          ={0x0C,0x16};
unsigned char RG_ST3_SYMCIP_RSCREATE_CUR[2]         ={0x0C,0x17};

//**************************************************************************
//* Register Address End
//**************************************************************************

//**************************************************************************
//* RG_EET_OPMODE Register Value
//**************************************************************************
unsigned int STEM_WRITE_MAIN_AUTO = 0x0D;
unsigned int STEM_READ_MAIN = 0x02;
//**************************************************************************
//* RG_EET_OPMODE Register Value End
//**************************************************************************

//**************************************************************************
//* RG_MEM_TEST_OPMODE Register Value
//**************************************************************************
unsigned int ST1_MEM_TEST_RG_EEBUF_WR = 0x02;
unsigned int ST1_MEM_TEST_EE_WRRD_EN = 0x03 ;
unsigned int ST1_MEM_TEST_EE_WR_EN = 0x03;
unsigned int ST1_MEM_TEST_STANDBY = 0x01;
unsigned int ST1_MEM_TEST_EE_BIST =0x05;
unsigned char A_EEPROM[2] = {0x00,0x00};
unsigned char A_EEPROM_FFC0[2] = {0xff,0xc0};
//**************************************************************************
//* RG_MEM_TEST_OPMODE Register Value End
//**************************************************************************

