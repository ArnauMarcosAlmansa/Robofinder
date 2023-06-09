<h1 style="font-size: 48px;">ROBOFINDER</h1>

# Introducción
<p align="justify">
Robofinder es un proyecto de la asignatura de Robótica del Grado en Ingeniería Informática de la Universidad Autónoma de Barcelona.
</p>

<p align="justify">
Este robot ha sido diseñado para la exploración y el mapeo de entornos. Gracias al algorismo de Simultaneous Localization and Mapping (SLAM), RoboFinder tiene la capacidad de generar mapas tridimensionales detallados mientras se mueve con autonomía. Robofinder es un robot móvil autónomo capaz de navegar por un edificio y poder encontrar todo tipo de objetos y localizarse a sí mismo a partir de estos.  Este es un proyecto basado en lo que se conoce como visual SLAM. 
</p>

<div align="center">
    <img src="https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/Imagen_robot.jpg" alt="Foto" width="80%">
</div>

# Tabla de Contenidos
- [Introducción](#introducción)
- [Características](#características)
- [Hardware](#hardware)
    - [Componentes](#componentes)
    - [Circuito](#circuito)
    - [Diseño y Materiales](#diseño-y-materiales)  
- [Software](#software)
- [Resultado Final](#resultado-final)
- [Demo Video](#demo-video)
- [Agradecimientos](#agradecimientos)
- [Autores](#autores)


# Características

<p align="justify">
Su sistema se basa en una cámara estéreo y encoders, permitiéndole obtener una comprensión precisa de su ubicación en todo momento.
</p>

<p align="justify">
RoboFinder cuenta con un potente sistema de control. Una Jetson Nano se encarga del procesamiento, mientras que la comunicación I2C asegura una transmisión de información fluida y confiable entre los componentes. Este enfoque de control inteligente garantiza movimientos precisos y coordinados en cada acción que realiza.
</p>

<p align="justify">
Además de su capacidad de mapeo y navegación, RoboFinder se preocupa por la seguridad. Sus sensores de ultrasonido le permiten detectar obstáculos cercanos y detenerse de manera segura para evitar colisiones. Para brindar una experiencia visualmente atractiva, hemos incorporado LEDs que indican el movimiento del robot, añadiendo un toque estético a su funcionalidad.
</p>

<p align="justify">
Representa un avance significativo en la exploración robótica, abriendo nuevas posibilidades en campos como la cartografía, la inspección de entornos complejos y la investigación científica. Su combinación de tecnología innovadora y capacidad de adaptación lo convierten en una herramienta versátil y confiable.
</p>

<p align="justify">
Descubre con RoboFinder un mundo de posibilidades en la exploración y mapeo de entornos. Siéntete parte de la revolución robótica y únete a nosotros en la búsqueda de soluciones avanzadas para desafíos del mundo real
</p>



# Hardware
## Componentes
Principales componentes utilizados:

                                                                                               
 Componente                                  |        Componente                                   |              
|:--------------------------------------------:|:--------------------------------------------:|
| **Nvidia Jetson Nano Developer Kit** <div style="width: 50%; height: 50%;margin:auto">![Jetson Nano](/Im%C3%A1genes%20para%20Readme/Jetson_Nano.png)</div> | **DUO MLX - USB Stereo Camera** <div style="width: 50%; height: 50%;margin:auto">![DUO MLX - USB Stereo Camera](/Im%C3%A1genes%20para%20Readme/Camara%20duo.PNG)</div> |
| **1 Arduino Mega** <div style="width: 50%; height: 50%;margin:auto">![1 Arduino Mega](/Im%C3%A1genes%20para%20Readme/mega.jpg)</div>   | **2 Sensores de distancia por ultrasonidos HC-SR04** <div style="width: 50%; height: 50%;margin:auto">![2 Sensores de distancia por ultrasonidos HC-SR04](/Im%C3%A1genes%20para%20Readme/ULtrasonido.PNG)</div> |
| **2 Motor	Micro Metal 150:1** <div style="width: 50%; height: 50%;margin:auto">![2 Motor	Micro Metal 150:1](/Im%C3%A1genes%20para%20Readme/motor.PNG)</div> | **2 Ruedas antideslizantes con encoders** <div style="width: 50%; height: 50%;margin:auto">![2 Ruedas antideslizantes con encoders](/Im%C3%A1genes%20para%20Readme/rueda_con_encoder.PNG)</div>                        |
| **1 Rueda Loca Metálica** <div style="width: 50%; height: 50%;margin:auto">![Rueda loca](/Im%C3%A1genes%20para%20Readme/rueda_loca.PNG)</div>| **Power Bank 20A** <div style="width: 50%; height: 50%;margin:auto">![Power Bank 20A](/Im%C3%A1genes%20para%20Readme/powerbanl.PNG)</div>|





## Circuito
Esquema global de Robofinder:

<div align="center">
    <img src="https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/RoboFinder_esquem%C3%A1tico.png" alt="Circuito" width="80%">
</div>


<p align="justify">
El Arduino tiene conectado el controlador de motores (L298N), los dos sensores de ultrasonido (HC-R04) y los encoders para detectar el movimiento de los motores.
Aparte el Arduino se comunica con la Jetson Nano mediante I2C. 
En la Jetson Nano tenemos conectada la cámara estéreo (DUO 3D MLX) mediante USB y la conexión de I2C mediante GPIO (puerto 3 SDA , puerto 5 SCL y puerto 6 GND).
</p>

# Diseño y Materiales
Diseño final de la estructura:


 Componente                                  |        Componente                                   |              
|:--------------------------------------------:|:--------------------------------------------:|
| **Base del Robot** <div style="width: 50%; height: 50%; margin:auto;">![Base del Robot](/Im%C3%A1genes%20para%20Readme/Piezas%203D/Base%20Robofinder.png)</div> | **Piso intermedio y superior del Robot** <div style="width: 50%; height: 50%; margin:auto">![Piso intermedio y superior del Robot](/Im%C3%A1genes%20para%20Readme/Piezas%203D/Piso%20Robofinder.png)</div>
| **Soporte de la cámara 3D** <div style="width: 50%; height: 50%; margin:auto">![Soporte de la cámara 3D](/Im%C3%A1genes%20para%20Readme/Piezas%203D/Soporte%20Camara%20Robofinder.png)</div> | **Soporte para los sensores de ultrasonido** <div style="width: 50%; height: 50%; margin:auto">![Soporte para los sensores de ultrasonido](/Im%C3%A1genes%20para%20Readme/Piezas%203D/Soporte_ultrasonido.png)</div>
| **Soporte para la matriz led** <div style="width: 50%; height: 50%; margin:auto">![Soporte para la matriz led](/Im%C3%A1genes%20para%20Readme/Piezas%203D/32x8_matrix_v09_mod.png)</div> | **Soporte para el motor y aspa** <div style="width: 50%; height: 50%; margin:auto">![Soporte para el motor y aspa](/Im%C3%A1genes%20para%20Readme/Piezas%203D/fan%2Bbase.png)</div>


Si quieres ver el diseño en 3D en detalle, presiona en esta imagen:

<div align="center">
<a href="https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/models/Piezas_robofinder_montado.stl">
  <img src="/Im%C3%A1genes%20para%20Readme/Piezas%203D/Robofinder_montado_x2.jpg" width="80%">
</a>
</div>




# Software
## Diagrama Software:

<div align="center">
    <img src="https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/Diagrama%20Software.png" alt="Diagrama Software" width="80%">
</div>

## Módulos para el Arduino:

<div align="justify">
    
- **Módulo de control de motores:** Encargado de comandar a los motores: cómo tienen que moverse, velocidad, sentido; y qué motor debe moverse para poder moverse en línea recta o hacer curvas sobre su propio eje o abiertas; también poder frenar por si detecta algún imprevisto.

- **Módulo de sensores encoder:** Para poder leer datos de los encoders, cuantas vueltas dan las ruedas, o cuando completan una vuelta, etc.

- **Módulo de sensores de ultrasonido:** Para poder obtener los valores de los sensores de ultrasonido y poder alertar sobre objetos en la trayectoria.

- **Módulo de comunicación I2C:** Para poder comunicarse con la Jetson Nano. Tanto para recibir como enviar información, respecto a cuando hay que empezar a moverse (sentido, velocidad, posición final) o si hay que pararse. Permite comunicar los sensores de ultrasonido y los encoders con la Jetson para que así esta pueda tener los valores de estos sensores.

- **Módulo de control (interpretar órdenes que da la Jetson, capacidad de responder a imprevistos):** Entender las órdenes que recibimos de la Jeston Nano y poder gestionar las interrupciones que nos pueden dar los sensores de ultrasonido si un objeto inesperado se nos cruza en nuestra trayectoria.

- **Módulo de leds:** Permite definir distintos patrones en la matriz led que tiene el robot. Esta nos notifica cuando el robot se mueve, hacia qué dirección lo hace o si está parado.
    
</div>
    
    
## Módulos para la Jetson:

<div align="justify">

- **Módulo de comunicación I2C** Para poder comunicar con el Arduino cuando ha de empezar a mover los motores, qué motores, a que velocidad, también para recibir datos de los sensores de ultrasonido y los valores de los encoders. La comunicación se hace usando un protocolo de mensajes que hemos definido nosotros. Este protocolo envia un byte indicando el tipo de mensaje y un payload de datos.

- **Módulo de Mapa**
Generar un mapa de ocupación 3D del espacio donde se mueve el robot para poder navegar y consultar que tiene el robot a su alrededor y no colisionar con los objetos. Esto nos permite intentar localizar el robot y navegar teniendo en cuenta parte del entorno que el robot no está viendo en un momento concreto.
Hemos hecho uso de la librería OctoMap (https://github.com/OctoMap/octomap).

- **Módulo de Visión**
Este módulo es el encargado de hacer las imágenes con la cámara estéreo. 
Aplicamos el detector de características ORB para encontrar puntos de interés junto con BruteForce Matcher. Hacemos la correspondencia entre los puntos de ambas imágenes y filtramos las correspondencias para quitar las malas. Esto lo hacemos con el ratio de Lowe y filtrando las que tienen coordenada ‘y’ muy diferente. 
Triangulamos los puntos en el espacio 3D de la cámara. Luego los convertimos al espacio 3D del mundo.
Para hacer el procesamiento usamos OpenCV con CUDA.

- **Módulo de Localización**
Este módulo se encarga de intentar ubicar al robot dentro del espacio usando la información del mapa y los puntos que detecta la cámara.
Para conseguir ubicar el robot este módulo incorpora la localización de Monte Carlo para poder determinar en que zona está el robot. 
Concretamente, generamos partículas en un rango probable de error del robot. Para asignar a cada partícula un peso hacemos RayCast desde la posición y orientación de una especie de cámara virtual de cada partícula y hacemos un KNN con k = 1 con los puntos detectados de la cámara, asignando un peso menor a aquellas partículas que tienen menor correspondencia con los puntos de la cámara.
Después de esto, hacemos un resampling de partículas con el algoritmo llamado Random Wheel, que consiste en  generar nuevas partidas a partir del peso cumulativo de las anteriores.
Todo el proceso anterior se itera cinco veces para refinar y aproximar la posición estimada del robot.
Para este módulo utilizamos la parte de ML de OpenCV.

- **Módulo de Navegación**
Este módulo se encarga de planificar la navegación local.
Este módulo es el que se encarga de generar las órdenes que finalmente llegan al Arduino para la gestión de los motores y de los sensores. 
Teniendo en cuenta la posición del robot, hacemos raycast 360 grados alrededor del robot a 15 alturas diferentes de 0.5 cm a 15.5 cm, que es la altura del robot. A partir de los datos del raycast, el robot busca un ángulo donde no haya obstáculos, mira que no haya obstáculos con los ultrasonidos y avanza.

</div>



    
# Resultado Final


# Demo Video
Presiona la miniatura para visualizar el video en youtube de todo el proceso!


[![Demo del proyecto](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/miniatura.png)](https://youtu.be/TqlxrmKnXCs/)



# Agradecimientos
<p align="justify">
Queremos destacar y agradecer especialmente a la UAB, junto a los profesores de esta asignatura por proporcionarnos los mejores componentes posibles y por el apoyo brindado semanalmente.
</p>

[Universitat Autònoma de Barcelona (UAB)](https://www.uab.cat/web/universitat-autonoma-de-barcelona-1345467954409.html)

<p align="justify">
También al equipo del Open lab por facilitarnos la impresión 3D, el material y todas las herramientas que nos han permitido llevar a cabo nuestro robot.
</p>

[Open Labs UAB](https://www.uab.cat/open-labs/)


# Autores
- [Pere Pérez López](https://github.com/Pachi23)

- [Lluís Alonso Pichardo](https://github.com/lluisalosnsop)

- [Arnau Marcos Almansa](https://github.com/ArnauMarcosAlmansa)

- [Sergi Sanz Orellana](https://github.com/sergisanzorellana)

