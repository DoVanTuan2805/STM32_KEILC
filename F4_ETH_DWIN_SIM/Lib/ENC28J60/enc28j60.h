#ifndef ENC28J60_H_
#define ENC28J60_H_
//--------------------------------------------------
//Include cac thu vien can thiet
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "main.h"
//-------------------------------------------------
#define CS_GPIO_PORT  GPIOB
#define CS_PIN        GPIO_PIN_0
#define SS_SELECT()   HAL_GPIO_WritePin(CS_GPIO_PORT,CS_PIN,GPIO_PIN_RESET)
#define SS_DESELECT() HAL_GPIO_WritePin(CS_GPIO_PORT,CS_PIN,GPIO_PIN_SET)

//--------------------------------------------------
//dinh nghia cac Opcode
#define ENC28J60_BIT_FIELD_SET       0x80
#define ENC28J60_BIT_FIELD_CLR       0xA0
#define ENC28J60_READ_CTRL_REG       0x00
#define ENC28J60_WRITE_CTRL_REG      0x40
#define ENC28J60_SOFT_RESET          0xFF
#define ENC28J60_READ_BUF_MEM        0x3A
#define ENC28J60_WRITE_BUF_MEM       0x7A
//--------------------------------------------------
#define RXSTART_INIT        0x0000  // start of RX buffer, room for 2 packets
#define RXSTOP_INIT         0x0BFF  // end of RX buffer
//--------------------------------------------------
#define TXSTART_INIT        0x0C00  // start of TX buffer, room for 1 packet
#define TXSTOP_INIT         0x11FF  // end of TX buffer
//--------------------------------------------------
// ENC28J60 ECON1 Register Bit Definitions
#define ECON1            0x1F
#define ECON1_TXRST      0x80
#define ECON1_RXRST      0x40
#define ECON1_DMAST      0x20
#define ECON1_CSUMEN     0x10
#define ECON1_TXRTS      0x08
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01

// ENC28J60 ECON2 Register Bit Definitions
#define ECON2            0x1E
#define ECON2_PKTDEC     0x40

// ENC28J60 ESTAT Register Bit Definitions
#define ESTAT            0x1D
#define ESTAT_INT        0x80
#define ESTAT_LATECOL    0x10
#define ESTAT_RXBUSY     0x04
#define ESTAT_TXABRT     0x02
#define ESTAT_CLKRDY     0x01

// ENC28J60 ERXFCON Register Bit Definitions
#define ERXFCON          (0x18|0x20) //boi vi no thuoc bank1 nen can or them 0x20
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01

// Bank 0 registers
#define ERDPT           (0x00|0x00)
#define EWRPT           (0x02|0x00)
#define ETXST           (0x04|0x00)
#define ETXND           (0x06|0x00)
#define ERXST           (0x08|0x00)
#define ERXND           (0x0A|0x00)
#define ERXRDPT         (0x0C|0x00)
#define ERXWRPT         (0x0E|0x00)

// Bank 1 registers
#define EPKTCNT         (0x19|0x20)

// Bank 2 registers
#define MACON1          (0x00|0x40|0x80) //boi vi la bank 2 nen OR voi 0x40
#define MACON2          (0x01|0x40|0x80)
#define MACON3          (0x02|0x40|0x80)
#define MACON4          (0x03|0x40|0x80)
#define MABBIPG         (0x04|0x40|0x80)
#define MAIPG           (0x06|0x40|0x80)
#define MAMXFL          (0x0A|0x40|0x80)
#define MIREGADR        (0x14|0x40|0x80)
#define MIWR            (0x16|0x40|0x80)
// -- cau hinh vi tri cac bit trong MACON1---//
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01
// -- cau hinh vi tri cac bit trong MACON3---//
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMLEN   0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01

// Bank 3 registers
#define MAADR1           (0x00|0x60|0x80) //boi vi la bank 3 nen OR voi 0x60
#define MAADR0           (0x01|0x60|0x80)
#define MAADR3           (0x02|0x60|0x80)
#define MAADR2           (0x03|0x60|0x80)
#define MAADR5           (0x04|0x60|0x80)
#define MAADR4           (0x05|0x60|0x80)
#define MISTAT           (0x0A|0x60|0x80)

//--------------------------------------------------
//MII read registers and bit
#define MICMD		     (0x12|0x40|0x80)  //thanh ghi nay thuoc bank 2 nen OR 0x40
#define MICMD_MIIRD       0x01
#define MIRDL            (0x18|0x40|0x80)
#define MIRDH            (0x19|0x40|0x80)
#define MISTAT_BUSY       0x01

#define EIE              0x1B
// ENC28J60 EIE Register Bit Definitions
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40
#define EIE_DMAIE        0x20
#define EIE_LINKIE       0x10
#define EIE_TXIE         0x08
#define EIE_WOLIE        0x04
#define EIE_TXERIE       0x02
#define EIE_RXERIE       0x01

#define ECOCON           (0x15|0x60)     //thanh ghi nay thuoc bank 3 nen OR 0x60

// PHY registers
#define PHCON1           0x00
#define PHSTAT1          0x01
#define PHHID1           0x02
#define PHHID2           0x03
#define PHCON2           0x10
#define PHSTAT2          0x11
#define PHIE             0x12
#define PHIR             0x13
#define PHLCON           0x14
//--------------------------------------------------
#define PHCON2_HDLDIS    0x0100
//--------------------------------------------------
// PHLCON
#define PHLCON_LACFG3  0x0800
#define PHLCON_LACFG2  0x0400
#define PHLCON_LACFG1  0x0200
#define PHLCON_LACFG0  0x0100
#define PHLCON_LBCFG3  0x0080
#define PHLCON_LBCFG2  0x0040
#define PHLCON_LBCFG1  0x0020
#define PHLCON_LBCFG0  0x0010
#define PHLCON_LFRQ1   0x0008
#define PHLCON_LFRQ0   0x0004
#define PHLCON_STRCH   0x0002

// ENC28J60 EIR Register Bit Definitions
#define EIR              0x1C
#define EIR_PKTIF        0x40
#define EIR_DMAIF        0x20
#define EIR_LINKIF       0x10
#define EIR_TXIF         0x08
#define EIR_WOLIF        0x04
#define EIR_TXERIF       0x02
#define EIR_RXERIF       0x01

#define   MAX_FRAMELEN    1518   // (note: maximum ethernet frame length would be 1518)

//khai bao nguyen mau ham
void SPI_SendByte(uint8_t _byte);
uint8_t SPI_ReceiveByte(void);
uint8_t SPI_ReceiveByte(void);
void ENC28J60_write_command(uint8_t op,uint8_t addres, uint8_t data);
uint8_t ENC28J60_read_command(uint8_t op,uint8_t addres);
void ENC28J60_SetBank(uint8_t addres);
void ENC28J60_writeByte(uint8_t addres,uint8_t data);
uint8_t ENC28J60_readByte(uint8_t addres);
void ENC28J60_writeByte16(uint8_t addres,uint16_t data);
void ENC28J60_writePhy(uint8_t addres,uint16_t data);
uint16_t ENC28J60_readPhy(uint8_t addres);
bool ENC29J600_init(SPI_HandleTypeDef *spi);
void ENC28J60_error (void);

uint8_t ENC28J60_read_Byte_Buffer(void);
void ENC28J60_ReadBuffer(uint16_t len, uint8_t* data);
uint16_t ENC28J60_read_packet(uint8_t *buf,uint16_t buflen);
void ENC28J60_writeBuf(uint16_t len,uint8_t* data);
void ENC28J60_send_packet(uint8_t *buf,uint16_t buflen);
//--------------------------------------------------
#endif /* ENC28J60_H_ */
