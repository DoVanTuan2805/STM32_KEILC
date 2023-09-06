#include "stm32f10x.h"

typedef struct
{
		uint32_t CCR;
		uint32_t CNDTR;
		uint32_t CPAR;
		uint32_t CMAR;
}DMA_ChanelTypdef;

typedef struct
{
		uint32_t DMA_ISR;
		uint32_t DMA_IFCR;
}DMA_Typdef;

#define DMA1_ADDRESS		0x40020000

#define DMA1_Base 		((DMA_Typdef *) DMA1_ADDRESS)

#define DMA1_Chanel_1 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x0008 ))
#define DMA1_Chanel_2 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x001C ))
#define DMA1_Chanel_3 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x0030 ))
#define DMA1_Chanel_4 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x0044 ))
#define DMA1_Chanel_5 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x0058 ))
#define DMA1_Chanel_6 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x006C ))
#define DMA1_Chanel_7 ((DMA_ChanelTypdef *) ( DMA1_ADDRESS + 0x0080 ))
 
void DMA_MEM2MEM (uint32_t *data1, uint32_t *data2, uint32_t length);

#define DATA_TRANFER 19U
static char str1[DATA_TRANFER] = {"Lap Trinh Nhung A-Z"};
static char str2[DATA_TRANFER];

int main(void)
{
		DMA_MEM2MEM((uint32_t*)(uint32_t)str1, (uint32_t*)(uint32_t)str2, DATA_TRANFER);
		while (1)
		{
			
		}
}

void DMA_MEM2MEM (uint32_t *data1, uint32_t *data2, uint32_t length)
{
		/*	ENABLE CLOCK DMA1 */
		RCC->AHBENR |= (1 << 0);
	
		/*	
			Set the peripheral register address in the DMA_CPARx register	
			The data will be moved from/ to this address to/ from the memory after the peripheral event.
		*/
		DMA1_Chanel_2->CPAR	= (uint32_t) data2;
	
		/*	
			Set the memory address in the DMA_CMARx register 
			The data will be written to or read from this memory after the peripheral event
		*/
		DMA1_Chanel_2->CMAR	= (uint32_t) data1;
		
		/*
			DMA channel x number of data register 
			Configure the total number of data to be transferred in the DMA_CNDTRx register. 
			After each peripheral event, this value will be decremented
		*/
		DMA1_Chanel_2->CNDTR = (uint32_t) length;
		
		/*	MEM TO MEM ENABLE */
		DMA1_Chanel_2->CCR 	|= (1U << 14);
		
		/*	Channel priority level */
		DMA1_Chanel_2->CCR |= (0U << 13 ) | (1U << 12);
		
		/* CONFIG DATA TRANSFER DIRECTION : READ MEMORY */
		DMA1_Chanel_2->CCR |= (1U << 4);
		
		/* CONFIG CIRCULAR MODE : ENABLE	*/
		DMA1_Chanel_2->CCR |= (0U << 5);
		
		/* CONFIG MEMORY INCREMENT MODE : ENABLE	*/
		DMA1_Chanel_2->CCR |= (1U << 7);
		
		/* CONFIG PERIPHERAL INCREMENT MODE : ENABLE	*/
		DMA1_Chanel_2->CCR |= (1U << 6);
		
		/*	CONFIG MEMORY SIZE 32 BITS */
		DMA1_Chanel_2->CCR |= (1U << 11 ) | (0U << 10);
		
		/*	CONFIG PERIPHERAL SIZE 32 BITS */
		DMA1_Chanel_2->CCR |= (1U << 9 ) | (0U << 8);
		
		/*	CHANNEL ENABLE */
		DMA1_Chanel_2->CCR |= (1U << 0);
}

