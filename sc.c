// Fuses (E:FF, H:FF, L:73) - 128kHz as cpu clock

#include <avr/io.h>
#include <avr/interrupt.h>

char curState, prevState, counter;

void delay(void){
  unsigned int i;
  for (i=25;--i;) {
    asm("nop");
  }
}

inline void decr(void) {
  counter--;
  if (counter<-3) {
    counter+=4;
    PORTB &= ~(1<<PB4);
    delay();
    PORTB |= (1<<PB4);
  }
}
inline void incr(){
  counter++;
  if (counter>3) {
    counter-=4;
    PORTB &= ~(1<<PB3);
    delay();
    PORTB |= (1<<PB3);
  }
}

ISR(PCINT0_vect, ISR_NAKED) {

  prevState=curState;
  curState=PINB & 3;

  if (prevState==3){
    if (curState==2) {decr();}
    else if (curState==1) {incr();}
  } else if (curState==3){
    if (prevState==1) {decr();}
    else if (prevState==2) {incr();}
  }
  if (prevState==0){
    if (curState==2) {incr();}
    else if (curState==1) {decr();}
  } else if (curState==0){
    if (prevState==1) {incr();}
    else if (prevState==2) {decr();}
  }

  if (curState == 3) counter=0;
  reti();
}

int main(void){

  DDRB = (1<<PB3 | 1<<PB4); // 3, 4 output pins
  PORTB = (1<<PB3 | 1<<PB4) | (1<<PB0 | 1<<PB1); // 0, 1 pullups on encoder

  GIMSK = (1<<PCIE);
  PCMSK = (1<<PCINT1 | 1<<PCINT0);

  ACSR = 1<<7; // Analog Comparator disable, affects power usage during run mode
  ADCSRA &= ~(1<<ADEN);  // disable ADC. I'm pretty sure it's disabled by default though.

  MCUCR |= (1<<SE | 1<<SM1); // sleep mode enable, power down

  sei();

  while(1) {
    asm("sleep");
  };
}
