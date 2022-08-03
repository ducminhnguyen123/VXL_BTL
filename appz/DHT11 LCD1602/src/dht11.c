#include "dht11.h"
#include "delay.h"

void dht11_init(void)
{
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_OD;
	gpioInit.GPIO_Pin = GPIO_Pin_8;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpioInit);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

uint8_t dht11_read(uint8_t *pu8Data)
{
	uint16_t u16Tim;// bien kiemtra dua chan 12 len muc cao hay time out va nhay ra khoi vong lap
	uint8_t u8Buff[5];
	uint8_t u8CheckSum;
	uint8_t i;
	uint8_t j;
	//xem lai cho nay
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	Delay_Ms(20);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	
	/* cho chan PB8 len cao */
	/*TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 20) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(TIM2);
	if (u16Tim >= 20) {
		return 0;
	}*/
	
	/* cho chan PB8 xuong thap */
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 40) {
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(TIM2);
	if ((u16Tim >= 40) || (u16Tim <=20)) {
		return 0;
	}
	
	/* cho chan PB8 len cao */
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 90) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(TIM2);
	if ((u16Tim >= 90) || (u16Tim <= 70)) {
		return 0;
	}
	
	/* cho chan PB8 xuong thap */
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 95) {
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(TIM2);
	if ((u16Tim >= 95) || (u16Tim <= 75)) {
		return 0;
	}
	
	/* nhan 5 byte  */
  for (j=0; j < 4;++j) {
	 for (i = 0; i < 8; ++i) {
		/* cho chan PB8 len cao */
		TIM_SetCounter(TIM2, 0);
		while (TIM_GetCounter(TIM2) < 65) {
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
				break;
			}
		}
		
		/* cho chan PB8 xuong thap */
		TIM_SetCounter(TIM2, 0);
		while (TIM_GetCounter(TIM2) < 80) {
			if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)) {
				break;
			}
		}
	
		u16Tim = TIM_GetCounter(TIM2);
		
		u8Buff[j] <<= 1;
		if (u16Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[j] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[j] &= ~1;
		}
	}
}
	
	for (i = 0; i < 4; ++i) {
		pu8Data[i] = u8Buff[i];
	}
	return 1;
}
