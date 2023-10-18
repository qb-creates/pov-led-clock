# Persistence of Vision (POV) Clock
Created a fully functional analog style pov(persistence of vision) clock. Persistence of vision displays are generally LED displays which 'shows' images by displaying a section of an image at a given time, in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by attaching my microcontroller board and LEDs to the motor of a hard drive. A hard drive motor was used by it is a three phase motor and it is also spins fast enough to make the image look complete and not have any stutters. An IR remote was used to set the time on the clock upon power up. A PWM signal made from a 555 timer was used to drive a speed controller which controls the speed of our motor. This project was designed, built, and programmed by me.


<!-- 
 This program will project the image of an anolog clock using the idea of POV (Persistence of Vision).
 The circuit is mounted to a 3 phase brushless motor and will project the image of the clock everytime 
 the motor completes a full rotation. An IR Detector connected to External Interrupt 0 is used to notify 
 the circuit that a full rotation of the motor has occured. This will let the circuit know that it is back 
 at its home position. A 2 second CTC timer is used to add one second to the clock on each CTC compare 
 interrupt. This interrupt will be triggered every 1 second. Two 16 channel LED drivers are used to drive
 the LEDs. Communication with the LED drivers is done using the SPI protocol. -->

- Project Video (YouTube): https://www.youtube.com/watch?v=ZK3UpSKuKRA
 <div align="center">
    <!-- <img src="images/pov-led-clock-powered.png" width = "446" height = "428"> -->
    <img src="images/pov-led-clock-powered.png" width = "335" height = "321">
 </div>

<h2> Table of Contents</h2>

- [Overview](#overview)
- [Software](#software)
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
- [Parts List](#partslist)


## Overview <a name="overview"></a>

## Software <a name="software"></a>
VSCode with platform.io extension
AVRDude to deploy
KiCad design pcb
AutoDesk fusion for the case

## Parts List <a name="partslist"></a>

# Clock Circuit <a name="clockcircuit"></a>
## Description
<div>
    <img src = "images/led-circuit-schematic.jpg">
</div>

## LED Driver
## IR Receiver
## Notes

## ESC Driver Circuit<a name="escdriver"></a>
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

<h2>Images</h2>
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
</div>