/*
 * NEOPIXEL.cpp
 *
 * Created: 25/03/2024 01:36:09 a. m.
 * Author : UGB
 */ 

#define  F_CPU 13485000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//

#define ROJO 0xF00
#define VERDE 0xF0000
#define AZUL 0xF


const uint8_t NUMBERS1[] PROGMEM = {
	1,2,3,4,5,6,7,8,9,16,17,24,25,32,33,40,41,48,49,56,57,58,59,60,61,62,63,64
};

const uint8_t NUMBERS2[] PROGMEM = {
	10,11,12,13,14,15,18,23,26,31,34,39,42,47,50,51,52,53,54,55
};

const uint8_t NUMBERS3[] PROGMEM = {
	19,20,21,22,27,30,35,38,43,44,45,46
};

const uint8_t NUMBERS4[] PROGMEM = {
	28,	29,	36,	37	
};

// definicion de 0H, 1H, 0L, 1L

#define Tim_0H while(TCNT0 <= 1)//(_delay_us(0.4))  //0.4 US
#define Tim_1H while(TCNT0 <= 9)//(_delay_us(0.8))  //0.8 US
#define Tim_0L while(TCNT0 <= 5)//(_delay_us(0.85))  //0.85 US 266.66 uamp
#define Tim_1L while(TCNT0 <= 5)//(_delay_us(0.45))  //0.45 US
#define RES    while(TCNT0 <= 720)//(_delay_us(50))  // 50 US
#define reset  (TCNT0 =0)

// un NOP en ASM  equivale 83ns  y una asignacion en portb a 1 log igual 83ns y 42ns
// para un 0 logico es igual a 292 ns

// 167 ns y 125 con operadores a 1 log
// 375 ns con 0 log

// VALOR HEXADECIMAL 0xRRGGBB

//void enviar(uint8_t R, uint8_t G, uint8_t B, uint8_t N_Leds);
void enviar_all_bits(uint32_t dat, uint8_t N_Leds);
void enviar_color_individual(uint32_t dat);
void color_posicion(uint8_t* POSICION, uint32_t color);
void color_serie(uint32_t color);

int main(void)
{
	CCP = 0xD8;
	CLKPSR = 0;
	OSCCAL = 0xFF; //Aprox 12 Mhz 

	DDRB = 0x01;

	while (1){
		
		for (uint8_t i=0; i<4; i++){
			color_serie(VERDE);
			color_serie(ROJO);
			color_serie(AZUL);
		}
		
		for (uint8_t i=0; i<4; i++){
			color_posicion((uint8_t*)NUMBERS1, ROJO);
			color_posicion((uint8_t*)NUMBERS2, VERDE);
			color_posicion((uint8_t*)NUMBERS3,AZUL);
			color_posicion((uint8_t*)NUMBERS4,ROJO);
		}
	}
}

void enviar(uint8_t R, uint8_t G, uint8_t B, uint8_t N_Leds){
	uint32_t aux;

	
	// se envia G R B cuyo bit se envia el de mayor peso primero
	for (uint8_t i=0; i<N_Leds; i++){
		aux=0;
		aux = G;
		aux = (aux<<8)| R;
		aux = (aux<<8)| B;
		/////////////////////////////////////////////////////////// primera trama
		for(uint8_t j=0; j<24; j++){
			if( aux & 0x800000){
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			else{
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			aux<<=1;
			
		}////////////////////////////////////////////////// fin de g
		
		
	}
	PORTB &= 0x00;
	_delay_us(588);
}

void enviar_all_bits(uint32_t dat, uint8_t N_Leds){
	
	uint32_t aux;
	
	// se envia G R B cuyo bit se envia el de mayor peso primero
	for (uint8_t i=0; i<N_Leds; i++){
		aux=dat;

		for(uint8_t j=0; j<24; j++){
			if( aux & 0x800000){
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			else{
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			aux<<=1;
			
		}////////////////////////////////////////////////// fin de g
		
		
	}
	PORTB &= 0x00;
	_delay_us(588);
	
}

void enviar_color_individual(uint32_t dat){
	
	uint32_t aux;
	// se envia G R B cuyo bit se envia el de mayor peso primero
		aux=dat;
		
		for(uint8_t j=0; j<24; j++){
			if( aux & 0x800000){
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			else{
				PORTB |= 0x01; // aprox 167 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				PORTB &= 0x00;// aprox 375 ns
				__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
				
			}
			aux<<=1;
			
		}////////////////////////////////////////////////// fin de g
	
}

void color_posicion(uint8_t* POSICION, uint32_t color){
	uint8_t POS=0;
	for (uint32_t i=1; i<66; i++){
		if(i==POSICION[POS]){
			enviar_color_individual(color); // GGRRBB  A49E9E
			POS++;
		}
		else{
			enviar_color_individual(0x0);
		}
	}
	POS=0;
	_delay_ms(250);
}

void color_serie(uint32_t color){
	for (uint32_t i=0; i<65; i++){
		enviar_all_bits(color,i); // GGRRBB  A49E9E

		_delay_ms(5);
		
	}
}

/*
const uint8_t NUMBERS[] PROGMEM = {
	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	//	0	0-4
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0xF,	//	1	5-9
	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF	//	0	0-4
	
};1,2,3,4,5,6,7,8,9,16,17,24,25,32,33,40,41,48,49,56,57,58,59,60,61,62,63,64

const uint8_t NUMBERS1[] PROGMEM = {
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	0	0-4
	0x0,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0x0,	//	1	5-9
	0x0,	0xF,	0x0,	0x0,	0x0,	0x0,	0xF,	0x0,	//	1	5-9
	0x0,	0xF,	0x0,	0x0,	0x0,	0x0,	0xF,	0x0,	//	1	5-9
	0x0,	0xF,	0x0,	0x0,	0x0,	0x0,	0xF,	0x0,	//	1	5-9
	0x0,	0xF,	0x0,	0x0,	0x0,	0x0,	0xF,	0x0,	//	1	5-9
	0x0,	0xF,	0xF,	0xF,	0xF,	0xF,	0xF,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0
	
};10,11,12,13,14,15,18,23,26,31,34,39,42,47,50,51,52,53,54,55

const uint8_t NUMBERS2[] PROGMEM = {
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	0	0-4
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0xF,	0xF,	0xF,	0xF,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0xF,	0x0,	0x0,	0xF,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0xF,	0x0,	0x0,	0xF,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0xF,	0xF,	0xF,	0xF,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0
	
};19,20,21,22,27,30,35,38,43,44,45,46

const uint8_t NUMBERS3[] PROGMEM = {
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	0	0-4
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0xF,	0xF,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0xF,	0xF,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	//	1	5-9
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0
	
};*/