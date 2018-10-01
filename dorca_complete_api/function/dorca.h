#ifndef __DORCA_30_H__
#define __DORCA_30_H__ 
void SpiRead (int fd,int inst, int addr, int nbytes, unsigned char *rx_data);
void SpiWrite(int fd,int inst, int addr, int nbytes, char *value);
void Dorca3Cm0SpiInit( int com_speed );
void Dorca3Close();
void Dorca3Cm0Close();
void Dorca3SpiInit( int com_speed );
void SendData(unsigned char *buffer,int size);
void ReadData(unsigned char *tx_buffer,unsigned char *rx_buffer, int size);
#endif //__DORCA_20_H__
