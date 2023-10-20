# Persistence of Vision (POV) Clock

A propeller display that is made up of a single row of LEDs and projects an image of an analog clock. The display utilizes an ATmega32 
microcontroller two 16-channel LED drivers, and a brushless DC motor.

- Project Video (YouTube): https://www.youtube.com/watch?v=ZK3UpSKuKRA
- Persistance of Vision: https://en.wikipedia.org/wiki/Persistence_of_vision

 <div align="center">
    <!-- <img src="images/pov-led-clock-powered.png" width = "446" height = "428"> -->
    <img src="images/pov-led-clock-powered.png" width = "335" height = "321">
 </div>

<h2> Table of Contents</h2>

1. [Overview](#overview)
2. [Software Used](#software)
3. [Clock Circuit](#clockcircuit)
    - [Description](#clockdescription) <!-- Description of the circuit. Schematic. Double sided board for space-->
    - [LED Driver](#leddriver) <!-- Description of how data is transfered to led sinks -->
    - [IR Receiver](#irreceiver) <!-- External interrupt-->
    - [Parts List](#clockpartslist) <!--Leaving out crystal because of balance. Modifying isp header  If you use crystal you will need to update FCPU and timer OCR-->
4. [ESC Driver Circuit](#escdriver)
    - [Description](#escdescription) <!-- Description of the circuit. Schematic image.-->
    - [PWM](#escpwm)
    - [Parts List](#escpartslist) <!--Choose a crystal where you can get pwm frequency between 50 - 500hz. Maybe include a small capacitor for button debouncing-->
5. [Hardware](#hardware)
6. [Power Supply](#powersupply)


## Overview <a name="overview"></a>
Persistence of vision displays are LED displays which project images by displaying a section of an image at a given time in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by mounting my circuit board to brushless DC motor. A motor with high rpm was chosen so that it will spin fast enough to make the image look complete and not have any stutters. This project was designed, built, and programmed by me.


## Software Used<a name="software"></a>
- VSCode with PlatformIO extension: https://docs.platformio.org/en/latest/what-is-platformio.html
- VRDude (Flash Uploader): https://github.com/avrdudes/avrdude
- KiCad (PCB Design): https://www.kicad.org
- AutoDesk Fusion 360 Personal (Case Design): https://www.autodesk.com/products/fusion-360/personal


## Clock Circuit <a name="clockcircuit"></a>
### Description <a name="clockdescription"></a>
 This circuit will project the image of an anolog clock using the idea of POV (Persistence of Vision).
 The circuit is mounted to a brushless motor and will project the image of the clock everytime the
 motor completes a full rotation. An IR Detector connected to External Interrupt 0 is used to notify 
 the circuit that a full rotation of the motor has occured. This will let the circuit know that it is back 
 at its home position. A 2 second CTC timer is used to add one second to the clock on each CTC compare 
 interrupt. This interrupt will be triggered every 1 second. Two 16 channel LED drivers are used to drive
 the LEDs. Communication with the LED drivers is done using the SPI protocol.
<div>
    <img src = "images/led-circuit-schematic.jpg">
</div>

### LED Driver <a name="leddriver"></a>
### IR Receiver <a name="irreceiver"></a>
### Parts List <a name="clockcircuitnotes"></a>
|Part Number|Quantity|
|-----|:--------:|
|<a href="https://www.mouser.com/ProductDetail/Microchip-Technology/ATMEGA32A-AU?qs=6WoM%2FY8vMbbFYDtovr%252BQ5Q%3D%3D">ATmega32A-AU</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">ISP Header</a>| x1 |
|<a href="https://www.digikey.com/en/products/detail/jst-sales-america-inc/S2B-PH-SM4-TB/926655?s=N4IgTCBcDaICwFYEFoCMB2OBONyByAIiALoC%2BQA">JST Connector</a>| x1 |
|<a href="https://www.digikey.com/en/products/detail/stmicroelectronics/STP16CPC05MTR/2021162">STP16CPC05MTR LED Driver</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/led-indication-discrete/105">SMD Led 1205 (3216 Metric)</a>| x32 |
|<a href="https://www.digikey.com/en/products/filter/oscillators/172">16Mhz Crystal</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">22pF Capacitor</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">100nF Capacitor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">10K Resistor</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">7.32K Resistor</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/diodes/rectifiers/single-diodes/280">1n4148 Diode</a>| x1 |
|<a href="https://www.sparkfun.com/products/19018">Infrared Detector</a>| x1 |


## ESC Driver Circuit<a name="escdriver"></a>
### Description<a name="escdescription"></a>
This circuit produces a PWM output that is used to drive an ESC (Electronic Speed Controller).
A push button connected to PINA7 is used to adjust the duty cycle of the PWM signal.  
This is done to start and stop the motor. Button debouncing is accomplished in software
by using a 300ms delay after detecting the first pulse before checking again. 
<div>
    <img src = "images/esc-driver-circuit-schematic.JPG">
</div>

### PWM<a name="escpwm"></a>

### Parts List<a name="escpartslist"></a>
|Part Number|Quantity|
|-----|:--------:|
|<a href="https://www.digikey.com/en/products/detail/microchip-technology/ATTINY861-20PU/1245922">ATtiny861-20PU</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">22pF Capacitor</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">100nF Capacitor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/oscillators/172">16Mhz Crystal</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">10K resistor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">330 resistor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/diodes/rectifiers/single-diodes/280">1n4148 Diode</a>| x1 |
|<a href="https://www.sparkfun.com/products/18772">Infrared Emitter</a>| x1 ||
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">ISP Header</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">Push Button</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">Pcb Board 4cm x 6cm</a>| x1 |

## Hardware <a name="hardware"></a>
|Part Number|Quantity|
|-----|:--------:|
|<a href="https://www.amazon.com/gp/product/B08CRTR7CZ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1">Brushless Motor</a>| x1 |
|<a href="https://www.amazon.com/gp/product/B071GRSFBD/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1">ESC 3A UBEC</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">IR Transmitter</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">Barrel Jack</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">9V 1A Power Supply</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">5V 200mA Wireless power </a>| x1 |

## Power Supply <a name="powersupply"></a>





















<!-- power supply
wireless power -->

<!-- <h2>Images</h2>
<div>
    <img src = "images/pov-led-clock.JPEG" width = "410" height = "500" style="padding: 0; margin: 0;">
    <img src = "images/case-inside.JPEG" width = "410" height = "500">
</div>
<div>
    <img src = "images/led-circuit-front.JPEG" width = "203" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/led-circuit-back.JPEG" width = "203" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/attiny-circuit-front.JPEG" width = "203" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/attiny-circuit-back.JPEG" width = "203" height = "270" style="padding: 0; margin: 0;">
</div>

<h2>LED Clock Schematic</h2>
<div>
    <img src = "images/led-circuit-schematic.jpg">
</div>

<h2>ESC Driver and Motor Schematic</h2>
<div>
    <img src = "images/esc-driver-circuit-schematic.JPG">
</div> -->