A propeller display that is made up of a single row of LEDs and projects an image of an analog clock. The display utilizes an ATmega32 
microcontroller two 16-channel LED drivers, and a brushless DC motor.

- Project Video (YouTube): https://www.youtube.com/watch?v=ZK3UpSKuKRA

# Persistence of Vision (POV) Clock
 <div align="center">
    <!-- <img src="images/pov-led-clock-powered.png" width = "446" height = "428"> -->
    <img src="images/pov-led-clock-powered.png" width = "335" height = "321">
 </div>

<h2> Table of Contents</h2>

- [Overview](#overview)
- [Software Used](#software)
- [Parts List](#partslist)
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
Persistence of vision displays are generally LED displays which 'shows' images by displaying a section of an image at a given time, in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by mounting my circuit board to brushless DC motor. A motor with high rpm was chosen so that it will spin fast enough to make the image look complete and not have any stutters. This project was designed, built, and programmed by me.

## Software Used<a name="software"></a>
- <a href="https://docs.platformio.org/en/latest/what-is-platformio.html">VSCode with PlatformIO extension</a>
- AVRDude: https://github.com/avrdudes/avrdude
- KiCad: https://www.kicad.org/
- AutoDesk Fusion 360: https://www.autodesk.com/products/fusion-360/personal

## Parts List <a name="partslist"></a>
|Part |Quantity|Link|
|-----|--------|----|
|Motor|--------|----|
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