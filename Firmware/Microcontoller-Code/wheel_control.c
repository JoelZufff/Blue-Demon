#define     FORWARD     1
#define     BACK        0

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
struct bth_conection_t
{
    short updated_data;
    short update_request;
    short status;
};

struct motor_t
{
    int     *DutyCicle;
    long    IN1_pin;
    long    IN2_pin;
};

struct controller_t
{
    signed long     Joystick[2];
    int             Triggers[2];
};

// ---------------------------- Funciones --------------------------- //
void motors_init()
{
    // Configuracion de Timers
    T2CON       = 0b00000100;
    PR2         = 39;
    
    // Configuracion de CCPXCON para PWM
    CCPTMRS0 = CCPTMRS0 = 0b00000000;       // Configuramos CCPX con timer 2
    
    // Configiracion de modulo CCPX para PWM
    CCP4CON = 0b00001100;
    CCP3CON = 0b00001100;
    CCP2CON = 0b00001100;
    CCP1CON = 0b00001100;
}

void motor_movement(struct motor_t *motor, short direction, int velocity)   // Modo de movimiento normal
{
    // Establecemos direccion de giro
    output_bit(motor->IN1_pin, direction);
    output_bit(motor->IN2_pin, !direction);

    // Establecemos DC para velocidad
    long aux = max_dutycicle * velocity / 100;      // Ver si eliminar
    *(motor->DutyCicle) = aux >> 2;     // Revisar
}

void motor_shortbrake(struct motor_t *motor)    // Modo de freno de corto (Solo para emergencias)
{
    output_high(motor->IN1_pin);
    output_high(motor->IN2_pin);
}

void drive_tires(struct controller_t *xbox_controller, struct motor_t tires[2][2])
{
    short direction     = FORWARD;
    signed long diff    = xbox_controller.Triggers[0] - xbox_controller.Triggers[0];

    // Establecemos direccion de desplazamiento y velocidad en funcion de triggers
    if(diff > 0)   // Si esta avanzando mas que retrocediendo
        direction = FORWARD;
    else if(diff < 0)  // Si esta retrocediendo mas que avanzando
        direction = BACK;
    else if(xbox_controller.Triggers[0] == 255)     // Si estal al maximo ambos (Ver si con 255 basta)
    {
        motor_shortbrake(&tires[0][0]), motor_shortbrake(&tires[0][1]);
        motor_shortbrake(&tires[1][0]), motor_shortbrake(&tires[1][1]);
        
        return;
    }

    // Ya que tenemos la direccion de movimiento, mocemos las llantas en funcion de la direccion de joysticks


}

/*
Ir recto: Las 4 llantas giran a la misma velocidad
Girar hacia un lado: Una llanta gira a una velocidad y la otra a una velocidad menor o directamente en sentido contrario

Puedo usar eje y del joystick para definir que tanto se avanza al girar a la derecha o izquierda
*/
