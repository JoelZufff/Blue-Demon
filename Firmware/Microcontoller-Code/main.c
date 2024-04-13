// --------------- Preprocesadores de microcontrolador -------------- //
#include    <18F26K22.h>                                                // Libreria del Microcontrolador
#fuses      INTRC, NOWDT, NOPROTECT, NOLVP, NOMCLR, CCP2C1, CCP3C6      // Fusibles (Multiplexado de P2B en C0)
#use        delay(internal = 16MHz)                                     // Configuracion de frecuencia y delay
#use        rs232(rcv = pin_c7, xmit = pin_c6, baud = 9600, bits = 8, parity = n) 

#include    <stdlib.h>
#include    "wheel_control.c"

#define     TMR0_val    15535

// --------------------- Direccion de registros --------------------- //
#BYTE       TRISA               = 0xF92
#BYTE       TRISB               = 0xF93
#BYTE       TRISC               = 0xF94
#BIT        status_LED          = 0xF89.0   // LATA

#BYTE       INTCON              = 0xFF2
#BIT        RC1IE               = 0xF9D.5   // PIE1
#BIT        IPEN                = 0xFD0.7   // RCON
#BIT        RC1IP               = 0xF9F.5   // IPR1

#BYTE       T0CON               = 0xFD5
const int16 *TMR0               = 0xFD6;

// ----------------------- Variable Globales ------------------------ //
struct motor_t  tires[2][2]   = 
{
    {   // Llantas delanteras
        { 0xFBE, PIN_B1, PIN_B2 },  // Izquierda
        { 0xF67, PIN_B3, PIN_B4 }   // Derecha
    },
    {   // Llantas traseras
        { 0xF5E, PIN_B5, PIN_B6 },  // Izquierda
        { 0xF58, PIN_C3, PIN_C4 }   // Derecha
    }
};

struct controller_t xbox_controller;

struct bth_conection_t PC = { FALSE, FALSE, FALSE };

// -------------------------- Interrupciones ------------------------ //
#int_rda
void control_instructions()
{
    int data = getch();
    
    if(data == '*')     // Recibimos caracter de inicio de datos
    {   
        char XJoystick[8], YJoystick[8], LeftTrigger[5], RightTrigger[5];
        
        // Obtenemos cadenas de caracteres con los valores a recibir
        gets(XJoystick);
        gets(YJoystick);

        gets(LeftTrigger);
        gets(RightTrigger);

        // Realizamos la conversion y actualizamos los valores del control
        xbox_controller.Joystick[0] =  atol(XJoystick);
        xbox_controller.Joystick[1] =  atol(YJoystick);
        
        xbox_controller.Triggers[0] =  atol(LeftTrigger);
        xbox_controller.Triggers[1] =  atol(RightTrigger);

        // Actualizamos variables de conexion
        PC.updated_data = PC.status = status_LED = TRUE;
        PC.update_request = FALSE;
    }
}

#int_timer0
void request_data()
{
    if(PC.update_request)  // No hay conexion (Ver si hacerlo cada x veces que suceda)
        PC.status = status_LED = FALSE;

    putc('*');
    PC.update_request = TRUE;

    *TMR0 = TMR0_val;
}

/* Inforamcion de variables
* update_request: Boolenao para señalar que se solicito una actualizacion de informacion.
    * update_request = FALSE : Se recibio la actualizacion
    * update_request = TRUE  : Se solicito un actualizacion y no se recibio

* updated_data: Booleano para señalar que hubo una actualizacion de informacion y no ha sido aplicada.
    * updated_data = FALSE   : No hay nueva informacion
    * updated_data = TRUE    : Hay informacion pendiente de aplicar

* status: Booleano para saber si hay conexion con el dispositivo
*/

// ---------------------------- Funciones --------------------------- //
void log_init()
{
    // Configuracion de pines GPIO
    TRISA       = 0b00000000;
    TRISB       = 0b00000000;
    TRISC       = 0b00000000;

    // Configuracion de Interrupciones
    IPEN = RC1IE = RC1IP = TRUE;        // Establecemos la interrupcion de recepcion serial como prioridad
    INTCON      = 0b11100000;

    // Configuracion de timers
    *TMR0       = TMR0_val;
    T0CON       = 0b10000010;
}

// ------------------------ Codigo Principal ----------------------- //
void main()
{
    log_init();
    motors_init();
    
    while (TRUE)
    {
        //*/
        if(!PC.status)
        {
            // Si no hay conexion ponemos modo reposo (Detenido y fijo)
            continue;
        }

        // Si hay conexion
        if(PC.updated_data)                // Si se actualizo la posicion
        {
            drive_tires(&xbox_controller, tires);
            PC.updated_data = FALSE;
        }
        //*/

        /*/ Revision de  almacenamiento de variables
        if(flag)
        {
            printf("Joystick X = %ld | Y = %ld\r\n", Xbox_controller.Joystick[0],  Xbox_controller.Joystick[1]);
            printf("Triggers 1 =  %u | 2 = %u\r\n", Xbox_controller.Triggers[0],  Xbox_controller.Triggers[1]);

            flag  = FALSE;
        }
        //*/
    }
}

/*
El codigo esta compuesto de las siguientes partes

    * Interrupcion de recepcion de datos en puerto serial: Recibe los datos actualizados del control de XBOX desde el programa de computadora en C#

    * Interrupcion de timer 0: Desbore de timer de 100 ms que envia el caracter para solicitar una actualizacion de control y confirmar la conexion bluetooth

    * Funcion principal: Inicializa los registros, y mueve las ruedas cuando hay nueva informacion
*/