#include "dorca30_api.h"
#include "global.h"
extern int cs; 
unsigned char ShaBitSize(long long x,unsigned char *bitsize)
{
	int i;
	long long data_size;
	unsigned char hex_size[8];
	data_size = x*8;
	
	hex_size[0] = ((unsigned char) (data_size >> 56)) & 0xff;
	hex_size[1] = ((unsigned char) (data_size >> 48)) & 0xff;
	hex_size[2] = ((unsigned char) (data_size >> 40)) & 0xff;
	hex_size[3] = ((unsigned char) (data_size >> 32)) & 0xff;
	hex_size[4] = ((unsigned char) (data_size >> 24)) & 0xff;
	hex_size[5] = ((unsigned char) (data_size >> 16)) & 0xff;
	hex_size[6] = ((unsigned char) (data_size >>  8)) & 0xff;
	hex_size[7] = ((unsigned char) (data_size >>  0)) & 0xff;
	
	for( i=0; i<8; i++)
	{
		bitsize[i] = hex_size[i];
	}
	return 0;
}

long long ShaMultiFrameFrame(unsigned char *txdata, long long ByteNo)
{
	int success =  1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	long long i;
	long long j;
	long long temp_cnt = 0; 
	//multi_frame data array
	long long array_no=0, array_cnt=0; 
	unsigned int frame_cnt = 0;
	unsigned char frame_buffer[64];
	tx_data[0] = 2;
	tspi_interface(cs, ADDR_NOR_W,RG_SHA_CTRL				   , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x6;
	tspi_interface(cs, ADDR_NOR_W,RG_ST0_OPMODE					, 0, 0, 0, 0, tx_data, rx_data, 1); 
	tx_data[0] = 0x4;
	tspi_interface(cs, ADDR_NOR_W,RG_ST1_STDSPI_OPMODE 				  , 0, 0, 0, 0, tx_data, rx_data, 1);	

	frame_cnt = (ByteNo/64); // sha operation counter
	
	for( i=0; i<frame_cnt; i++)
	{
		array_cnt = (i+1) * 64;

		for( array_no=0; array_no<64; array_no++)
		{
            temp_cnt = (array_cnt-64) + array_no;
            frame_buffer[array_no] = txdata[temp_cnt];
		}
		#ifdef DEBUG_SHA
		printf("\r\n frame_buffer %d line %d  ShaMultiFrameFrame",gFrameNumber++, __LINE__);
		PrintByte(frame_buffer,64);
		#endif
		reversebuffer(tx_data, frame_buffer, 64);
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300 				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);
	}

	return temp_cnt+1;
}
unsigned char ShaSingleFrame(unsigned char *txdata, long long temp_cnt, long long ByteNo, unsigned char frame_type)
{
	int success =  1;
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	int i, j;
	long long last_cnt;
	long long mod55 = 0;
	unsigned char frame_buffer[64] = { 0x00, };
	unsigned char bitsize[8];
	
	last_cnt = ByteNo % 64;			
	ShaBitSize(ByteNo,bitsize);
	for( j=0; j<64; j++)
	{
	   frame_buffer[j] = 0x00;
	}

	//last frame input user write last txdata_Bytes
	for( j=0; j<last_cnt; j++)
	{
	  
	  	 frame_buffer[j] = txdata[j]; 
	}
	//txdata last Byte index : write data = 0x80;

	frame_buffer[last_cnt] = 0x80;
				
	if ( last_cnt <= 55 )
	{
	
	tx_data[0] =0;
	tspi_interface(cs, ADDR_NOR_W,RG_SHA_CTRL				   , 0, 0, 0, 0, tx_data, rx_data, 1);
	tx_data[0] = 0x6;
	tspi_interface(cs, ADDR_NOR_W,RG_ST0_OPMODE 				, 0, 0, 0, 0, tx_data, rx_data, 1); 
	tx_data[0] = 0x4;
	tspi_interface(cs, ADDR_NOR_W,RG_ST1_STDSPI_OPMODE				  , 0, 0, 0, 0, tx_data, rx_data, 1);	
	    for( j=(last_cnt+1); j<64; j++)
	    {
		    frame_buffer[j] = 0x00;
	    }

	    for( j=0; j<8; j++) frame_buffer[j+56] = bitsize[j];

		reversebuffer(tx_data, frame_buffer, 64);
		#ifdef DEBUG_SHA
		printf("\r\n frame_buffer %d line %d ShaSingleFrame <55",gFrameNumber++, __LINE__);
		PrintByte(frame_buffer,64);	
		#endif
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300				  , 0, 0, 0, 0, tx_data, rx_data, 64);
    		
	}

	if ( last_cnt >= 56 )
	{
		
		reversebuffer(tx_data, frame_buffer, 64);
		#ifdef DEBUG_SHA
		printf("\r\n frame_buffer %d line %d",gFrameNumber++, __LINE__);
		PrintByte(frame_buffer,64);
		#endif
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);		
		
	    for( j=0; j<64; j++)
	    {
	        frame_buffer[j] = 0x00;
	    }

	    for( j=0; j<8; j++) frame_buffer[j+56] = bitsize[j];
		tx_data[0] = 3;
		tspi_interface(cs, ADDR_NOR_W,RG_SHA_CTRL				   , 0, 0, 0, 0, tx_data, rx_data, 1);

		#ifdef DEBUG_SHA
		printf("\r\n frame_buffer %d line %d",gFrameNumber++, __LINE__);

		printf("\r\n frame_buffer %d",gFrameNumber++);
		#endif
		PrintByte(frame_buffer,64);		
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300 				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);		
	}



	return 0;
}


unsigned char LastMultiFrame(unsigned char *txdata, long long temp_cnt, long long ByteNo, unsigned char frame_type)
{
	int i, j;
	long long last_cnt;
	long long mod55 = 0;
	unsigned char frame_buffer[64] = { 0x00, };
	unsigned char bitsize[8];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	
	last_cnt = ByteNo % 64;			
	ShaBitSize(ByteNo,bitsize);

	#ifdef DEBUG_SHA
	printf("\r\n last_cnt %d",last_cnt);
	printf("\r\n temp_cnt %d",temp_cnt);
	#endif

	//last frame input user write last txdata_Bytes
	for( j=0; j<last_cnt; j++)
	{
	  	frame_buffer[j] = txdata[(temp_cnt+j)]; // only single frame operation

	}
	//txdata last Byte index : write data = 0x80;
	frame_buffer[last_cnt] = 0x80;
				



	

	if ( last_cnt <= 55 )
	{
	    for( j=(last_cnt+1); j<64; j++)
	    {
		    frame_buffer[j] = 0x00;
	    }

	    for( j=0; j<8; j++) frame_buffer[j+56] = bitsize[j];


		tx_data[0] = 3;
		tspi_interface(cs, ADDR_NOR_W,RG_SHA_CTRL				   , 0, 0, 0, 0, tx_data, rx_data, 1);

		reversebuffer(tx_data, frame_buffer, 64);
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300 				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);		
		#ifdef DEBUG_SHA		
		printf("\r\n frame_buffer %d line %d",gFrameNumber++, __LINE__);
		PrintByte(frame_buffer,64);	
		#endif
	}


	if ( last_cnt >= 56 )
	{
		
		reversebuffer(tx_data, frame_buffer, 64);
		#ifdef DEBUG_SHA		
		printf("\r\n frame_buffer %d line %d",gFrameNumber++, __LINE__);

		PrintByte(frame_buffer,64);
		#endif
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);		
		
	    for( j=0; j<64; j++)
	    {
	        frame_buffer[j] = 0x00;
	    }

	    for( j=0; j<8; j++) frame_buffer[j+56] = bitsize[j];
		tx_data[0] = 3;
		tspi_interface(cs, ADDR_NOR_W,RG_SHA_CTRL				   , 0, 0, 0, 0, tx_data, rx_data, 1);

		reversebuffer(tx_data, frame_buffer, 64);
		#ifdef DEBUG_SHA
		printf("\r\n frame_buffer %d line %d",gFrameNumber++, __LINE__);

		PrintByte(frame_buffer,64);		
		#endif
		tspi_interface(cs, ADDR_NOR_W,RG_EEBUF300 				  , 0, 0, 0, 0, tx_data, rx_data, 64);	
		Delay_us(10);		
	}

	return 0;
}


void StandardShaMode(unsigned char *txdata, unsigned char *rxdata, long long ByteNo)
{
	long long i;
	long long temp_cnt;
	unsigned char frame_type;
	unsigned char tx_data[64];
	unsigned char rx_data[64];

	
	//gene_WakeUp();

	if(ByteNo > 55)
	{
		frame_type = 1; // multi-frame set
	}
	else
	{
		frame_type = 0; // single-frame set
	}
	#ifdef DEBUG_SHA
	printf("\r\n ByteNo %d",ByteNo);
	#endif
	if( !frame_type )
	{
		#ifdef DEBUG_SHA
		printf("\r\n SingleFrame");
		#endif
		temp_cnt = 0;		
		ShaSingleFrame(txdata,temp_cnt,ByteNo,frame_type);		
		
	}
	else
	{
		#ifdef DEBUG_SHA
		printf("\r\n MultiFrame");
		#endif
		temp_cnt = ShaMultiFrameFrame(txdata, ByteNo);
		
		//multi frame last single frame make	
		if(temp_cnt == 1)
			temp_cnt = 0;
		LastMultiFrame(txdata,temp_cnt,ByteNo,frame_type);
	}
	
    tspi_interface(cs, ADDR_NOR_R,RG_EEBUF400 				  , 0, 0, 0, 0, tx_data, rx_data, 32);	
    reversebuffer(rxdata,rx_data,32);
    
	tx_data[0] = 1;
	tspi_interface(cs, ADDR_NOR_W,RG_ST1_STDSPI_OPMODE 				  , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 1;
	tspi_interface(cs, ADDR_NOR_W,RG_ST0_OPMODE 				  , 0, 0, 0, 0, tx_data, rx_data, 1);	
	tx_data[0] = 0;
	tspi_interface(cs, ADDR_NOR_W,RG_ACCESS 				  , 0, 0, 0, 0, tx_data, rx_data, 1);	
	EndOperation();
	return ;   
}