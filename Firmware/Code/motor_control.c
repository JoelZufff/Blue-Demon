#define     max_dutycicle   160

// -------------------- Direcciones de registros -------------------- //
#BYTE       CCPTMRS0        = 0xF49
#BYTE       CCPTMRS1        = 0xF48

#BYTE       CCP1CON         = 0xFBD
#BYTE       CCP2CON         = 0xF66
#BYTE       CCP3CON         = 0xF5D
#BYTE       CCP4CON         = 0xF57

#BYTE       T2CON           = 0xFBA
#BYTE       PR2             = 0xFBB

// -------------------------- Estructuras --------------------------- //
struct motor_t
{
    int     *DutyCicle;
    long    IN1_pin;
    long    IN2_pin;
};

// ---------------------------- Funciones --------------------------- //
void motors_init(int number)
{
    // Configuracion de Timers
    T2CON       = 0b00000100;
    PR2         = 39;
    
    // Configuracion de CCPXCON para PWM
    CCPTMRS0 = CCPTMRS0 = 0b00000000;       // Configuramos CCPX con timer 2
    
    switch (number)
    {
        case 4:
            CCP4CON = 0b00001100;
        case 3:
            CCP3CON = 0b00001100;
        case 2:
            CCP2CON = 0b00001100;
        case 1:
            CCP1CON = 0b00001100;
        break;
        default: break;
    }
}

/*  Donde:
* direcition    := Booleano para escoger derecha o izquierda
* speed         := Numero flotante de 0 a 100
*/
void motor_movement(struct motor_t motor, short direction, float velocity)
{
    // Establecemos direccion de giro
    output_bit(motor.IN1_pin, direction);
    output_bit(motor.IN2_pin, !direction);

    // Establecemos DC para velocidad
    long aux = max_dutycicle * velocity;  
    motor->DutyCicle = aux >> 2;
}