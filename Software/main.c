// --------------- Preprocesadores de microcontrolador -------------- //
#include    <18F26K22.h>                                                // Libreria del Microcontrolador
#fuses      INTRC_IO, NOWDT, NOPROTECT, NOLVP, NOMCLR, CCP3C6, CCP2C0   // Fusibles (Multiplexado de P2B en C0)
#use        delay(clock = 16M, crystal = 16M)                           // Configuracion de frecuencia y delay
//#use        rs232(rcv = pin_b7, xmit = pin_b6, baud = 9600, bits = 8, parity = n) 

// --------------------- Direccion de registros --------------------- //
#BYTE       TRISA           = 0xF92
#BYTE       TRISB           = 0xF93
#BYTE       TRISC           = 0xF94

const int16 *ADRES          = 0xFC3;
#BYTE       ADCON0          = 0xFC2
#BIT        DONE            = 0xFC2.1
#BYTE       ADCON2          = 0xFC0

// Registros para el ECCP con PWM complementario
#BYTE       CCP1CON         = 0xFBD
#BYTE       CCPR1L          = 0xFBE
#BYTE       CCPTMRS0        = 0xF49
#BYTE       CCPTMRS1        = 0xF48

#BYTE       CCP2CON         = 0xF66
#BYTE       CCPR2L          = 0xF67

#BYTE       CCP3CON         = 0xF5D
#BYTE       CCPR3L          = 0xF5E

#BYTE       CCP4CON         = 0xF57
#BYTE       CCPR4L          = 0xF58

#BYTE       CCP5CON         = 0xF54
#BYTE       CCPR5L          = 0xF55

#BYTE       T2CON           = 0xFBA
#BYTE       PR2             = 0xFBB

// ----------------------- Variable Globales ------------------------ //


// -------------------------- Interrupciones ------------------------ //


// ---------------------------- Funciones --------------------------- //


// ------------------------ Codigo Principal ----------------------- //
void main()
{
    TRISA       = 0b00000111;
    TRISB       = 0b00000000;
    TRISC       = 0b00000000;

    // ADC
    ADCON0      = 0b00000011;
    ADCON2      = 0b10000000;

    // PWM
    T2CON       = 0b00000111;
    PR2         = 249;
    
    CCPTMRS0 = CCPTMRS0 = 0b00000000;       // Configuramos CCPX con timer 2

    CCP1CON = CCP2CON = CCP3CON = CCP4CON = CCP5CON = 0b10001100;

    int16 ADC;

    while (TRUE)
    {
        ADCON0  = 0b00000011;
        delay_us(10);
        ADC = (*ADRES / 1023.0 * 1000);
        CCPR1L = CCPR4L = ADC >> 2;
        CCPR5L = (1000 - ADC) >> 2;
        //printf("ADCON0 = %lu\r\n", (int16) CCPR1L << 2);

        ADCON0  = 0b00000111;
        delay_us(10);
        ADC = (*ADRES / 1023.0 * 1000);
        CCPR2L  = ADC >> 2;

        ADCON0  = 0b00001011;
        delay_us(10);
        ADC = (*ADRES / 1023.0 * 1000);
        CCPR3L  = ADC >> 2;
    }
}