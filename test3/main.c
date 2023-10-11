/*
 * test3.c
 *
 * Created: 20.09.2023 22:15:18
 * Author : stnikan
 */ 
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <avr/interrupt.h>   //для использования прерываний

int16_t v_max = 6000;
int16_t v = 1;
int16_t dv = 0;
uint8_t k = 12000; // коэф для перевода скорости двигателя во время переключения обмоток
uint8_t i = 0;
uint16_t my_time = 0;
uint8_t sec = 0;
uint16_t ms = 0;

void timer_ini(void)
{
	//нам больше не нужен делей, используем для изменения работающей обмотки
	ETIMSK = (1<<TOIE3);
	TCCR3B = (1<<CS32); //используем делитель частоты
	TCNT3 = 65492; //срабатывает каждые 23 мкс
	
}


//———————————————
ISR (TIMER3_OVF_vect) //функция по таймеру 3, крутит двигатель с определённой задержкой
{
	TCNT3 = 65492; //прошла мс
	ms++;
	my_time ++;
	if (my_time  > k/v){
		my_time = 0;
		if (v>0){i=(i+1)%4;}
		else {i = (i+3)%4;}
	}
	
	if (ms > 1000){
		sec= (sec+1)%12;
		ms = ms%1000;
	}
	
	
	if (sec>6 && sec < 10){dv = 0;}
	else if (sec>2 && sec<6){dv = -1;}
	else {dv = 1;}
	v += dv*10;
	
	
	
	
	
}

uint8_t my_phase[] = {0b00000011, 0b00000110, 0b00001100, 0b00001001} ;

int main(void)
{
    /* Replace with your application code */
	DDRC = 0x0F;
    while (1) 
    {	
		PORTC = my_phase[i];
    }
}





//если эта хрень заработает я буду в шоке
