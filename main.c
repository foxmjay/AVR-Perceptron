#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdfix.h>


#define BNI0  (PINC & (1<<0))
#define BNI1  (PINC & (1<<1))
#define BNI2  (PINC & (1<<2))
#define BNI3  (PINC & (1<<3))
#define BNO0  (PINC & (1<<4))
#define BNO1  (PINC & (1<<5))
#define BNO2  (PIND & (1<<3))
#define BNO3  (PINB & (1<<5))

#define TRAINB 2

#define LEDI0(stat) ( stat ? (PORTD |= (1<<4)) : (PORTD &= ~(1<<4)))
#define LEDI1(stat) ( stat ? (PORTD |= (1<<5)) : (PORTD &= ~(1<<5)))
#define LEDI2(stat) ( stat ? (PORTD |= (1<<6)) : (PORTD &= ~(1<<6)))
#define LEDI3(stat) ( stat ? (PORTD |= (1<<7)) : (PORTD &= ~(1<<7)))
#define LEDO0(stat) ( stat ? (PORTB |= (1<<0)) : (PORTB &= ~(1<<4)))
#define LEDO1(stat) ( stat ? (PORTB |= (1<<1)) : (PORTB &= ~(1<<4)))
#define LEDO2(stat) ( stat ? (PORTB |= (1<<2)) : (PORTB &= ~(1<<4)))
#define LEDO3(stat) ( stat ? (PORTB |= (1<<3)) : (PORTB &= ~(1<<4)))

#define psize  5
#define TRAINB 2



int main(void) {    


    // set the PINs direction
    DDRD = 0xf0;
    DDRB = 0x0f;
    DDRC = 0x00;
    
    // clear PINs stat

    PORTD =0x00;
    PORTB =0x00;
    PORTC =0x00;
   

    short _Fract learnConst=0.9;				// learnnig constant
    short _Accum weights[psize]={0.5,-0.1,0.8,-0.6,0.6};	// initial weights
 
  for(;;){


     short _Accum inputs[psize]={0,0,0,0,1};
     short _Accum keypressed =-1;    

 	// get the input button
   while(keypressed<0){
      PORTD =0x00;
      PORTB =0x00;

    if(!BNI0){LEDI0(1);inputs[0]=1;keypressed =0;}
    if(!BNI1){LEDI1(1);inputs[1]=1;keypressed =1;}
    if(!BNI2){LEDI2(1);inputs[2]=1;keypressed =2;}
    if(!BNI3){LEDI3(1);inputs[3]=1;keypressed =3;}
  }
    
    

    short _Accum  guess=0;
    uint8_t i=0;
    // guess the result       
      for(i=0;i<psize;i++){
        if(inputs[i]!=0)guess+=weights[i];

      }
 
      short _Accum g = (int)guess;		// rouding the float value if >=0.5 it count as 1
      if((guess - g)>=0.5)
	g++;
     
     if(g==0)LEDO0(1);
     if(g==1)LEDO1(1);
     if(g==2)LEDO2(1);
     if(g==3)LEDO3(1);
    
     if(PIND & (1<<TRAINB)){		// in training mode

	  keypressed=-1;
	 
	  // get the output button
	  while(keypressed<0){

		 if(!BNO0){keypressed =0;}
		 if(!BNO1){keypressed =1;}
		 if(!BNO2){keypressed =2;}
		 if(!BNO3){keypressed =3;}
	  }
	  // caclulate the error
	 short _Accum error = keypressed-guess;

	 // re-update  the weights
	 for(i=0;i<psize;i++){
             weights[i] +=learnConst*error*inputs[i];
         }	
     }

   	_delay_ms(10);
  }


}
