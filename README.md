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

- [Overview](#overview)
- [Software Used](#software)
- [Parts List](#partslist)
    - [ESC Driver](#partsescdriver) <!-- Description of the circuit. Schematic. Double sided board for space-->
- [Clock Circuit](#clockcircuit)
    - [Description](#clockdescription) <!-- Description of the circuit. Schematic. Double sided board for space-->
    - [LED Driver](#leddriver) <!-- Description of how data is transfered to led sinks -->
    - [IR Receiver](#irreceiver) <!-- External interrupt-->
    - [Notes](#clockcircuitnotes) <!--Leaving out crystal because of balance. Modifying isp header  If you use crystal you will need to update FCPU and timer OCR-->
- [ESC Driver Circuit](#escdriver)
    - [Description](#escdescription) <!-- Description of the circuit. Schematic image.-->
    - [LED Driver](#leddriver) <!-- Description of how data is transfered to led sinks -->
    - [IR Receiver](#irreceiver) <!-- External interrupt-->
    - [Notes](#esccircuitnotes) <!--Choose a crystal where you can get pwm frequency between 50 - 500hz. Maybe include a small capacitor for button debouncing-->
- [Hardware](#hardware)
- [Power Supply](#powersupply)


## Overview <a name="overview"></a>
Persistence of vision displays are LED displays which project images by displaying a section of an image at a given time in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by mounting my circuit board to brushless DC motor. A motor with high rpm was chosen so that it will spin fast enough to make the image look complete and not have any stutters. This project was designed, built, and programmed by me.

## Software Used<a name="software"></a>
- <a href="https://docs.platformio.org/en/latest/what-is-platformio.html">VSCode with PlatformIO extension</a>
- <a href="https://github.com/avrdudes/avrdude">VRDude (Flash Uploader)</a> 
- <a href="https://www.kicad.org/">KiCad (PCB Design)</a> 
- <a href="https://www.autodesk.com/products/fusion-360/personal">AutoDesk Fusion 360 Personal (Case Design)</a> 

## Parts List <a name="partslist"></a>
### ESC Driver <a name="partsescdriver"></a>
|Part Number|Quantity|Notes|
|-----|--------|----|
|<a style="color: black; text-decoration: underline;text-decoration-style: dotted;" href="https://www.amazon.com/gp/product/B08CRTR7CZ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1">Brushless Motor</a>|   x1   |sdfsdf|
|<a href="https://www.amazon.com/gp/product/B071GRSFBD/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1">ESC 3A UBEC</a>|   x1   |sdfsdf|
|<a href="https://www.digikey.com/en/products/detail/microchip-technology/ATTINY861-20PU/1245922">ATtiny861-20PU-ND</a>|   x1   |sdfsdfs|
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">22pF Ceramic Capacitor</a>|   x2   |sdfsdf|
|<a href="https://www.digikey.com/en/products/filter/oscillators/172">16Mhz Crystal</a>|   x1   |sdfsdf|
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">10K ohm resistor</a>|   x1   |sdfsdf|
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">330 ohm resistor</a>|   x1   |asdfsdf|
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">ISP Header</a>|   x1   |sdfsdf|


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
### Notes <a name="clockcircuitnotes"></a>

## ESC Driver Circuit<a name="escdriver"></a>
This circuit produces a PWM output that is used to drive an ESC (Electronic Speed Controller).
A push button connected to PINA7 is used to adjust the duty cycle of the PWM signal.  
This is done to start and stop the motor. Button debouncing is accomplished in software
by using a 300ms delay after detecting the first pulse before checking again. 
<div>
    <img src = "images/esc-driver-circuit-schematic.JPG">
</div>
<!-- motor info
esc
case -->

## Hardware <a name="hardware"></a>
<!-- motor info
esc
case
ir transmitter -->

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