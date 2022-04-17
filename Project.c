#include <mega32a.h>
#include <delay.h>
 
#define SegDataPort       PORTB
#define SegDataPin        PINB
#define SegDataDDR        DDRB
 
#define SegCntrlPort       PORTC
#define SegCntrlPin        PINC
#define SegCntrlDDR        DDRC
 
int hours = 0;
int minutes = 0;
int seconds = 0;
unsigned char data[10] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f };
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
TCNT1H=0xBDC >> 8;
TCNT1L=0xBDC & 0xff;
    seconds++;
 
    if(seconds == 60)
    {
        seconds = 0;
        minutes++;
    }
    if(minutes == 60)
    {
        minutes = 0;
        hours++;        
    }
    if(hours > 23)
        hours = 0;
}
 
void main(void)
{
SegDataDDR = 0xFF;
SegCntrlDDR = 0x3F;
SegCntrlPort = 0xFF;
 
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x0B;
TCNT1L=0xDC;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
#asm("sei");

    while(1)
    {
        if((SegCntrlPin & 0x40) == 0 )
        {    
            delay_ms(200);
            if(minutes < 59)
                minutes++;
            else
                minutes = 0;
        }         
        if((SegCntrlPin & 0x80) == 0 )
        {    
            delay_ms(200);
            if(hours < 23)
                hours++;
            else
                hours = 0;
        }                     
        
        SegCntrlPort = ~0x01;
        SegDataPort = data[seconds%10];
        delay_ms(5);
        
        SegCntrlPort = ~0x02;
        SegDataPort = data[seconds/10];
        delay_ms(5);
        
        SegCntrlPort = ~0x04; 
        SegDataPort = data[minutes%10];
        delay_ms(5);
        
        SegCntrlPort = ~0x08;
        SegDataPort = data[minutes/10]; 
        delay_ms(5);
        
        SegCntrlPort = ~0x10;  
        SegDataPort = data[hours%10]; 
        delay_ms(5);
        
        SegCntrlPort = ~0x20;    
        SegDataPort = data[hours/10];
        delay_ms(5);
    }
}