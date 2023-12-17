#include "enc28j60.h"

//--------------------------------------------------------------------------
static SPI_HandleTypeDef *hspi;
//--------------------------------------------------------------------------
uint8_t gateway_ip[4];
uint8_t gateway_mac[6];
const uint8_t macaddr [6] = {0x00,0x00,0x20,0x07,0x19,0x98};
uint8_t ip [4] = {192,168,1,198};
char debug_string[60];
//--------------------------------------------------------------------------
void SPI_SendByte(uint8_t _byte)
{
	HAL_SPI_Transmit(hspi,(uint8_t *)&_byte,1,10);
}
//--------------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
 uint8_t dump = 0xFF;
 uint8_t _byte;
 HAL_SPI_TransmitReceive(hspi,(uint8_t *)&dump,(uint8_t *)&_byte,1,10);
 return _byte;
}
//--------------------------------------------------
void ENC28J60_write_command(uint8_t op,uint8_t addres, uint8_t data)
{
 SS_SELECT();
 SPI_SendByte(op|(addres&0x1F));
 SPI_SendByte(data);
 SS_DESELECT();
}
uint8_t ENC28J60_read_command(uint8_t op,uint8_t addres)
{
 uint8_t result;
 SS_SELECT();
 SPI_SendByte(op|(addres&0x1F));
 if(addres & 0x80) SPI_ReceiveByte();
 result=SPI_ReceiveByte();
 SS_DESELECT();
 return result;
}
uint8_t Enc28j60Bank;
void ENC28J60_SetBank(uint8_t addres)
{
 if ((addres&0x60)!=Enc28j60Bank) //neu bank hien tai khac bank da cai dat
 {
  ENC28J60_write_command(ENC28J60_BIT_FIELD_CLR,ECON1,ECON1_BSEL1|ECON1_BSEL0);
  Enc28j60Bank = addres&0x60;    //luu lai de lan sau kiem tra
  ENC28J60_write_command(ENC28J60_BIT_FIELD_SET,ECON1,Enc28j60Bank>>5);
 }
}
void ENC28J60_writeByte(uint8_t addres,uint8_t data)
{
 ENC28J60_SetBank(addres);
 ENC28J60_write_command(ENC28J60_WRITE_CTRL_REG,addres,data);
}
//--------------------------------------------------
uint8_t ENC28J60_readByte(uint8_t addres)
{
 ENC28J60_SetBank(addres);
 return ENC28J60_read_command(ENC28J60_READ_CTRL_REG,addres);
}
void ENC28J60_writeByte16(uint8_t addres,uint16_t data)
{
 ENC28J60_writeByte(addres, data);
 ENC28J60_writeByte(addres+1, data>>8);
}
void ENC28J60_writePhy(uint8_t addres,uint16_t data) //ham ghi PHY
{
  ENC28J60_writeByte(MIREGADR, addres);
  ENC28J60_writeByte16(MIWR, data);
  while(ENC28J60_readByte(MISTAT)&MISTAT_BUSY); //cho het ban
}
uint16_t ENC28J60_readPhy(uint8_t addres) //ham doc PHY
{
  uint16_t value;
  ENC28J60_writeByte(MIREGADR, addres);
  ENC28J60_writeByte(MICMD, MICMD_MIIRD);       //set bit MICMD_MIIRD bat dau doc
  while(ENC28J60_readByte(MISTAT)&MISTAT_BUSY); //cho het ban
  ENC28J60_writeByte(MICMD,0x00);               //clear bit MICMD_MIIRD
  value = ENC28J60_readByte(MIRDL) + ((uint16_t)ENC28J60_readByte(MIRDH)<<8) ;  //doc gia tri trong 2 thanh gho MIWR
  return value; //tra ve gia tri
}
static uint16_t NextPacketPtr;
void ENC29J600_ini(SPI_HandleTypeDef *spi)
{
 hspi = spi;
 uint8_t mymac[6];
 UART_putString("Dang khoi tao ENC28J60 ...\r\n");
 ENC28J60_write_command(ENC28J60_SOFT_RESET,0x1F,0); //soft reset
 HAL_Delay(2);
 while(!ENC28J60_read_command(ENC28J60_READ_CTRL_REG,ESTAT)&ESTAT_CLKRDY); //cho bit CLKRDY duoc set
 if(ENC28J60_readByte(ERDPT) != 0xFA)ENC28J60_error(); //khoi tao that bai
 else
 {
    NextPacketPtr=RXSTART_INIT;
    //cau hinh kich thuoc bo dem truyen nhan
    ENC28J60_writeByte16(ERXST,RXSTART_INIT);
    ENC28J60_writeByte16(ERXND,RXSTOP_INIT);

    ENC28J60_writeByte16(ETXST,TXSTART_INIT);
    ENC28J60_writeByte16(ETXND,TXSTOP_INIT);

    //reset con tro RX
    ENC28J60_writeByte16(ERXRDPT,RXSTART_INIT);
    ENC28J60_writeByte16(ERXWRPT,RXSTART_INIT);

    //rx buffer filters
    //ENC28J60_writeByte(ERXFCON,ERXFCON_UCEN|ERXFCON_ANDOR|ERXFCON_CRCEN); //set 3 bit

    //cau hinh MAC
    ENC28J60_writeByte(MACON1,MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
    ENC28J60_writeByte(MACON2,0x00);
    ENC28J60_write_command(ENC28J60_BIT_FIELD_SET,MACON3,MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
    ENC28J60_writeByte16(MAIPG,0x0C12);
    ENC28J60_writeByte(MABBIPG,0x12);

    //Set maximum frame length
    ENC28J60_writeByte16(MAMXFL,1500);

    //Set Mac addr
    ENC28J60_writeByte(MAADR5,macaddr[0]);
    ENC28J60_writeByte(MAADR4,macaddr[1]);
    ENC28J60_writeByte(MAADR3,macaddr[2]);
    ENC28J60_writeByte(MAADR2,macaddr[3]);
    ENC28J60_writeByte(MAADR1,macaddr[4]);
    ENC28J60_writeByte(MAADR0,macaddr[5]);

    //read MAC
    mymac[0]=ENC28J60_readByte(MAADR5);
    mymac[1]=ENC28J60_readByte(MAADR4);
    mymac[2]=ENC28J60_readByte(MAADR3);
    mymac[3]=ENC28J60_readByte(MAADR2);
    mymac[4]=ENC28J60_readByte(MAADR1);
    mymac[5]=ENC28J60_readByte(MAADR0);

    sprintf(debug_string,"\r\nYour MAC addr = %02X:%02X:%02X:%02X:%02X:%02X\r\n",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
    UART_putString(debug_string);

    //**********Advanced Initialisations************//
    ENC28J60_writePhy(PHCON2,PHCON2_HDLDIS);
    ENC28J60_writePhy(PHLCON,PHLCON_LACFG2|PHLCON_LBCFG2|PHLCON_LBCFG1|PHLCON_LBCFG0|PHLCON_LFRQ0|PHLCON_STRCH);
    ENC28J60_SetBank (ECON1);
    ENC28J60_write_command (ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE);
    ENC28J60_writeByte(ECOCON,0x02);
    HAL_Delay(1);
    ENC28J60_write_command (ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN); // cho phép nhan gói
 }
}
void ENC28J60_error (void)
{
  UART_putString("Khoi tao module ENC28J60 that bai !\r\n");
}

//------------------------------------lam viec voi cac goi tin -------------------------------------//
uint8_t ENC28J60_read_Byte_Buffer(void)
{
   return ENC28J60_read_command(ENC28J60_READ_BUF_MEM,0);
}
void ENC28J60_ReadBuffer(uint16_t len, uint8_t* data)
{
  SS_SELECT();
  SPI_SendByte(ENC28J60_READ_BUF_MEM); //gui Opcode
  while(len)
  {
        len--;
        // read data
        *data = (uint8_t)SPI_ReceiveByte();
        data++;
  }
 *data='\0'; //set end buffer
 SS_DESELECT();
}
uint16_t ENC28J60_read_packet(uint8_t *buf,uint16_t buflen)
{
  uint16_t status;
  uint16_t len=0;
  if(ENC28J60_readByte(EPKTCNT)>0)  // neu co goi tin
  {
      //UART_putString("Co goi tin moi\r\n");

      //Thiet lap con tro de doc du lieu tu goi tin nhan duoc
      ENC28J60_writeByte16(ERDPT,NextPacketPtr);

      //Doc gia tri con tro cua goi tin tiep theo
      NextPacketPtr=ENC28J60_read_Byte_Buffer();
      NextPacketPtr|=(uint16_t)ENC28J60_read_Byte_Buffer()<<8;

      // Doc kich thuoc cua goi tin
      len  = ENC28J60_read_Byte_Buffer();
	  len |= ENC28J60_read_Byte_Buffer()<<8;

      len-=4; // xoa 4byte checksum o cuoi
      if(len>buflen) len=buflen;

      //Doc trang thai cua bo nhan
      status  = ENC28J60_read_Byte_Buffer();
	  status |= ENC28J60_read_Byte_Buffer()<<8;

      if ((status & 0x80)==0)len=0; //kiem tra bit Received Ok, neu err thi khong doc goi nay
      else
      {
        //doc du lieu trong bo dem
        ENC28J60_ReadBuffer(len, buf);
      }

      // Chuyen con tro du lieu nhan toi phan dau cua goi tin tiep theo.
      if(NextPacketPtr-1>RXSTOP_INIT)ENC28J60_writeByte16(ERXRDPT,RXSTOP_INIT);
      else ENC28J60_writeByte16(ERXRDPT,NextPacketPtr);
      ENC28J60_write_command(ENC28J60_BIT_FIELD_SET,ECON2,ECON2_PKTDEC);
  }
  return len;
}
void ENC28J60_writeBuf(uint16_t len,uint8_t* data)
{
  SS_SELECT();
  SPI_SendByte(ENC28J60_WRITE_BUF_MEM);
  while(len--)
  SPI_SendByte(*data++);
  SS_DESELECT();
}
void ENC28J60_send_packet(uint8_t *buf,uint16_t buflen)
{
   //cho qua trinh truyen truoc do hoan tat
   while(ENC28J60_readByte(ECON1)&ECON1_TXRTS);

   // Thiet lap con tro bat dau khong gian bo dem truyen
   ENC28J60_writeByte16(EWRPT,TXSTART_INIT);

   //Thiet lap con tro toi vi tri cua byte cuoi cua goi tin
   ENC28J60_writeByte16(ETXND,TXSTART_INIT+buflen);

   //truyen byte dau tien la 0x00 vao bo dem
   ENC28J60_write_command(ENC28J60_WRITE_BUF_MEM,0,0);

   //truyen goi tin vao bo dem
   ENC28J60_writeBuf(buflen,buf);

   //set bit TXRTS cho enc28j60 truyen goi tin
   ENC28J60_write_command(ENC28J60_BIT_FIELD_SET,ECON1,ECON1_TXRTS);
}
//--------------------------------------------------------------------------
//End file

