// --------------- Preprocesadores de microcontrolador -------------- //
#include    <18F26K22.h>                                                // Libreria del Microcontrolador
#fuses      INTRC_IO, NOWDT, NOPROTECT, NOLVP, NOMCLR, CCP2C1, CCP3C6   // Fusibles (Multiplexado de P2B en C0)
#use        delay(internal = 16MHz)                                     // Configuracion de frecuencia y delay
//#use        rs232(rcv = pin_b7, xmit = pin_b6, baud = 9600, bits = 8, parity = n) 

#define RIGHT   1
#define LEFT    0

// --------------------- Direccion de registros --------------------- //
#BYTE       TRISA           = 0xF92
#BYTE       TRISB           = 0xF93
#BYTE       TRISC           = 0xF94
#BIT        BUTTON          = 0xF80.1

const int16 *ADRES          = 0xFC3;
#BYTE       ADCON0          = 0xFC2
#BIT        DONE            = 0xFC2.1
#BYTE       ADCON2          = 0xFC0

#BYTE       CCPTMRS0        = 0xF49
#BYTE       CCPTMRS1        = 0xF48

#BYTE       CCP1CON         = 0xFBD
#BYTE       CCP2CON         = 0xF66
#BYTE       CCP3CON         = 0xF5D
#BYTE       CCP4CON         = 0xF57

#BYTE       T2CON           = 0xFBA
#BYTE       PR2             = 0xFBB

// -------------------------- Estructuras --------------------------- //
struct wheel_t
{
    int     *DutyCicle;
    long    in_pins[2];
};

// ----------------------- Variable Globales ------------------------ //
struct wheel_t wheels[4] = 
{
    { 0xFBE, { PIN_B1, PIN_B2 } },  /* Rueda A */ 
    { 0xF67, { PIN_B3, PIN_B4 } },  /* Rueda B */
    { 0xF5E, { PIN_B5, PIN_B6 } },  /* Rueda C */
    { 0xF58, { PIN_C3, PIN_C4 } }   /* Rueda D */
};

// -------------------------- Interrupciones ------------------------ //


// ---------------------------- Funciones --------------------------- //
void log_init()
{
    // Configuracion de pines GPIO
    TRISA       = 0b00000011;
    TRISB       = 0b00000000;
    TRISC       = 0b00000000;

    // Configuracion de ADC
    ADCON0      = 0b00000011;
    ADCON2      = 0b10000000;

    // Configuracion de Timers
    T2CON       = 0b00000111;
    PR2         = 249;
    
    // Configuracion de CCPXCON para PWM
    CCPTMRS0 = CCPTMRS0 = 0b00000000;       // Configuramos CCPX con timer 2
    CCP1CON = CCP2CON = CCP3CON = CCP4CON = 0b00001100;
}

void wheel_movement(int wheel_index, int1 direction)
{
    output_bit(wheels[wheel_index].in_pins[0], direction);
    output_bit(wheels[wheel_index].in_pins[1], !direction);
}

// ------------------------ Codigo Principal ----------------------- //
void main()
{
    log_init();

    int1 direction = LEFT;
    int16 ADC;

    for(int i = 0; i < 4 ; i++)
        wheel_movement(i, direction);

    while (TRUE)
    {
        ADCON0  = 0b00000011;
        delay_us(10);
        ADC = (*ADRES / 1023.0 * 1000);
        
        for(int i = 0; i < 4 ; i++)
            (wheels[i])->DutyCicle = ADC >> 2;

        if(BUTTON)
        {
            while(BUTTON);
            direction ^= 1;
            
            wheel_movement(0, direction);
            wheel_movement(1, direction);
            wheel_movement(2, direction);
            wheel_movement(3, direction);
        }
    }
}