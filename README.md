**Robot para competencia Minisumo**
Este fue uno de mis proyectos más demandantes, ya que, además de las limitaciones de tamaño y peso de la competencia, todo se creó desde cero, sin comprar diseños preexistentes ni módulos de control preprogramados.

**Diseño:**  
Se diseñó de manera personalizada desde cero en el software SOLIDWORKS, teniendo en cuenta las restricciones de tamaño de la competencia.

- Se priorizó el máximo aprovechamiento del espacio, asegurando una distribución adecuada del peso para lograr un diseño estable.
- La carcasa fue impresa en 3D con filamento PETG de color azul. Se eligió este material por su mayor dureza en comparación con el PLA.
- La base fue fabricada en una placa de aluminio con la ayuda de un profesor, a partir de un modelo 3D.

**Programación de microcontrolador:**  
La programación se realizó utilizando PIC C COMPILER.

- Se configuraron los periféricos manipulando registros a través de punteros, obteniendo las direcciones de memoria de la hoja de datos del fabricante.
- Se implementó una codificación robusta y fácil de entender, aprovechando las herramientas del lenguaje C, como funciones selectivas, iterativas, estructuras y punteros.
- Se utilizaron **TIMERS** para generar 4 señales PWM y un timer con interrupción exclusivo para verificar la conexión con el control.
- Se empleó el protocolo de comunicación serial **RS232** para recibir las instrucciones del controlador.

**Programación de control:**  
Se desarrolló un programa en **C#** para Windows que actuó como intermediario entre el control y el robot Minisumo.

- Se aprovechó la compatibilidad del software para obtener la posición de los joysticks, botones y gatillos de un control de Xbox. Los datos obtenidos se organizaban en una cadena de caracteres siguiendo nuestro propio protocolo.
- Se utilizó un **TIMER** para enviar los datos recibidos del control a una tasa de 40 Hz. Estos datos se transmitían al microcontrolador del robot a través de Bluetooth utilizando el protocolo **RS232**.
- Los datos se enviaban a una tasa fija para que el microcontrolador pudiera detectar una desconexión cuando no se recibían datos en un periodo determinado.

**Componentes electrónicos:**  

- **Microcontrolador PIC 18F26K22**: Microcontrolador de 16 MHz con 5 módulos CCPX, lo que nos permitió generar 4 señales PWM independientes.
- **Módulo Bluetooth HC-05**: Módulo que facilitó la recepción de datos desde la computadora al microcontrolador mediante Bluetooth.
- **4 micromotorreductores de 150 rpm**: Motores con engranajes metálicos que ofrecieron un buen equilibrio entre torque y velocidad.
- **Regulador de voltaje L7805**: Acompañado de un condensador, ayudó a regular el voltaje de la batería para alimentar el microcontrolador.
- **Driver puente H TB6612FNG**: Driver eficiente para controlar los micromotores, capaz de trabajar a frecuencias más altas que los comunes L298N.
- **Batería de 2000 mAh a 7.4 V**: Batería para coches de radiocontrol, que cumplió con los requisitos de voltaje y corriente del sistema.
