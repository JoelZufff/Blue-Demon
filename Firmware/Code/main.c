// --------------- Preprocesadores de microcontrolador -------------- //
#include    <18F26K22.h>                                                // Libreria del Microcontrolador
#fuses      INTRC_IO, NOWDT, NOPROTECT, NOLVP, NOMCLR, CCP2C1, CCP3C6   // Fusibles (Multiplexado de P2B en C0)
#use        delay(internal = 16MHz)                                     // Configuracion de frecuencia y delay
//#use        rs232(rcv = pin_b7, xmit = pin_b6, baud = 9600, bits = 8, parity = n) 

#include    "motor_control.c"

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

// -------------------------- Estructuras --------------------------- //


// ----------------------- Variable Globales ------------------------ //
struct motor_t wheels[4] = 
{
    { 0xFBE, PIN_B1, PIN_B2 },  /* Rueda A */ 
    { 0xF67, PIN_B3, PIN_B4 },  /* Rueda B */
    { 0xF5E, PIN_B5, PIN_B6 },  /* Rueda C */
    { 0xF58, PIN_C3, PIN_C4 }   /* Rueda D */
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
}

// ------------------------ Codigo Principal ----------------------- //
void main()
{
    log_init();
    motors_init(4);

    int1 direction = LEFT;

    while (TRUE)
    {
        // Solicitamos y almacenamos lectura de ADC
        DONE = TRUE, delay_us(10);
        //velocity = (*ADRES / 1023.0);
        
        for(int i = 0; i < 4 ; i++)
            motor_movement( wheels[i], (i < 2 ? direction : !direction), *ADRES / 1023.0 );

        if(BUTTON)
        {
            while(BUTTON);
            direction ^= 1;
        }
    }
}