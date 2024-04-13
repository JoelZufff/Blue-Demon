using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using SharpDX.XInput;
using System.ComponentModel;
using System.IO.Ports;

namespace Xbox_Controller_to_PIC
{
    internal class Program
    {
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
            if (xbox_controller.IsConnected) 
                Terreneitor.Write("*" + Joystick[0].ToString() + "\r" + Joystick[1].ToString() + "\r" + Triggers[0].ToString() + "\r" + Triggers[1].ToString() + "\r");
            else
                Terreneitor.Write("*0\r0\r0\r0\r");
        }

        static void Main(string[] args)
        {
            // Configuracion de interrupciones
            Terreneitor.DataReceived += Send_data;

            serial_conection:
            // Realizar conexion con microcontrolador por bluetooth
            try
            {
                Terreneitor.Open();
            }
            catch (Exception ex)
            {
                Console.Clear();
                Console.WriteLine(ex.Message);
                Console.Write("Presione una tecla para reintentar");
                Console.ReadKey();

                goto serial_conection;
            }

            while (true)
            {
                if(!Terreneitor.IsOpen)     // Si se desconecta
                    goto serial_conection;
                
                if(xbox_controller.GetState(out var state))     // Si el control esta conectado, actualizamos variables de estado
                {
                    Joystick[0] = state.Gamepad.LeftThumbX; Joystick[1] = state.Gamepad.LeftThumbY;
                    Triggers[0] = state.Gamepad.LeftTrigger; Triggers[1] = state.Gamepad.RightTrigger;
                }

                if(Console.KeyAvailable) 
                {
                    // Imprimimos estatus en consola
                    if(xbox_controller.IsConnected)
                    {
                        Console.Clear();
                        Console.WriteLine("Joystick\t->\t{0}\t{1}", Joystick[0], Joystick[1]);
                        Console.WriteLine("Gatillos\t->\t{0}\t{1}", Triggers[0], Triggers[1]);
                    }
                    else
                    {
                        Console.Clear();
                        Console.WriteLine("No se detecta control");
                    }
                }
            }

        }
    }
}
