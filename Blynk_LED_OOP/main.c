#include "stm32f10x.h"
#include "blynk_1s.h"
int main(void)
{
	BlynkMode_t b1,b2;
	GPIO_t const *gpioBlynk[] =
	{
		&b1.gpio,
		//&b2.gpio,
		//(GPIO_t*)(0)
	};
	Blynk_ctor(&b1, BLYNK1S, GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	Blynk_ctor(&b2, BLYNK1S, GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	

	
	while(1){
		EnableModeBlynk_2s(gpioBlynk, sizeof(gpioBlynk) / sizeof(gpioBlynk[0]));
	}
}
