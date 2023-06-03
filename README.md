<h1 style="font-size: 48px;">ROBOFINDER</h1>

# Introducción
<p align="justify">
Robofinder es un proyecto de la asignatura de Robótica del Grado en Ingeniería Informática de la Universidad Autónoma de Barcelona.
</p>

<p align="justify">
Este robot ha sido diseñado para la exploración y el mapeo de entornos. Gracias al algorismo de Simultaneous Localization and Mapping (SLAM), RoboFinder tiene la capacidad de generar mapas tridimensionales detallados mientras se mueve con autonomía. 
</p>


![Ejemplo de imagen](images/ejemplo.jpg)

# Tabla de Contenidos
- [Introducción](#introducción)
- [Características](#características)
- [Hardware](#hardware)
    - [Componentes](#componentes)
    - [Circuito](#circuito)

- [Impresión 3d](#impresión-3d)  
- [Requisitos de Software](#requisitos-de-software)
- [Instalación](#instalación)
- [Uso](#uso)
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
                                                                                               
 Componente                                  |                            | Componente                                   |                                |
|--------------------------------------------|----------------------------|--------------------------------------------|----------------------------|
| <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/Jetson_Nano.png)</div>               | Nvidia Jetson Nano Developer Kit               | <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/Camara%20duo.PNG)</div>               | DUO MLX - USB Stereo Camera            |
| <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/mega.jpg)</div>               | Arduino Mega              | <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/ULtrasonido.PNG)</div>             | Sensor de distancia por ultrasonidos HC-SR04            |
| <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/motor.PNG)</div>              | Motor	Micro Metal 150:1            | <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/rueda_con_encoder.PNG)</div>              | Rueda antideslizande con encoder            |
| <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/rueda_loca.PNG)</div>| Rueda Loca Metálica| <div style="width: 50%; height: 50%;">![Jetson Nano](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/Im%C3%A1genes%20para%20Readme/powerbanl.PNG)</div>| Power Bank 20A |





## Circuito
Esquema global de Robofinder:
![Circuito](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/RoboFinder_esquem%C3%A1tico.png)

# Impresión 3d


# Requisitos de Software


# Instalación

# Uso

# Resultado Final


# Demo Video
Presiona para visualizar el video en youtube!
[![Demo del proyecto](https://github.com/ArnauMarcosAlmansa/Robofinder/blob/master/ROBOT%20DESIGN.png)](https://www.youtube.com/)



# Agradecimientos
<p align="justify">
Queremos destacar y agradecer especialmente a la UAB, junto a los profesores de esta asignatura por proporcionarnos los mejores componentes posibles y por el apoyo brindado semanalmente.
</p>

[Universitat Autònoma de Barcelona (UAB)](https://www.uab.cat/web/universitat-autonoma-de-barcelona-1345467954409.html)

<p align="justify">
También al equipo del Open lab por facilitarnos la impresión 3D y todas las herramientas que nos han permitido llevar a cabo nuestro robot.
</p>

[Open Labs UAB](https://www.uab.cat/open-labs/)

# Autores
- [Pere Pérez López](https://github.com/Pachi23)

- [Lluís Alonso Pichardo](https://github.com/lluisalosnsop)

- [Arnau Marcos Almansa](https://github.com/ArnauMarcosAlmansa)

- [Sergi Sanz Orellana](https://github.com/sergisanzorellana)

