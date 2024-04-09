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
        static void Main(string[] args)
        {
            // Realizar conexion con microcontrolador
            Controller xbox_controller = new Controller(UserIndex.One);
            /*SerialPort Terreneitor = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);

            if (!Terreneitor.IsOpen)
            {
                Console.WriteLine("No se establecio conexion");
                Console.ReadKey();
                return;
            }*/

            // Utilizare las 2 palancas, una para acelerar y la otra para acelerar en reversa
            while (true)
            {
                if(xbox_controller.GetState(out var state))
                {
                    short[] Joystick = { state.Gamepad.LeftThumbX, state.Gamepad.LeftThumbY };
                    byte[] Triggers  = { state.Gamepad.LeftTrigger, state.Gamepad.RightTrigger };

                    // Obtenemos valor de coordenadas de Joystick y lo dividimos en 2 bytes para envio
                    byte[] JoystickX_bytes = BitConverter.GetBytes(Joystick[0]);
                    byte[] JoystickY_bytes = BitConverter.GetBytes(Joystick[1]);

                    // Enviamos valores de control a microcontrolador
                    //Console.Write("J" + (char) JoystickX_bytes[0] + (char) JoystickX_bytes[1] + (char) JoystickY_bytes[0] + (char) JoystickY_bytes[1] + "\n");
                    //Console.Write("T" + (char) Triggers[0] + (char) Triggers[1] + "\n");
                    
                    if(Console.KeyAvailable)
                    {
                        Console.Clear();

                        // Imprimimos estatus en consola
                        Console.Write("Joystick\t->\t{0}\t{1}\n", Joystick[0], Joystick[1]);
                        Console.Write("Gatillos\t->\t{0}\t{1}", Triggers[0], Triggers[1]);
                        Console.ReadKey();
                    }
                }
            }

        }
    }
}
