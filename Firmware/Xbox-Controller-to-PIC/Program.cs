using System;
using SharpDX.XInput;
using System.IO.Ports;
using System.Timers;

namespace Xbox_Controller_to_PIC
{
    internal class Program
    {
        // Timer
        public static Timer Send_timer = new Timer(100);
        // Puerto Serial
        public static SerialPort Terreneitor = new SerialPort("COM8", 9600, Parity.None, 8, StopBits.One);
        // Control de xbox
        public static Controller xbox_controller = new Controller(UserIndex.One);
        // Variables Globales
        public static short[] Joystick = new short[2];
        public static byte[] Triggers = new byte[2];

        static void Send_data(object sender, EventArgs e)
        {
            // Enviamos valores de control a microcontrolador
            Console.Clear();
            
            if (xbox_controller.IsConnected)
            {
                try
                {
                    Terreneitor.Write("*" + Joystick[0].ToString() + "\r" + Joystick[1].ToString() + "\r" + Triggers[0].ToString() + "\r" + Triggers[1].ToString() + "\r");
                }
                catch 
                {
                    Terreneitor.Close();
                    return;
                }
                
                Console.WriteLine("Joystick\t->\t{0}\t{1}", Joystick[0], Joystick[1]);
                Console.WriteLine("Gatillos\t->\t{0}\t{1}", Triggers[0], Triggers[1]);
            }
            else
            {
                Terreneitor.Write("*0\r0\r0\r0\r");
                Console.WriteLine("No se detecta control");
            }
        }

        static void Main(string[] args)
        {
            // Configuracion de interrupciones
            Send_timer.Elapsed += Send_data;

            // Configuracion de timer
            Send_timer.Enabled = true;
            Send_timer.AutoReset = true;

            serial_conection:
            // Realizar conexion con microcontrolador por bluetooth
            Send_timer.Stop();

            try
            {
                Terreneitor.Open();
            }
            catch (Exception ex)
            {
                Console.Clear();
                Console.WriteLine(ex.Message);
                Console.Beep();

                goto serial_conection;
            }

            Send_timer.Start();

            
            while (true)
            {
                if (!Terreneitor.IsOpen)
                    goto serial_conection;

                if(xbox_controller.GetState(out var state))     // Si el control esta conectado, actualizamos variables de estado
                {
                    Joystick[0] = state.Gamepad.LeftThumbX; Joystick[1] = state.Gamepad.LeftThumbY;
                    Triggers[0] = state.Gamepad.LeftTrigger; Triggers[1] = state.Gamepad.RightTrigger;
                }
            }

        }
    }
}
