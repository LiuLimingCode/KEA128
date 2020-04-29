#include "headfile.h"

uint8 Pit_flag = 0;

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
	Pit_flag = 1;
	
}

int main(void)
{
	int32 times1,times2,time,delay;
    get_clk();
	pit_init_ms(pit0, 100);
	
    OLED_Init();
	set_irq_priority(PIT_CH0_IRQn, 1);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	
    while(1)
	{
		if(Pit_flag == 1)
		{
			Pit_flag = 0;
			delay = 0;
			times1 = PIT->CHANNEL[pit0].CVAL;
			if(PIT->CHANNEL[pit0].TFLG &  PIT_TFLG_TIF_MASK)
				delay += 1;
			
			OLED_P6x8Int(0, 0, 121, 3);
			OLED_P6x8Int(0, 1, 121, 3);
			OLED_P6x8Int(0, 2, 121, 3);
			OLED_P6x8Int(0, 3, 121, 3);
			OLED_P6x8Int(0, 4, 121, 3);
			OLED_P6x8Int(0, 5, 122, 3);
			OLED_P6x8Int(0, 6, 123, 3);
			OLED_P6x8Int(64, 0, 121, 3);
			OLED_P6x8Int(64, 1, 121, 3);
			OLED_P6x8Int(64, 2, 121, 3);
			OLED_P6x8Int(64, 3, 121, 3);
			OLED_P6x8Int(64, 4, 121, 3);
			OLED_P6x8Int(64, 5, 122, 3);
			OLED_P6x8Int(64, 6, 123, 3);
			times2 = PIT->CHANNEL[pit0].CVAL;
			if(PIT->CHANNEL[pit0].TFLG &  PIT_TFLG_TIF_MASK)
				delay += 1;
			time = times1 - times2;
			OLED_P6x8Flo(0, 7, (float)time/(100 * bus_clk_khz));
			OLED_P6x8Int(64, 7, delay, 3);
		}
		
	}
        
	
}
