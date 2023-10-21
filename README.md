# Persistence of Vision (POV) Clock

A propeller display that is made up of a single row of LEDs and projects an image of an analog clock. The display utilizes an ATmega32 
microcontroller two 16-channel LED drivers, and a brushless DC motor.

- Project Video (YouTube): https://www.youtube.com/watch?v=ZK3UpSKuKRA
- Persistance of Vision: https://en.wikipedia.org/wiki/Persistence_of_vision

 <div align="center">
     <!--  <img src="images/pov-led-clock-powered.png" width = "446" height = "428">  -->
    <!-- <img src="images/pov-led-clock-powered.png" width = "335" height = "321"> -->
    <img src="images/pov-led-clock-powered.png" width = "400" height = "384">
 </div>

<h2> Table of Contents</h2>

1. [Overview](#overview)
2. [Software Used](#software)
3. [ESC Driver Circuit](#escdriver)
    - [PWM Overview](#escpwm)
    - [Parts List](#escpartslist) <!--Choose a crystal where you can get pwm frequency between 50 - 500hz. Maybe include a small capacitor for button debouncing-->
4. [Clock Circuit](#clockcircuit)
    - [LED Driver](#leddriver) <!-- Description of how data is transfered to led sinks -->
    - [Timer Overview](#clocktimer) <!-- External interrupt-->
    - [Parts List](#clockpartslist) <!--Leaving out crystal because of balance. Modifying isp header  If you use crystal you will need to update FCPU and timer OCR-->
5. [Hardware](#hardware)
6. [Power Supply](#powersupply)
7. [Schematics](#schematics)
    - [ESC Driver Schematic](#clockschematic)
    - [Clock Schematic](#clockschematic)


## 1. Overview <a name="overview"></a>
Persistence of vision displays are LED displays which project images by displaying a section of an image at a given time in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by mounting my circuit board to brushless DC motor. A motor with high rpm was chosen so that it will spin fast enough to make the image look complete and not have any stutters. This project was designed, built, and programmed by me.


## 2. Software Used<a name="software"></a>
- VSCode with PlatformIO extension: https://docs.platformio.org/en/latest/what-is-platformio.html
- VRDude (Flash Uploader): https://github.com/avrdudes/avrdude
- KiCad (PCB Design): https://www.kicad.org
- AutoDesk Fusion 360 Personal (Case Design): https://www.autodesk.com/products/fusion-360/personal

## 3. ESC Driver Circuit<a name="escdriver"></a>
The ATtiny861 produces a PWM output that is used to drive the ESC (Electronic Speed Controller). A push button connected to PINA7 is used to adjust the duty cycle of the PWM signal.
By adjusting the duty cycle of the PWM signal to a value where the time high is less than 1ms we can stop the motor. We can press the button once to start the motor, then press it
again to stop it. Button debouncing is accomplished in software by using a 300ms delay after detecting the first pulse before checking again. The IR transmitter is used to notify
the clock circuit that it has made it rotated back to it's starting position. The inductive transmitter will be mounted to the case while the receiver is mounted to the motor and connected
to the clock circuit. The ESC's UBEC will supply the ATtiny, IR transmitter, and inductive transmitter with power (5V 3A).
<div align="center">
 <div>
    <img src = "images/attiny-circuit-front.JPEG" width = "206" height = "275" style="padding: 0; margin: 0;">
    <img src = "images/attiny-circuit-back.JPEG" width = "206" height = "275" style="padding: 0; margin: 0;">
    <img src = "images/case-inside2.jpg" width = "357" height = "275">
 </div>
</div>

### PWM Overview<a name="escpwm"></a>
I wanted the PWM frequency to be as close to 500Hz as possible. The time high for a 100% duty cycle would be 2ms. Usually a 1ms pulse translates to 0% throttle and a 2ms pulse translates to 100% throttle. 
The duty cycle of the PWM signal can be adjusted by setting Timer1's OCR1B register to a value between 0 and 255. I was able to use every value between 128 and 255 for OCR1B without exceeding 2ms. 
This gave me more wiggle when setting the speed of the motor instead of going from 0% throttle to 100% immediately. 

PWM Frequency Equation for ATtiny861
- PWM Frequency = fck / (prescaler * 255)
   
Variables
- fck = 16 Mhz
- Target PWM Frequency = 500 Hz
- prescaler = ?
  
Solve for prescaler
- prescaler = fck / (Target PWM Frequency * 255)
- prescaler = 16 Mhz / (500 Hz * 255)
- prescaler = 125.5 round to 128

Solve for Actual PWM Frequency
- Actual PWM Frequency = 16 Mhz / (128 * 255)
- Actual PWM Frequency = 490 Hz
---
### Parts List<a name="escpartslist"></a>
|Part Number|Quantity|
|-----|:--------:|
|<a href="https://www.digikey.com/en/products/detail/microchip-technology/ATTINY861-20PU/1245922">ATtiny861-20PU</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">ISP Header</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">22pF Capacitor</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">100nF Capacitor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/oscillators/172">16Mhz Crystal</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">10K resistor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">330 resistor</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/diodes/rectifiers/single-diodes/280">1n4148 Diode</a>| x1 |
|<a href="https://www.digikey.com/en/products/detail/e-switch/RP3502ABLK/280446?s=N4IgTCBcDaIKIHECMBOADGgtAOQCIgF0BfIA">Push Button</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">Pcb Board 4cm x 6cm</a>| x1 |
|<a href="https://www.amazon.com/gp/product/B08CRTR7CZ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1">Brushless Motor</a>| x1 |
|<a href="https://www.amazon.com/gp/product/B09H5L3KN5/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1">Barrel Jack</a>| x1 |
|<a href="https://www.sparkfun.com/products/18772">Infrared Emitter</a>| x1 ||
|<a href="https://www.amazon.com/gp/product/B071GRSFBD/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1">ESC 3A UBEC</a>| x1 |
|<a href="https://www.amazon.com/Adapter-Regulated-Switching-Interchangeable-Equipment/dp/B0BFPXZ7S1/ref=sr_1_3_sspa?crid=396GU23EXCZA8&keywords=9v+power+supply&qid=1697854150&sprefix=9v+power%2Caps%2C93&sr=8-3-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1">9V 1A Power Supply</a>| x1 |
|<a href="https://www.adafruit.com/product/1407">5V 200mA Inductive Charging Set </a>| x1 |

## 4. Clock Circuit <a name="clockcircuit"></a>
 This circuit will project the image of an anolog clock using the idea of POV (Persistence of Vision).
 The circuit is mounted to a brushless motor and will project the image of the clock everytime the
 motor completes a full rotation. An IR Detector connected to External Interrupt 0 is used to notify 
 the circuit that a full rotation of the motor has occured. This will let the circuit know that it is back 
 at its home position. A 2 second CTC timer is used to add one second to the clock on each CTC compare 
 interrupt. This interrupt will be triggered every 1 second. Two 16 channel LED drivers are used to drive
 the LEDs. Communication with the LED drivers is done using the SPI protocol.
<div align="center">
    <img src = "images/led-circuit-front.JPEG" width = "206" height = "275" style="padding: 0; margin: 0;">
    <img src = "images/led-circuit-back.JPEG" width = "206" height = "275" style="padding: 0; margin: 0;">
    <img src = "images/pov-led-clock.JPEG" width = "357" height = "275" style="padding: 0; margin: 0;">
</div>

### LED Drivers <a name="leddriver"></a>
The LED Drivers are chained together. Each driver must receive two bytes of data to control their 16 output channels. Four bytes of data must be sent to the first driver in order to control all 32 output channels. The LEDs are mounted in order starting from the center of the PCB with LED 1 and ending at the opposite end with LED 32. The table below shows which output channel each LED is connected to. 

|_**STP16CPC05 1 Outputs**_|LED Number|--|_**STP16CPC05 2 Outputs**_|LED Number|
|:--:|:--:|:--:|:--:|:--:|
|**Byte0**||--|**Byte2**||
|Out0|LED1|--|Out0|LED17|
|Out1|LED2|--|Out1|LED18|
|Out2|LED3|--|Out2|LED19|
|Out3|LED4|--|Out3|LED20|
|Out4|LED5|--|Out4|LED21|
|Out5|LED6|--|Out5|LED22|
|Out6|LED7|--|Out6|LED23|
|Out7|LED8|--|Out7|LED24|
|||--|||
|**Byte1**||--|**Byte3**||
|Out8|LED16|--|Out8|LED32|
|Out9|LED15|--|Out9|LED31|
|Out10|LED14|--|Out10|LED30|
|Out11|LED13|--|Out11|LED29|
|Out12|LED12|--|Out12|LED28|
|Out13|LED11|--|Out13|LED27|
|Out14|LED10|--|Out14|LED26|
|Out15|LED9|--|Out15|LED25|

Because the LED Drivers are daisy chained together, the data must be transmitted in the following order.
* Byte3 (MSB transmitted first) 
* Byte2 (LSB transmitted first) 
* Byte1 (MSB transmitted first) 
* Byte0 (LSB transmitted first) 
---
### Timer Overview <a name="clocktimer"></a>
A one second timer is configured to update the seconds hand of the clock. The timer is configured to operate in Clear Timer on Compare Match Mode (CTC). A CTC frequency of 0.5 Hz (2s) is required. 
This will trigger the compare match interrupt every second.Because the weight of the crystal caused the ciruit to not be completely balanced, it was discarded and the ATmega32's internal 8 Mhz 
oscillator was used instead. Calculations to find the appropriate OCRn values are listed below.

CTC Waveform Frequency Equation
- Frequency = fck / (2 * prescaler * (1 + OCRnA)).

Variables
- Frequency = 0.5Hz = 2 seconds
- fck = 8 Mhz
- prescaler = 256
- OCR1A = ?

Rearranged formula to solve for OCRnA:
- OCR1A = (fck / (2 * prescaler * Frequency)) - 1

Solve for OCRnA
- OCR1A = (8 Mhz / (2 * 256 * 0.5Hz)) - 1 
- OCR1A = 31249
---
### Parts List <a name="clockpartslist"></a>
|Part Number|Quantity|
|-----|:--------:|
|<a href="https://www.mouser.com/ProductDetail/Microchip-Technology/ATMEGA32A-AU?qs=6WoM%2FY8vMbbFYDtovr%252BQ5Q%3D%3D">ATmega32A-AU</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/rectangular-connectors/headers-male-pins/314">ISP Header</a>| x1 |
|<a href="https://www.digikey.com/en/products/detail/jst-sales-america-inc/S2B-PH-SM4-TB/926655?s=N4IgTCBcDaICwFYEFoCMB2OBONyByAIiALoC%2BQA">JST Connector</a>| x1 |
|<a href="https://www.digikey.com/en/products/detail/stmicroelectronics/STP16CPC05MTR/2021162">STP16CPC05MTR LED Driver</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/led-indication-discrete/105">SMD Led 1205 (3216 Metric)</a>| x32 |
|<a href="https://www.digikey.com/en/products/filter/oscillators/172">16Mhz Crystal</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">22pF Capacitor 0603 (1608 Metric)</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/ceramic-capacitors/60">100nF Capacitor 0603 (1608 Metric)</a>| x1 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">10K Resistor 0603 (1608 Metric)</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/through-hole-resistors/53">7.32K Resistor 0603 (1608 Metric)</a>| x2 |
|<a href="https://www.digikey.com/en/products/filter/diodes/rectifiers/single-diodes/280">1n4148 Diode</a>| x1 |
|<a href="https://www.sparkfun.com/products/19018">Infrared Detector</a>| x1 |

## 5. Hardware <a name="hardware"></a>
```stl
solid ASCII
  facet normal 1.000000e+00 5.072022e-09 0.000000e+00
    outer loop
      vertex   -3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -3.000000e+00 -1.418626e+01 0.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 5.072022e-09 0.000000e+00
    outer loop
      vertex   -3.000000e+00 -1.418626e+01 0.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -2.201450e-16
    outer loop
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -2.051425e-16
    outer loop
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -2.051425e-16
    outer loop
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -1.761598e-16
    outer loop
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 2.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 0.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -1.351722e-16
    outer loop
      vertex   -1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -8.497279e-17
    outer loop
      vertex   -1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 0.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -1.487024e-16
    outer loop
      vertex   -7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -5.071961e-17
    outer loop
      vertex   -7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
      vertex   7.764571e-01 -2.589778e+01 0.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 4.347396e-17
    outer loop
      vertex   7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
      vertex   7.764571e-01 -2.589778e+01 2.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 1.274592e-16
    outer loop
      vertex   7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   1.500000e+00 -2.559808e+01 0.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 2.549184e-16
    outer loop
      vertex   1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 4.055166e-16
    outer loop
      vertex   1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   2.121320e+00 -2.512132e+01 0.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 -9.052177e-16
    outer loop
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 -4.746399e-16
    outer loop
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 5.072022e-09 0.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 5.072022e-09 -9.009722e-24
    outer loop
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   3.000000e+00 -1.418626e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   4.672880e+00 -1.372640e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   6.278861e+00 -1.307004e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
      vertex   1.435023e+01 -2.916026e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   1.435023e+01 -2.916026e+01 2.000000e+00
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
      vertex   2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
      vertex   2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
      vertex   2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
      vertex   1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   7.764571e-01 -2.589778e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
      vertex   7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 2.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 2.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 2.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
      vertex   -2.121320e+00 -2.512132e+01 2.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 2.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.317957e+01 -2.970772e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 -2.724506e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.365606e+01 -2.228544e+01 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.825291e+01 -1.606310e+01 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 -8.930603e+00 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 -1.292087e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
      vertex   -3.185966e+01 6.419640e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.185966e+01 6.419640e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
      vertex   -2.943981e+01 1.376762e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.943981e+01 1.376762e+01 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
      vertex   -2.535186e+01 2.033551e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.535186e+01 2.033551e+01 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
      vertex   -1.982744e+01 2.575117e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.982744e+01 2.575117e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
      vertex   -1.317957e+01 2.970772e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.317957e+01 2.970772e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
      vertex   -5.784932e+00 3.198100e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 3.198100e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
      vertex   1.937492e+00 3.244220e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 3.244220e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
      vertex   9.550134e+00 3.106517e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 3.106517e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
      vertex   1.662165e+01 2.792795e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.662165e+01 2.792795e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
      vertex   2.275137e+01 2.320830e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.275137e+01 2.320830e+01 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
      vertex   2.759196e+01 1.717364e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.759196e+01 1.717364e+01 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
      vertex   3.086915e+01 1.016589e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 1.016589e+01 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
      vertex   3.239726e+01 2.582130e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.239726e+01 2.582130e+00 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   3.208970e+01 -5.147936e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.208970e+01 -5.147936e+00 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   2.996389e+01 -1.258631e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.996389e+01 -1.258631e+01 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   2.614029e+01 -1.931153e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.614029e+01 -1.931153e+01 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   2.083554e+01 -2.494254e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.083554e+01 -2.494254e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 2.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   -8.106506e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   -8.066777e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   -8.015000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   -7.954705e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   -7.825295e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   -7.765000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   -7.713223e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   -7.673494e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   -7.640000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   -7.673494e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   -7.713223e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   -7.765000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   -7.890000e+01 -7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   -7.954705e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   -8.015000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   -8.066777e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   -8.106506e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 1.000000e+01
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 1.000000e+01
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
      vertex   -8.131481e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 1.000000e+01
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 1.000000e+01
      vertex   -8.106506e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 1.000000e+01
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 1.000000e+01
      vertex   -8.066777e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 1.000000e+01
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 1.000000e+01
      vertex   -8.015000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 1.000000e+01
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 1.000000e+01
      vertex   -7.954705e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 1.000000e+01
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 1.000000e+01
      vertex   -7.890000e+01 7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 1.000000e+01
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 1.000000e+01
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
      vertex   -7.765000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 1.000000e+01
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 1.000000e+01
      vertex   -7.713223e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 1.000000e+01
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 1.000000e+01
      vertex   -7.673494e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 1.000000e+01
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 1.000000e+01
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
      vertex   -7.640000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 1.000000e+01
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 1.000000e+01
      vertex   -7.648519e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 1.000000e+01
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 1.000000e+01
      vertex   -7.673494e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 1.000000e+01
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 1.000000e+01
      vertex   -7.713223e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 1.000000e+01
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 1.000000e+01
      vertex   -7.765000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 1.000000e+01
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 1.000000e+01
      vertex   -7.825295e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 1.000000e+01
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 1.000000e+01
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
      vertex   -7.954705e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 1.000000e+01
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 1.000000e+01
      vertex   -8.015000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 1.000000e+01
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 1.000000e+01
      vertex   -8.066777e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 1.000000e+01
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 1.000000e+01
      vertex   -8.106506e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 1.000000e+01
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 1.000000e+01
      vertex   -8.131481e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   7.640000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   7.648519e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   7.673494e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   7.713223e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   7.765000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   7.825295e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   7.954705e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   8.015000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   8.066777e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   8.106506e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   8.131481e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   8.131481e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   8.106506e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   8.066777e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   8.015000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   7.954705e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   7.890000e+01 -7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   7.765000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   7.713223e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   7.673494e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 1.000000e+01
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 1.000000e+01
      vertex   7.640000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 1.000000e+01
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 1.000000e+01
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
      vertex   7.673494e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 1.000000e+01
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 1.000000e+01
      vertex   7.713223e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 1.000000e+01
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 1.000000e+01
      vertex   7.765000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 1.000000e+01
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 1.000000e+01
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
      vertex   7.890000e+01 7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 1.000000e+01
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 1.000000e+01
      vertex   7.954705e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 1.000000e+01
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 1.000000e+01
      vertex   8.015000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 1.000000e+01
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 1.000000e+01
      vertex   8.066777e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 1.000000e+01
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 1.000000e+01
      vertex   8.106506e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 1.000000e+01
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 1.000000e+01
      vertex   8.131481e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 1.000000e+01
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 1.000000e+01
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
      vertex   8.131481e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 1.000000e+01
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 1.000000e+01
      vertex   8.106506e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 1.000000e+01
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 1.000000e+01
      vertex   8.066777e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 1.000000e+01
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 1.000000e+01
      vertex   8.015000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 1.000000e+01
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 1.000000e+01
      vertex   7.954705e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 1.000000e+01
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 1.000000e+01
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
      vertex   7.825295e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 1.000000e+01
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 1.000000e+01
      vertex   7.765000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 1.000000e+01
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 1.000000e+01
      vertex   7.713223e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 1.000000e+01
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 1.000000e+01
      vertex   7.673494e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 1.000000e+01
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 1.000000e+01
      vertex   7.648519e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -9.992094e-01 3.975652e-02 0.000000e+00
    outer loop
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
      vertex   3.239726e+01 -2.582130e+00 4.000000e+00
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
    endloop
  endfacet
  facet normal -9.992094e-01 3.975652e-02 0.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
      vertex   3.239726e+01 -2.582130e+00 4.000000e+00
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
    endloop
  endfacet
  facet normal -9.992094e-01 -3.975652e-02 0.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 2.000000e+00
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
      vertex   3.239726e+01 2.582130e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.992094e-01 -3.975652e-02 0.000000e+00
    outer loop
      vertex   3.239726e+01 2.582130e+00 2.000000e+00
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
      vertex   3.239726e+01 2.582130e+00 4.000000e+00
    endloop
  endfacet
  facet normal -9.928921e-01 -1.190182e-01 0.000000e+00
    outer loop
      vertex   3.239726e+01 2.582130e+00 2.000000e+00
      vertex   3.239726e+01 2.582130e+00 4.000000e+00
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.928921e-01 -1.190182e-01 0.000000e+00
    outer loop
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
      vertex   3.239726e+01 2.582130e+00 4.000000e+00
      vertex   3.208970e+01 5.147936e+00 4.000000e+00
    endloop
  endfacet
  facet normal -9.802974e-01 -1.975274e-01 0.000000e+00
    outer loop
      vertex   3.208970e+01 5.147936e+00 2.000000e+00
      vertex   3.208970e+01 5.147936e+00 4.000000e+00
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.802974e-01 -1.975274e-01 0.000000e+00
    outer loop
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
      vertex   3.208970e+01 5.147936e+00 4.000000e+00
      vertex   3.157925e+01 7.681194e+00 4.000000e+00
    endloop
  endfacet
  facet normal -9.615049e-01 -2.747878e-01 0.000000e+00
    outer loop
      vertex   3.157925e+01 7.681194e+00 2.000000e+00
      vertex   3.157925e+01 7.681194e+00 4.000000e+00
      vertex   3.086915e+01 1.016589e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.615049e-01 -2.747878e-01 0.000000e+00
    outer loop
      vertex   3.086915e+01 1.016589e+01 2.000000e+00
      vertex   3.157925e+01 7.681194e+00 4.000000e+00
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.366335e-01 -3.503108e-01 0.000000e+00
    outer loop
      vertex   3.086915e+01 1.016589e+01 2.000000e+00
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.366335e-01 -3.503108e-01 0.000000e+00
    outer loop
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
      vertex   2.996389e+01 1.258631e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.058404e-01 -4.236191e-01 0.000000e+00
    outer loop
      vertex   2.996389e+01 1.258631e+01 2.000000e+00
      vertex   2.996389e+01 1.258631e+01 4.000000e+00
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.058404e-01 -4.236191e-01 0.000000e+00
    outer loop
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
      vertex   2.996389e+01 1.258631e+01 4.000000e+00
      vertex   2.886918e+01 1.492716e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.693203e-01 -4.942492e-01 0.000000e+00
    outer loop
      vertex   2.886918e+01 1.492716e+01 2.000000e+00
      vertex   2.886918e+01 1.492716e+01 4.000000e+00
      vertex   2.759196e+01 1.717364e+01 2.000000e+00
    endloop
  endfacet
  facet normal -8.693203e-01 -4.942492e-01 0.000000e+00
    outer loop
      vertex   2.759196e+01 1.717364e+01 2.000000e+00
      vertex   2.886918e+01 1.492716e+01 4.000000e+00
      vertex   2.759196e+01 1.717364e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.273040e-01 -5.617544e-01 0.000000e+00
    outer loop
      vertex   2.759196e+01 1.717364e+01 2.000000e+00
      vertex   2.759196e+01 1.717364e+01 4.000000e+00
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
    endloop
  endfacet
  facet normal -8.273040e-01 -5.617544e-01 0.000000e+00
    outer loop
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
      vertex   2.759196e+01 1.717364e+01 4.000000e+00
      vertex   2.614029e+01 1.931153e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.800573e-01 -6.257081e-01 0.000000e+00
    outer loop
      vertex   2.614029e+01 1.931153e+01 2.000000e+00
      vertex   2.614029e+01 1.931153e+01 4.000000e+00
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.800573e-01 -6.257081e-01 0.000000e+00
    outer loop
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
      vertex   2.614029e+01 1.931153e+01 4.000000e+00
      vertex   2.452335e+01 2.132734e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.278788e-01 -6.857058e-01 0.000000e+00
    outer loop
      vertex   2.452335e+01 2.132734e+01 2.000000e+00
      vertex   2.452335e+01 2.132734e+01 4.000000e+00
      vertex   2.275137e+01 2.320830e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.278788e-01 -6.857058e-01 0.000000e+00
    outer loop
      vertex   2.275137e+01 2.320830e+01 2.000000e+00
      vertex   2.452335e+01 2.132734e+01 4.000000e+00
      vertex   2.275137e+01 2.320830e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.710984e-01 -7.413683e-01 0.000000e+00
    outer loop
      vertex   2.275137e+01 2.320830e+01 2.000000e+00
      vertex   2.275137e+01 2.320830e+01 4.000000e+00
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.710984e-01 -7.413683e-01 0.000000e+00
    outer loop
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
      vertex   2.275137e+01 2.320830e+01 4.000000e+00
      vertex   2.083554e+01 2.494254e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.100751e-01 -7.923436e-01 0.000000e+00
    outer loop
      vertex   2.083554e+01 2.494254e+01 2.000000e+00
      vertex   2.083554e+01 2.494254e+01 4.000000e+00
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.100751e-01 -7.923436e-01 0.000000e+00
    outer loop
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
      vertex   2.083554e+01 2.494254e+01 4.000000e+00
      vertex   1.878799e+01 2.651908e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.451948e-01 -8.383094e-01 0.000000e+00
    outer loop
      vertex   1.878799e+01 2.651908e+01 2.000000e+00
      vertex   1.878799e+01 2.651908e+01 4.000000e+00
      vertex   1.662165e+01 2.792795e+01 2.000000e+00
    endloop
  endfacet
  facet normal -5.451948e-01 -8.383094e-01 0.000000e+00
    outer loop
      vertex   1.662165e+01 2.792795e+01 2.000000e+00
      vertex   1.878799e+01 2.651908e+01 4.000000e+00
      vertex   1.662165e+01 2.792795e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.768675e-01 -8.789752e-01 0.000000e+00
    outer loop
      vertex   1.662165e+01 2.792795e+01 2.000000e+00
      vertex   1.662165e+01 2.792795e+01 4.000000e+00
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.768675e-01 -8.789752e-01 0.000000e+00
    outer loop
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
      vertex   1.662165e+01 2.792795e+01 4.000000e+00
      vertex   1.435023e+01 2.916026e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.055253e-01 -9.140838e-01 0.000000e+00
    outer loop
      vertex   1.435023e+01 2.916026e+01 2.000000e+00
      vertex   1.435023e+01 2.916026e+01 4.000000e+00
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.055253e-01 -9.140838e-01 0.000000e+00
    outer loop
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
      vertex   1.435023e+01 2.916026e+01 4.000000e+00
      vertex   1.198808e+01 3.020821e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.316193e-01 -9.434133e-01 0.000000e+00
    outer loop
      vertex   1.198808e+01 3.020821e+01 2.000000e+00
      vertex   1.198808e+01 3.020821e+01 4.000000e+00
      vertex   9.550134e+00 3.106517e+01 2.000000e+00
    endloop
  endfacet
  facet normal -3.316193e-01 -9.434133e-01 0.000000e+00
    outer loop
      vertex   9.550134e+00 3.106517e+01 2.000000e+00
      vertex   1.198808e+01 3.020821e+01 4.000000e+00
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.556166e-01 -9.667782e-01 0.000000e+00
    outer loop
      vertex   9.550134e+00 3.106517e+01 2.000000e+00
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
    endloop
  endfacet
  facet normal -2.556166e-01 -9.667782e-01 0.000000e+00
    outer loop
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
      vertex   7.051811e+00 3.172573e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.779979e-01 -9.840309e-01 0.000000e+00
    outer loop
      vertex   7.051811e+00 3.172573e+01 2.000000e+00
      vertex   7.051811e+00 3.172573e+01 4.000000e+00
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.779979e-01 -9.840309e-01 0.000000e+00
    outer loop
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
      vertex   7.051811e+00 3.172573e+01 4.000000e+00
      vertex   4.508905e+00 3.218571e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.925380e-02 -9.950622e-01 0.000000e+00
    outer loop
      vertex   4.508905e+00 3.218571e+01 2.000000e+00
      vertex   4.508905e+00 3.218571e+01 4.000000e+00
      vertex   1.937492e+00 3.244220e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.925380e-02 -9.950622e-01 0.000000e+00
    outer loop
      vertex   1.937492e+00 3.244220e+01 2.000000e+00
      vertex   4.508905e+00 3.218571e+01 4.000000e+00
      vertex   1.937492e+00 3.244220e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.988219e-02 -9.998023e-01 0.000000e+00
    outer loop
      vertex   1.937492e+00 3.244220e+01 2.000000e+00
      vertex   1.937492e+00 3.244220e+01 4.000000e+00
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.988219e-02 -9.998023e-01 0.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
      vertex   1.937492e+00 3.244220e+01 4.000000e+00
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.961513e-02 -9.982214e-01 0.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 2.000000e+00
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.961513e-02 -9.982214e-01 0.000000e+00
    outer loop
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
      vertex   -3.225748e+00 3.233952e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.387355e-01 -9.903295e-01 0.000000e+00
    outer loop
      vertex   -3.225748e+00 3.233952e+01 2.000000e+00
      vertex   -3.225748e+00 3.233952e+01 4.000000e+00
      vertex   -5.784932e+00 3.198100e+01 2.000000e+00
    endloop
  endfacet
  facet normal 1.387355e-01 -9.903295e-01 0.000000e+00
    outer loop
      vertex   -5.784932e+00 3.198100e+01 2.000000e+00
      vertex   -3.225748e+00 3.233952e+01 4.000000e+00
      vertex   -5.784932e+00 3.198100e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.169788e-01 -9.761763e-01 0.000000e+00
    outer loop
      vertex   -5.784932e+00 3.198100e+01 2.000000e+00
      vertex   -5.784932e+00 3.198100e+01 4.000000e+00
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.169788e-01 -9.761763e-01 0.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
      vertex   -5.784932e+00 3.198100e+01 4.000000e+00
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.938503e-01 -9.558515e-01 0.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 2.000000e+00
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.938503e-01 -9.558515e-01 0.000000e+00
    outer loop
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
      vertex   -1.077763e+01 3.066093e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.688639e-01 -9.294834e-01 0.000000e+00
    outer loop
      vertex   -1.077763e+01 3.066093e+01 2.000000e+00
      vertex   -1.077763e+01 3.066093e+01 4.000000e+00
      vertex   -1.317957e+01 2.970772e+01 2.000000e+00
    endloop
  endfacet
  facet normal 3.688639e-01 -9.294834e-01 0.000000e+00
    outer loop
      vertex   -1.317957e+01 2.970772e+01 2.000000e+00
      vertex   -1.077763e+01 3.066093e+01 4.000000e+00
      vertex   -1.317957e+01 2.970772e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.415455e-01 -8.972389e-01 0.000000e+00
    outer loop
      vertex   -1.317957e+01 2.970772e+01 2.000000e+00
      vertex   -1.317957e+01 2.970772e+01 4.000000e+00
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
    endloop
  endfacet
  facet normal 4.415455e-01 -8.972389e-01 0.000000e+00
    outer loop
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
      vertex   -1.317957e+01 2.970772e+01 4.000000e+00
      vertex   -1.549819e+01 2.856669e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.114355e-01 -8.593217e-01 0.000000e+00
    outer loop
      vertex   -1.549819e+01 2.856669e+01 2.000000e+00
      vertex   -1.549819e+01 2.856669e+01 4.000000e+00
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.114355e-01 -8.593217e-01 0.000000e+00
    outer loop
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
      vertex   -1.549819e+01 2.856669e+01 4.000000e+00
      vertex   -1.771883e+01 2.724506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.780920e-01 -8.159716e-01 0.000000e+00
    outer loop
      vertex   -1.771883e+01 2.724506e+01 2.000000e+00
      vertex   -1.771883e+01 2.724506e+01 4.000000e+00
      vertex   -1.982744e+01 2.575117e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.780920e-01 -8.159716e-01 0.000000e+00
    outer loop
      vertex   -1.982744e+01 2.575117e+01 2.000000e+00
      vertex   -1.771883e+01 2.724506e+01 4.000000e+00
      vertex   -1.982744e+01 2.575117e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.410936e-01 -7.674627e-01 0.000000e+00
    outer loop
      vertex   -1.982744e+01 2.575117e+01 2.000000e+00
      vertex   -1.982744e+01 2.575117e+01 4.000000e+00
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
    endloop
  endfacet
  facet normal 6.410936e-01 -7.674627e-01 0.000000e+00
    outer loop
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
      vertex   -1.982744e+01 2.575117e+01 4.000000e+00
      vertex   -2.181070e+01 2.409447e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.000421e-01 -7.141016e-01 0.000000e+00
    outer loop
      vertex   -2.181070e+01 2.409447e+01 2.000000e+00
      vertex   -2.181070e+01 2.409447e+01 4.000000e+00
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.000421e-01 -7.141016e-01 0.000000e+00
    outer loop
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
      vertex   -2.181070e+01 2.409447e+01 4.000000e+00
      vertex   -2.365606e+01 2.228544e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.545646e-01 -6.562257e-01 0.000000e+00
    outer loop
      vertex   -2.365606e+01 2.228544e+01 2.000000e+00
      vertex   -2.365606e+01 2.228544e+01 4.000000e+00
      vertex   -2.535186e+01 2.033551e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.545646e-01 -6.562257e-01 0.000000e+00
    outer loop
      vertex   -2.535186e+01 2.033551e+01 2.000000e+00
      vertex   -2.365606e+01 2.228544e+01 4.000000e+00
      vertex   -2.535186e+01 2.033551e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.043166e-01 -5.942010e-01 0.000000e+00
    outer loop
      vertex   -2.535186e+01 2.033551e+01 2.000000e+00
      vertex   -2.535186e+01 2.033551e+01 4.000000e+00
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.043166e-01 -5.942010e-01 0.000000e+00
    outer loop
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
      vertex   -2.535186e+01 2.033551e+01 4.000000e+00
      vertex   -2.688738e+01 1.825702e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.489834e-01 -5.284196e-01 0.000000e+00
    outer loop
      vertex   -2.688738e+01 1.825702e+01 2.000000e+00
      vertex   -2.688738e+01 1.825702e+01 4.000000e+00
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.489834e-01 -5.284196e-01 0.000000e+00
    outer loop
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
      vertex   -2.688738e+01 1.825702e+01 4.000000e+00
      vertex   -2.825291e+01 1.606310e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.882826e-01 -4.592973e-01 0.000000e+00
    outer loop
      vertex   -2.825291e+01 1.606310e+01 2.000000e+00
      vertex   -2.825291e+01 1.606310e+01 4.000000e+00
      vertex   -2.943981e+01 1.376762e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.882826e-01 -4.592973e-01 0.000000e+00
    outer loop
      vertex   -2.943981e+01 1.376762e+01 2.000000e+00
      vertex   -2.825291e+01 1.606310e+01 4.000000e+00
      vertex   -2.943981e+01 1.376762e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.219659e-01 -3.872712e-01 0.000000e+00
    outer loop
      vertex   -2.943981e+01 1.376762e+01 2.000000e+00
      vertex   -2.943981e+01 1.376762e+01 4.000000e+00
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
    endloop
  endfacet
  facet normal 9.219659e-01 -3.872712e-01 0.000000e+00
    outer loop
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
      vertex   -2.943981e+01 1.376762e+01 4.000000e+00
      vertex   -3.044059e+01 1.138510e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.498201e-01 -3.127966e-01 0.000000e+00
    outer loop
      vertex   -3.044059e+01 1.138510e+01 2.000000e+00
      vertex   -3.044059e+01 1.138510e+01 4.000000e+00
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.498201e-01 -3.127966e-01 0.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
      vertex   -3.044059e+01 1.138510e+01 4.000000e+00
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.716693e-01 -2.363444e-01 0.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 2.000000e+00
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
      vertex   -3.185966e+01 6.419640e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.716693e-01 -2.363444e-01 0.000000e+00
    outer loop
      vertex   -3.185966e+01 6.419640e+00 2.000000e+00
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
      vertex   -3.185966e+01 6.419640e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.873753e-01 -1.583980e-01 0.000000e+00
    outer loop
      vertex   -3.185966e+01 6.419640e+00 2.000000e+00
      vertex   -3.185966e+01 6.419640e+00 4.000000e+00
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.873753e-01 -1.583980e-01 0.000000e+00
    outer loop
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
      vertex   -3.185966e+01 6.419640e+00 4.000000e+00
      vertex   -3.226899e+01 3.868091e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.968388e-01 -7.945017e-02 0.000000e+00
    outer loop
      vertex   -3.226899e+01 3.868091e+00 2.000000e+00
      vertex   -3.226899e+01 3.868091e+00 4.000000e+00
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.968388e-01 -7.945017e-02 0.000000e+00
    outer loop
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
      vertex   -3.226899e+01 3.868091e+00 4.000000e+00
      vertex   -3.247431e+01 1.292087e+00 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 -0.000000e+00
    outer loop
      vertex   -3.247431e+01 1.292087e+00 2.000000e+00
      vertex   -3.247431e+01 1.292087e+00 4.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 2.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -3.247431e+01 -1.292087e+00 2.000000e+00
      vertex   -3.247431e+01 1.292087e+00 4.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.968388e-01 7.945017e-02 -0.000000e+00
    outer loop
      vertex   -3.247431e+01 -1.292087e+00 2.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.968388e-01 7.945017e-02 -0.000000e+00
    outer loop
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.873753e-01 1.583980e-01 -0.000000e+00
    outer loop
      vertex   -3.226899e+01 -3.868091e+00 2.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 4.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.873753e-01 1.583980e-01 -0.000000e+00
    outer loop
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 4.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.716693e-01 2.363444e-01 -0.000000e+00
    outer loop
      vertex   -3.185966e+01 -6.419640e+00 2.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 4.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.716693e-01 2.363444e-01 -0.000000e+00
    outer loop
      vertex   -3.124891e+01 -8.930603e+00 2.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 4.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
    endloop
  endfacet
  facet normal 9.498201e-01 3.127966e-01 -0.000000e+00
    outer loop
      vertex   -3.124891e+01 -8.930603e+00 2.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
    endloop
  endfacet
  facet normal 9.498201e-01 3.127966e-01 -0.000000e+00
    outer loop
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.219659e-01 3.872712e-01 -0.000000e+00
    outer loop
      vertex   -3.044059e+01 -1.138510e+01 2.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 4.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
    endloop
  endfacet
  facet normal 9.219659e-01 3.872712e-01 -0.000000e+00
    outer loop
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 4.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.882826e-01 4.592973e-01 -0.000000e+00
    outer loop
      vertex   -2.943981e+01 -1.376762e+01 2.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 4.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.882826e-01 4.592973e-01 -0.000000e+00
    outer loop
      vertex   -2.825291e+01 -1.606310e+01 2.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 4.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.489834e-01 5.284196e-01 -0.000000e+00
    outer loop
      vertex   -2.825291e+01 -1.606310e+01 2.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 4.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.489834e-01 5.284196e-01 -0.000000e+00
    outer loop
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 4.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.043166e-01 5.942010e-01 -0.000000e+00
    outer loop
      vertex   -2.688738e+01 -1.825702e+01 2.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 4.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
    endloop
  endfacet
  facet normal 8.043166e-01 5.942010e-01 -0.000000e+00
    outer loop
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 4.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.545646e-01 6.562257e-01 -0.000000e+00
    outer loop
      vertex   -2.535186e+01 -2.033551e+01 2.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 4.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.545646e-01 6.562257e-01 -0.000000e+00
    outer loop
      vertex   -2.365606e+01 -2.228544e+01 2.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 4.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.000421e-01 7.141016e-01 -0.000000e+00
    outer loop
      vertex   -2.365606e+01 -2.228544e+01 2.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 4.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.000421e-01 7.141016e-01 -0.000000e+00
    outer loop
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 4.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.410936e-01 7.674627e-01 -0.000000e+00
    outer loop
      vertex   -2.181070e+01 -2.409447e+01 2.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 4.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
    endloop
  endfacet
  facet normal 6.410936e-01 7.674627e-01 -0.000000e+00
    outer loop
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 4.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.780920e-01 8.159716e-01 -0.000000e+00
    outer loop
      vertex   -1.982744e+01 -2.575117e+01 2.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 4.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.780920e-01 8.159716e-01 -0.000000e+00
    outer loop
      vertex   -1.771883e+01 -2.724506e+01 2.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 4.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.114355e-01 8.593217e-01 -0.000000e+00
    outer loop
      vertex   -1.771883e+01 -2.724506e+01 2.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.114355e-01 8.593217e-01 -0.000000e+00
    outer loop
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.415455e-01 8.972389e-01 -0.000000e+00
    outer loop
      vertex   -1.549819e+01 -2.856669e+01 2.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 4.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 2.000000e+00
    endloop
  endfacet
  facet normal 4.415455e-01 8.972389e-01 -0.000000e+00
    outer loop
      vertex   -1.317957e+01 -2.970772e+01 2.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 4.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.688639e-01 9.294834e-01 -0.000000e+00
    outer loop
      vertex   -1.317957e+01 -2.970772e+01 2.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 4.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
    endloop
  endfacet
  facet normal 3.688639e-01 9.294834e-01 -0.000000e+00
    outer loop
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 4.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.938503e-01 9.558515e-01 -0.000000e+00
    outer loop
      vertex   -1.077763e+01 -3.066093e+01 2.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 4.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.938503e-01 9.558515e-01 -0.000000e+00
    outer loop
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 4.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.169788e-01 9.761763e-01 -0.000000e+00
    outer loop
      vertex   -8.307541e+00 -3.142029e+01 2.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 4.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.169788e-01 9.761763e-01 -0.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 4.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.387355e-01 9.903295e-01 -0.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 2.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 4.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
    endloop
  endfacet
  facet normal 1.387355e-01 9.903295e-01 -0.000000e+00
    outer loop
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 4.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.961513e-02 9.982214e-01 -0.000000e+00
    outer loop
      vertex   -3.225748e+00 -3.233952e+01 2.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 4.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.961513e-02 9.982214e-01 -0.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 4.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.988219e-02 9.998023e-01 0.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 2.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.988219e-02 9.998023e-01 0.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
      vertex   1.937492e+00 -3.244220e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.925380e-02 9.950622e-01 0.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 2.000000e+00
      vertex   1.937492e+00 -3.244220e+01 4.000000e+00
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.925380e-02 9.950622e-01 0.000000e+00
    outer loop
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
      vertex   1.937492e+00 -3.244220e+01 4.000000e+00
      vertex   4.508905e+00 -3.218571e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.779979e-01 9.840309e-01 0.000000e+00
    outer loop
      vertex   4.508905e+00 -3.218571e+01 2.000000e+00
      vertex   4.508905e+00 -3.218571e+01 4.000000e+00
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.779979e-01 9.840309e-01 0.000000e+00
    outer loop
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
      vertex   4.508905e+00 -3.218571e+01 4.000000e+00
      vertex   7.051811e+00 -3.172573e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.556166e-01 9.667782e-01 0.000000e+00
    outer loop
      vertex   7.051811e+00 -3.172573e+01 2.000000e+00
      vertex   7.051811e+00 -3.172573e+01 4.000000e+00
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
    endloop
  endfacet
  facet normal -2.556166e-01 9.667782e-01 0.000000e+00
    outer loop
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
      vertex   7.051811e+00 -3.172573e+01 4.000000e+00
      vertex   9.550134e+00 -3.106517e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.316193e-01 9.434133e-01 0.000000e+00
    outer loop
      vertex   9.550134e+00 -3.106517e+01 2.000000e+00
      vertex   9.550134e+00 -3.106517e+01 4.000000e+00
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
    endloop
  endfacet
  facet normal -3.316193e-01 9.434133e-01 0.000000e+00
    outer loop
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
      vertex   9.550134e+00 -3.106517e+01 4.000000e+00
      vertex   1.198808e+01 -3.020821e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.055253e-01 9.140838e-01 0.000000e+00
    outer loop
      vertex   1.198808e+01 -3.020821e+01 2.000000e+00
      vertex   1.198808e+01 -3.020821e+01 4.000000e+00
      vertex   1.435023e+01 -2.916026e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.055253e-01 9.140838e-01 0.000000e+00
    outer loop
      vertex   1.435023e+01 -2.916026e+01 2.000000e+00
      vertex   1.198808e+01 -3.020821e+01 4.000000e+00
      vertex   1.435023e+01 -2.916026e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.768675e-01 8.789752e-01 0.000000e+00
    outer loop
      vertex   1.435023e+01 -2.916026e+01 2.000000e+00
      vertex   1.435023e+01 -2.916026e+01 4.000000e+00
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.768675e-01 8.789752e-01 0.000000e+00
    outer loop
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
      vertex   1.435023e+01 -2.916026e+01 4.000000e+00
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.451948e-01 8.383094e-01 0.000000e+00
    outer loop
      vertex   1.662165e+01 -2.792795e+01 2.000000e+00
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
    endloop
  endfacet
  facet normal -5.451948e-01 8.383094e-01 0.000000e+00
    outer loop
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
      vertex   1.878799e+01 -2.651908e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.100751e-01 7.923436e-01 0.000000e+00
    outer loop
      vertex   1.878799e+01 -2.651908e+01 2.000000e+00
      vertex   1.878799e+01 -2.651908e+01 4.000000e+00
      vertex   2.083554e+01 -2.494254e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.100751e-01 7.923436e-01 0.000000e+00
    outer loop
      vertex   2.083554e+01 -2.494254e+01 2.000000e+00
      vertex   1.878799e+01 -2.651908e+01 4.000000e+00
      vertex   2.083554e+01 -2.494254e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.710984e-01 7.413683e-01 0.000000e+00
    outer loop
      vertex   2.083554e+01 -2.494254e+01 2.000000e+00
      vertex   2.083554e+01 -2.494254e+01 4.000000e+00
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.710984e-01 7.413683e-01 0.000000e+00
    outer loop
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
      vertex   2.083554e+01 -2.494254e+01 4.000000e+00
      vertex   2.275137e+01 -2.320830e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.278788e-01 6.857058e-01 0.000000e+00
    outer loop
      vertex   2.275137e+01 -2.320830e+01 2.000000e+00
      vertex   2.275137e+01 -2.320830e+01 4.000000e+00
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.278788e-01 6.857058e-01 0.000000e+00
    outer loop
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
      vertex   2.275137e+01 -2.320830e+01 4.000000e+00
      vertex   2.452335e+01 -2.132734e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.800573e-01 6.257081e-01 0.000000e+00
    outer loop
      vertex   2.452335e+01 -2.132734e+01 2.000000e+00
      vertex   2.452335e+01 -2.132734e+01 4.000000e+00
      vertex   2.614029e+01 -1.931153e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.800573e-01 6.257081e-01 0.000000e+00
    outer loop
      vertex   2.614029e+01 -1.931153e+01 2.000000e+00
      vertex   2.452335e+01 -2.132734e+01 4.000000e+00
      vertex   2.614029e+01 -1.931153e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.273040e-01 5.617544e-01 0.000000e+00
    outer loop
      vertex   2.614029e+01 -1.931153e+01 2.000000e+00
      vertex   2.614029e+01 -1.931153e+01 4.000000e+00
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
    endloop
  endfacet
  facet normal -8.273040e-01 5.617544e-01 0.000000e+00
    outer loop
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
      vertex   2.614029e+01 -1.931153e+01 4.000000e+00
      vertex   2.759196e+01 -1.717364e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.693203e-01 4.942492e-01 0.000000e+00
    outer loop
      vertex   2.759196e+01 -1.717364e+01 2.000000e+00
      vertex   2.759196e+01 -1.717364e+01 4.000000e+00
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
    endloop
  endfacet
  facet normal -8.693203e-01 4.942492e-01 0.000000e+00
    outer loop
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
      vertex   2.759196e+01 -1.717364e+01 4.000000e+00
      vertex   2.886918e+01 -1.492716e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.058404e-01 4.236191e-01 0.000000e+00
    outer loop
      vertex   2.886918e+01 -1.492716e+01 2.000000e+00
      vertex   2.886918e+01 -1.492716e+01 4.000000e+00
      vertex   2.996389e+01 -1.258631e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.058404e-01 4.236191e-01 0.000000e+00
    outer loop
      vertex   2.996389e+01 -1.258631e+01 2.000000e+00
      vertex   2.886918e+01 -1.492716e+01 4.000000e+00
      vertex   2.996389e+01 -1.258631e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.366335e-01 3.503108e-01 0.000000e+00
    outer loop
      vertex   2.996389e+01 -1.258631e+01 2.000000e+00
      vertex   2.996389e+01 -1.258631e+01 4.000000e+00
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
    endloop
  endfacet
  facet normal -9.366335e-01 3.503108e-01 0.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
      vertex   2.996389e+01 -1.258631e+01 4.000000e+00
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.615049e-01 2.747878e-01 0.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 2.000000e+00
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.615049e-01 2.747878e-01 0.000000e+00
    outer loop
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
      vertex   3.157925e+01 -7.681194e+00 4.000000e+00
    endloop
  endfacet
  facet normal -9.802974e-01 1.975274e-01 0.000000e+00
    outer loop
      vertex   3.157925e+01 -7.681194e+00 2.000000e+00
      vertex   3.157925e+01 -7.681194e+00 4.000000e+00
      vertex   3.208970e+01 -5.147936e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.802974e-01 1.975274e-01 0.000000e+00
    outer loop
      vertex   3.208970e+01 -5.147936e+00 2.000000e+00
      vertex   3.157925e+01 -7.681194e+00 4.000000e+00
      vertex   3.208970e+01 -5.147936e+00 4.000000e+00
    endloop
  endfacet
  facet normal -9.928921e-01 1.190182e-01 0.000000e+00
    outer loop
      vertex   3.208970e+01 -5.147936e+00 2.000000e+00
      vertex   3.208970e+01 -5.147936e+00 4.000000e+00
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.928921e-01 1.190182e-01 0.000000e+00
    outer loop
      vertex   3.239726e+01 -2.582130e+00 2.000000e+00
      vertex   3.208970e+01 -5.147936e+00 4.000000e+00
      vertex   3.239726e+01 -2.582130e+00 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -1.000000e+00 -0.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -1.000000e+00 -0.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -1.000000e+00 -0.000000e+00
    outer loop
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -1.000000e+00 -0.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.650568e-01 9.642328e-01 -1.712822e-15
    outer loop
      vertex   3.000000e+00 -1.418626e+01 0.000000e+00
      vertex   3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   4.672880e+00 -1.372640e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.650568e-01 9.642328e-01 0.000000e+00
    outer loop
      vertex   4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   3.000000e+00 -1.418626e+01 2.000000e+00
      vertex   4.672880e+00 -1.372640e+01 2.000000e+00
    endloop
  endfacet
  facet normal -3.783239e-01 9.256733e-01 0.000000e+00
    outer loop
      vertex   4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   6.278861e+00 -1.307004e+01 0.000000e+00
    endloop
  endfacet
  facet normal -3.783239e-01 9.256733e-01 0.000000e+00
    outer loop
      vertex   6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   6.278861e+00 -1.307004e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.861748e-01 8.738616e-01 0.000000e+00
    outer loop
      vertex   6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   7.794953e+00 -1.222656e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.861748e-01 8.738616e-01 0.000000e+00
    outer loop
      vertex   7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal -5.870654e-01 8.095395e-01 0.000000e+00
    outer loop
      vertex   7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.870654e-01 8.095395e-01 0.000000e+00
    outer loop
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.795515e-01 7.336278e-01 0.000000e+00
    outer loop
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   1.047225e+01 -1.002906e+01 0.000000e+00
    endloop
  endfacet
  facet normal -6.795515e-01 7.336278e-01 0.000000e+00
    outer loop
      vertex   1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
    endloop
  endfacet
  facet normal -7.623090e-01 6.472133e-01 0.000000e+00
    outer loop
      vertex   1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
    endloop
  endfacet
  facet normal -7.623090e-01 6.472133e-01 0.000000e+00
    outer loop
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
    endloop
  endfacet
  facet normal -8.341530e-01 5.515331e-01 0.000000e+00
    outer loop
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
    endloop
  endfacet
  facet normal -8.341530e-01 5.515331e-01 0.000000e+00
    outer loop
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
    endloop
  endfacet
  facet normal -8.940551e-01 4.479570e-01 0.000000e+00
    outer loop
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 0.000000e+00
    endloop
  endfacet
  facet normal -8.940551e-01 4.479570e-01 0.000000e+00
    outer loop
      vertex   1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.411576e-01 3.379679e-01 0.000000e+00
    outer loop
      vertex   1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.411576e-01 3.379679e-01 0.000000e+00
    outer loop
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.747863e-01 2.231403e-01 0.000000e+00
    outer loop
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.747863e-01 2.231403e-01 0.000000e+00
    outer loop
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.944597e-01 1.051182e-01 0.000000e+00
    outer loop
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal -9.944597e-01 1.051182e-01 0.000000e+00
    outer loop
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
    endloop
  endfacet
  facet normal -9.998962e-01 -1.440885e-02 0.000000e+00
    outer loop
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.998962e-01 -1.440885e-02 0.000000e+00
    outer loop
      vertex   1.446003e+01 1.075931e+00 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.910179e-01 -1.337296e-01 0.000000e+00
    outer loop
      vertex   1.446003e+01 1.075931e+00 0.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.910179e-01 -1.337296e-01 0.000000e+00
    outer loop
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
      vertex   1.446003e+01 1.075931e+00 2.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.679519e-01 -2.511358e-01 0.000000e+00
    outer loop
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.679519e-01 -2.511358e-01 0.000000e+00
    outer loop
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
      vertex   1.422801e+01 2.795281e+00 2.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
    endloop
  endfacet
  facet normal -9.310284e-01 -3.649467e-01 0.000000e+00
    outer loop
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
    endloop
  endfacet
  facet normal -9.310284e-01 -3.649467e-01 0.000000e+00
    outer loop
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
      vertex   1.379231e+01 4.474613e+00 2.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
    endloop
  endfacet
  facet normal -8.807761e-01 -4.735329e-01 0.000000e+00
    outer loop
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 0.000000e+00
    endloop
  endfacet
  facet normal -8.807761e-01 -4.735329e-01 0.000000e+00
    outer loop
      vertex   1.233760e+01 7.617973e+00 0.000000e+00
      vertex   1.315915e+01 6.089885e+00 2.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
    endloop
  endfacet
  facet normal -8.179144e-01 -5.753400e-01 0.000000e+00
    outer loop
      vertex   1.233760e+01 7.617973e+00 0.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
    endloop
  endfacet
  facet normal -8.179144e-01 -5.753400e-01 0.000000e+00
    outer loop
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
      vertex   1.233760e+01 7.617973e+00 2.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
    endloop
  endfacet
  facet normal -7.433432e-01 -6.689102e-01 0.000000e+00
    outer loop
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.433432e-01 -6.689102e-01 0.000000e+00
    outer loop
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
      vertex   1.133943e+01 9.037000e+00 2.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
    endloop
  endfacet
  facet normal -6.581301e-01 -7.529042e-01 0.000000e+00
    outer loop
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
    endloop
  endfacet
  facet normal -6.581301e-01 -7.529042e-01 0.000000e+00
    outer loop
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
      vertex   1.017891e+01 1.032665e+01 2.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
    endloop
  endfacet
  facet normal -5.634950e-01 -8.261195e-01 0.000000e+00
    outer loop
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.634950e-01 -8.261195e-01 0.000000e+00
    outer loop
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
      vertex   8.872674e+00 1.146846e+01 2.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
    endloop
  endfacet
  facet normal -4.607928e-01 -8.875077e-01 0.000000e+00
    outer loop
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
      vertex   5.899645e+00 1.324553e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.607928e-01 -8.875077e-01 0.000000e+00
    outer loop
      vertex   5.899645e+00 1.324553e+01 0.000000e+00
      vertex   7.439412e+00 1.244609e+01 2.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
    endloop
  endfacet
  facet normal -3.514938e-01 -9.361902e-01 0.000000e+00
    outer loop
      vertex   5.899645e+00 1.324553e+01 0.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
    endloop
  endfacet
  facet normal -3.514938e-01 -9.361902e-01 0.000000e+00
    outer loop
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
      vertex   5.899645e+00 1.324553e+01 2.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
    endloop
  endfacet
  facet normal -2.371627e-01 -9.714700e-01 0.000000e+00
    outer loop
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.371627e-01 -9.714700e-01 0.000000e+00
    outer loop
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
      vertex   4.275417e+00 1.385535e+01 2.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
    endloop
  endfacet
  facet normal -1.194363e-01 -9.928419e-01 0.000000e+00
    outer loop
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
    endloop
  endfacet
  facet normal -1.194363e-01 -9.928419e-01 0.000000e+00
    outer loop
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
      vertex   2.589981e+00 1.426681e+01 2.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
      vertex   8.674668e-01 1.447403e+01 2.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
    endloop
  endfacet
  facet normal 1.194363e-01 -9.928419e-01 0.000000e+00
    outer loop
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
    endloop
  endfacet
  facet normal 1.194363e-01 -9.928419e-01 0.000000e+00
    outer loop
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 2.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.371627e-01 -9.714700e-01 0.000000e+00
    outer loop
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.371627e-01 -9.714700e-01 0.000000e+00
    outer loop
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
      vertex   -2.589981e+00 1.426681e+01 2.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
    endloop
  endfacet
  facet normal 3.514938e-01 -9.361902e-01 0.000000e+00
    outer loop
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 0.000000e+00
    endloop
  endfacet
  facet normal 3.514938e-01 -9.361902e-01 0.000000e+00
    outer loop
      vertex   -5.899645e+00 1.324553e+01 0.000000e+00
      vertex   -4.275417e+00 1.385535e+01 2.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
    endloop
  endfacet
  facet normal 4.607928e-01 -8.875077e-01 0.000000e+00
    outer loop
      vertex   -5.899645e+00 1.324553e+01 0.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.607928e-01 -8.875077e-01 0.000000e+00
    outer loop
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
      vertex   -5.899645e+00 1.324553e+01 2.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.634950e-01 -8.261195e-01 0.000000e+00
    outer loop
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.634950e-01 -8.261195e-01 0.000000e+00
    outer loop
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
      vertex   -7.439412e+00 1.244609e+01 2.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
    endloop
  endfacet
  facet normal 6.581301e-01 -7.529042e-01 0.000000e+00
    outer loop
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
    endloop
  endfacet
  facet normal 6.581301e-01 -7.529042e-01 0.000000e+00
    outer loop
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
      vertex   -8.872674e+00 1.146846e+01 2.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
    endloop
  endfacet
  facet normal 7.433432e-01 -6.689102e-01 0.000000e+00
    outer loop
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
    endloop
  endfacet
  facet normal 7.433432e-01 -6.689102e-01 0.000000e+00
    outer loop
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
      vertex   -1.017891e+01 1.032665e+01 2.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
    endloop
  endfacet
  facet normal 8.179144e-01 -5.753400e-01 0.000000e+00
    outer loop
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
      vertex   -1.233760e+01 7.617973e+00 0.000000e+00
    endloop
  endfacet
  facet normal 8.179144e-01 -5.753400e-01 0.000000e+00
    outer loop
      vertex   -1.233760e+01 7.617973e+00 0.000000e+00
      vertex   -1.133943e+01 9.037000e+00 2.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
    endloop
  endfacet
  facet normal 8.807761e-01 -4.735329e-01 0.000000e+00
    outer loop
      vertex   -1.233760e+01 7.617973e+00 0.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
    endloop
  endfacet
  facet normal 8.807761e-01 -4.735329e-01 0.000000e+00
    outer loop
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
      vertex   -1.233760e+01 7.617973e+00 2.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.310284e-01 -3.649467e-01 0.000000e+00
    outer loop
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.310284e-01 -3.649467e-01 0.000000e+00
    outer loop
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
      vertex   -1.315915e+01 6.089885e+00 2.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.679519e-01 -2.511358e-01 0.000000e+00
    outer loop
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.679519e-01 -2.511358e-01 0.000000e+00
    outer loop
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
      vertex   -1.379231e+01 4.474613e+00 2.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.910179e-01 -1.337296e-01 0.000000e+00
    outer loop
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
      vertex   -1.446003e+01 1.075931e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.910179e-01 -1.337296e-01 0.000000e+00
    outer loop
      vertex   -1.446003e+01 1.075931e+00 0.000000e+00
      vertex   -1.422801e+01 2.795281e+00 2.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.998962e-01 -1.440885e-02 0.000000e+00
    outer loop
      vertex   -1.446003e+01 1.075931e+00 0.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal 9.998962e-01 -1.440885e-02 0.000000e+00
    outer loop
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   -1.446003e+01 1.075931e+00 2.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
    endloop
  endfacet
  facet normal 9.944597e-01 1.051182e-01 -0.000000e+00
    outer loop
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.944597e-01 1.051182e-01 -0.000000e+00
    outer loop
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 2.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.747863e-01 2.231403e-01 -0.000000e+00
    outer loop
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.747863e-01 2.231403e-01 -0.000000e+00
    outer loop
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 2.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
    endloop
  endfacet
  facet normal 9.411576e-01 3.379679e-01 -0.000000e+00
    outer loop
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 0.000000e+00
    endloop
  endfacet
  facet normal 9.411576e-01 3.379679e-01 -0.000000e+00
    outer loop
      vertex   -1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 2.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
    endloop
  endfacet
  facet normal 8.940551e-01 4.479570e-01 -0.000000e+00
    outer loop
      vertex   -1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
    endloop
  endfacet
  facet normal 8.940551e-01 4.479570e-01 -0.000000e+00
    outer loop
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 2.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
    endloop
  endfacet
  facet normal 8.341530e-01 5.515331e-01 -0.000000e+00
    outer loop
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
    endloop
  endfacet
  facet normal 8.341530e-01 5.515331e-01 -0.000000e+00
    outer loop
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 2.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
    endloop
  endfacet
  facet normal 7.623090e-01 6.472133e-01 -0.000000e+00
    outer loop
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.623090e-01 6.472133e-01 -0.000000e+00
    outer loop
      vertex   -1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 2.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
    endloop
  endfacet
  facet normal 6.795515e-01 7.336278e-01 -0.000000e+00
    outer loop
      vertex   -1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
    endloop
  endfacet
  facet normal 6.795515e-01 7.336278e-01 -0.000000e+00
    outer loop
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 2.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
    endloop
  endfacet
  facet normal 5.870654e-01 8.095395e-01 -0.000000e+00
    outer loop
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.870654e-01 8.095395e-01 -0.000000e+00
    outer loop
      vertex   -7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 2.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
    endloop
  endfacet
  facet normal 4.861748e-01 8.738616e-01 -0.000000e+00
    outer loop
      vertex   -7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.861748e-01 8.738616e-01 -0.000000e+00
    outer loop
      vertex   -6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 2.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 2.000000e+00
    endloop
  endfacet
  facet normal 3.783239e-01 9.256733e-01 -0.000000e+00
    outer loop
      vertex   -6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 0.000000e+00
    endloop
  endfacet
  facet normal 3.783239e-01 9.256733e-01 -0.000000e+00
    outer loop
      vertex   -4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 2.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 2.000000e+00
    endloop
  endfacet
  facet normal 2.650568e-01 9.642328e-01 -0.000000e+00
    outer loop
      vertex   -4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   -3.000000e+00 -1.418626e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.650568e-01 9.642328e-01 -0.000000e+00
    outer loop
      vertex   -3.000000e+00 -1.418626e+01 0.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 2.000000e+00
      vertex   -3.000000e+00 -1.418626e+01 2.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.239726e+01 -2.582130e+00 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   3.250000e+01 3.980102e-15 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   3.239726e+01 2.582130e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   3.239726e+01 2.582130e+00 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   3.208970e+01 5.147936e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   3.208970e+01 5.147936e+00 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   3.157925e+01 7.681194e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 7.681194e+00 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
      vertex   8.490000e+01 6.890000e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 1.016589e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.996389e+01 1.258631e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.996389e+01 1.258631e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.886918e+01 1.492716e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 1.492716e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.759196e+01 1.717364e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.759196e+01 1.717364e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.614029e+01 1.931153e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.614029e+01 1.931153e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.452335e+01 2.132734e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 2.132734e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.275137e+01 2.320830e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.275137e+01 2.320830e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   2.083554e+01 2.494254e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   2.083554e+01 2.494254e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   1.878799e+01 2.651908e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   1.878799e+01 2.651908e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   1.662165e+01 2.792795e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   1.662165e+01 2.792795e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   1.435023e+01 2.916026e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   1.435023e+01 2.916026e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   1.198808e+01 3.020821e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   1.198808e+01 3.020821e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
      vertex   6.890000e+01 6.890000e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 3.106517e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   7.051811e+00 3.172573e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 3.172573e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   4.508905e+00 3.218571e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 3.218571e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   1.937492e+00 3.244220e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 3.244220e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
      vertex   6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 3.249358e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -3.225748e+00 3.233952e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 3.233952e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -5.784932e+00 3.198100e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 3.198100e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
      vertex   -6.890000e+01 8.490000e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 3.142029e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -1.077763e+01 3.066093e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 3.066093e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -1.317957e+01 2.970772e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.317957e+01 2.970772e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -1.549819e+01 2.856669e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 2.856669e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -1.771883e+01 2.724506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 2.724506e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -1.982744e+01 2.575117e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.982744e+01 2.575117e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.181070e+01 2.409447e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 2.409447e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.365606e+01 2.228544e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.365606e+01 2.228544e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.535186e+01 2.033551e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.535186e+01 2.033551e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.688738e+01 1.825702e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 1.825702e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.825291e+01 1.606310e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.825291e+01 1.606310e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -2.943981e+01 1.376762e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.943981e+01 1.376762e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -3.044059e+01 1.138510e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 1.138510e+01 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
      vertex   -6.890000e+01 6.890000e+01 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 8.930603e+00 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -3.185966e+01 6.419640e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.185966e+01 6.419640e+00 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -3.226899e+01 3.868091e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 3.868091e+00 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -3.247431e+01 1.292087e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 1.292087e+00 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
      vertex   -8.490000e+01 6.890000e+01 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.247431e+01 -1.292087e+00 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   -3.226899e+01 -3.868091e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.226899e+01 -3.868091e+00 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   -3.185966e+01 -6.419640e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.185966e+01 -6.419640e+00 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
      vertex   -8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.124891e+01 -8.930603e+00 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -3.044059e+01 -1.138510e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.044059e+01 -1.138510e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.943981e+01 -1.376762e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.943981e+01 -1.376762e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.825291e+01 -1.606310e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.825291e+01 -1.606310e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.688738e+01 -1.825702e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.688738e+01 -1.825702e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.535186e+01 -2.033551e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.535186e+01 -2.033551e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.365606e+01 -2.228544e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.365606e+01 -2.228544e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.181070e+01 -2.409447e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.181070e+01 -2.409447e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -1.982744e+01 -2.575117e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.982744e+01 -2.575117e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.771883e+01 -2.724506e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -1.549819e+01 -2.856669e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.549819e+01 -2.856669e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -1.317957e+01 -2.970772e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.317957e+01 -2.970772e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -1.077763e+01 -3.066093e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -1.077763e+01 -3.066093e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -8.307541e+00 -3.142029e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.307541e+00 -3.142029e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -5.784932e+00 -3.198100e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -5.784932e+00 -3.198100e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -3.225748e+00 -3.233952e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -3.225748e+00 -3.233952e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.461711e-01 -3.249358e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   1.937492e+00 -3.244220e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.937492e+00 -3.244220e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   4.508905e+00 -3.218571e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   4.508905e+00 -3.218571e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   7.051811e+00 -3.172573e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.051811e+00 -3.172573e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   9.550134e+00 -3.106517e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   9.550134e+00 -3.106517e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   1.198808e+01 -3.020821e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.198808e+01 -3.020821e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   1.435023e+01 -2.916026e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.435023e+01 -2.916026e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.662165e+01 -2.792795e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   1.878799e+01 -2.651908e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   1.878799e+01 -2.651908e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.083554e+01 -2.494254e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.083554e+01 -2.494254e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.275137e+01 -2.320830e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.275137e+01 -2.320830e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.452335e+01 -2.132734e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.452335e+01 -2.132734e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.614029e+01 -1.931153e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.614029e+01 -1.931153e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.759196e+01 -1.717364e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.759196e+01 -1.717364e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.886918e+01 -1.492716e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.886918e+01 -1.492716e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.996389e+01 -1.258631e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.996389e+01 -1.258631e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.086915e+01 -1.016589e+01 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   3.157925e+01 -7.681194e+00 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.157925e+01 -7.681194e+00 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   3.208970e+01 -5.147936e+00 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   3.208970e+01 -5.147936e+00 4.000000e+00
      vertex   8.490000e+01 -6.890000e+01 4.000000e+00
      vertex   3.239726e+01 -2.582130e+00 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   -6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   6.890000e+01 -8.490000e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 4.000000e+00
      vertex   6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   2.300000e+00 -7.155000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 4.000000e+00
      vertex   -6.890000e+01 -6.890000e+01 4.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 0.000000e+00
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 0.000000e+00
      vertex   -8.565000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 0.000000e+00
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 0.000000e+00
      vertex   -8.524293e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 0.000000e+00
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 0.000000e+00
      vertex   -8.474567e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 0.000000e+00
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 0.000000e+00
      vertex   -8.407080e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 0.000000e+00
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 0.000000e+00
      vertex   -8.323882e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 0.000000e+00
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 0.000000e+00
      vertex   -8.227500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 0.000000e+00
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 0.000000e+00
      vertex   -8.120864e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 0.000000e+00
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 0.000000e+00
      vertex   -8.007213e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 0.000000e+00
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 0.000000e+00
      vertex   -7.772787e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
      vertex   -8.554745e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   7.215000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   7.255707e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   7.305433e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   8.554745e+01 -7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   8.565000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   8.554745e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   8.524293e+01 -8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   8.474567e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   8.407080e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   8.323882e+01 -8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   8.227500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   8.120864e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   8.007213e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   7.890000e+01 -8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   7.772787e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   7.552500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   7.456118e+01 -8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   7.372920e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   7.305433e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   7.255707e+01 -8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   7.225255e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
      vertex   7.772787e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 0.000000e+00
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 0.000000e+00
      vertex   7.890000e+01 8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 0.000000e+00
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 0.000000e+00
      vertex   8.007213e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 0.000000e+00
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 0.000000e+00
      vertex   8.120864e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 0.000000e+00
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 0.000000e+00
      vertex   8.227500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 0.000000e+00
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 0.000000e+00
      vertex   8.323882e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 0.000000e+00
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 0.000000e+00
      vertex   8.407080e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 0.000000e+00
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 0.000000e+00
      vertex   8.474567e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 0.000000e+00
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 0.000000e+00
      vertex   8.524293e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 0.000000e+00
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 0.000000e+00
      vertex   8.554745e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 0.000000e+00
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 0.000000e+00
      vertex   8.565000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 0.000000e+00
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 0.000000e+00
      vertex   8.554745e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 1.000000e+00 -0.000000e+00 0.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -1.000000e+00 0.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.000000e+00 0.000000e+00 0.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -1.000000e+00 -0.000000e+00 0.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 -0.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 1.000000e+00 0.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -8.106506e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -8.066777e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -8.015000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -7.954705e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.825295e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.765000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.713223e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.673494e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.640000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.673494e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.713223e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.765000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   -6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.890000e+01 -7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -7.954705e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.015000e+01 -7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.066777e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.106506e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.131481e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.131481e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.106506e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.066777e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.015000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -7.954705e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -7.890000e+01 7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -7.765000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -7.713223e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -7.673494e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
      vertex   -6.890000e+01 6.890000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.640000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.648519e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.673494e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.713223e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.765000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.825295e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -7.954705e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.015000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.066777e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.106506e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.131481e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 1.000000e+01
      vertex   7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   7.640000e+01 -7.890000e+01 1.000000e+01
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   7.648519e+01 -7.825295e+01 1.000000e+01
      vertex   7.673494e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   7.673494e+01 -7.765000e+01 1.000000e+01
      vertex   7.713223e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 1.000000e+01
      vertex   7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   7.765000e+01 -7.673494e+01 1.000000e+01
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -6.890000e+01 1.000000e+01
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   7.825295e+01 -7.648519e+01 1.000000e+01
      vertex   7.890000e+01 -7.640000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   7.890000e+01 -7.640000e+01 1.000000e+01
      vertex   7.954705e+01 -7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 1.000000e+01
      vertex   8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.015000e+01 -7.673494e+01 1.000000e+01
      vertex   8.066777e+01 -7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.066777e+01 -7.713223e+01 1.000000e+01
      vertex   8.106506e+01 -7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 1.000000e+01
      vertex   8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.131481e+01 -7.825295e+01 1.000000e+01
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.140000e+01 -7.890000e+01 1.000000e+01
      vertex   8.131481e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.131481e+01 -7.954705e+01 1.000000e+01
      vertex   8.106506e+01 -8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 1.000000e+01
      vertex   8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.066777e+01 -8.066777e+01 1.000000e+01
      vertex   8.015000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.015000e+01 -8.106506e+01 1.000000e+01
      vertex   7.954705e+01 -8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 1.000000e+01
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 1.000000e+01
      vertex   7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   7.825295e+01 -8.131481e+01 1.000000e+01
      vertex   7.765000e+01 -8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   7.765000e+01 -8.106506e+01 1.000000e+01
      vertex   7.713223e+01 -8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 1.000000e+01
      vertex   7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 -8.490000e+01 1.000000e+01
      vertex   7.673494e+01 -8.015000e+01 1.000000e+01
      vertex   7.648519e+01 -7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
      vertex   7.640000e+01 7.890000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   7.640000e+01 7.890000e+01 1.000000e+01
      vertex   7.648519e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   7.648519e+01 7.954705e+01 1.000000e+01
      vertex   7.673494e+01 8.015000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 1.000000e+01
      vertex   7.713223e+01 8.066777e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   7.713223e+01 8.066777e+01 1.000000e+01
      vertex   7.765000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   7.765000e+01 8.106506e+01 1.000000e+01
      vertex   7.825295e+01 8.131481e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 1.000000e+01
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 1.000000e+01
      vertex   7.954705e+01 8.131481e+01 1.000000e+01
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   7.954705e+01 8.131481e+01 1.000000e+01
      vertex   8.015000e+01 8.106506e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   8.015000e+01 8.106506e+01 1.000000e+01
      vertex   8.066777e+01 8.066777e+01 1.000000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 1.000000e+01
      vertex   8.106506e+01 8.015000e+01 1.000000e+01
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   8.106506e+01 8.015000e+01 1.000000e+01
      vertex   8.131481e+01 7.954705e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   8.131481e+01 7.954705e+01 1.000000e+01
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.140000e+01 7.890000e+01 1.000000e+01
      vertex   8.131481e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.131481e+01 7.825295e+01 1.000000e+01
      vertex   8.106506e+01 7.765000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 1.000000e+01
      vertex   8.066777e+01 7.713223e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.066777e+01 7.713223e+01 1.000000e+01
      vertex   8.015000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.015000e+01 7.673494e+01 1.000000e+01
      vertex   7.954705e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 1.000000e+01
      vertex   7.890000e+01 7.640000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   7.890000e+01 7.640000e+01 1.000000e+01
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   7.825295e+01 7.648519e+01 1.000000e+01
      vertex   7.765000e+01 7.673494e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   7.765000e+01 7.673494e+01 1.000000e+01
      vertex   7.713223e+01 7.713223e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 1.000000e+01
      vertex   7.673494e+01 7.765000e+01 1.000000e+01
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   7.673494e+01 7.765000e+01 1.000000e+01
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   6.890000e+01 6.890000e+01 1.000000e+01
      vertex   7.648519e+01 7.825295e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.490000e+01 -6.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -8.490000e+01 6.890000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -6.890000e+01 8.490000e+01 1.000000e+01
      vertex   6.890000e+01 8.490000e+01 1.000000e+01
      vertex   -8.890000e+01 8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.490000e+01 6.890000e+01 1.000000e+01
      vertex   8.490000e+01 -6.890000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   8.890000e+01 -8.890000e+01 1.000000e+01
      vertex   -6.890000e+01 -8.490000e+01 1.000000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   3.000000e+00 -1.418626e+01 0.000000e+00
      vertex   4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   6.278861e+00 -1.307004e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   7.794953e+00 -1.222656e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   1.047225e+01 -1.002906e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   7.305433e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   7.255707e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   1.332917e+01 -5.708180e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   8.120864e+01 7.255707e+01 0.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   8.227500e+01 7.305433e+01 0.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   8.323882e+01 7.372920e+01 0.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   8.407080e+01 7.456118e+01 0.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   8.474567e+01 7.552500e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.524293e+01 7.659136e+01 0.000000e+00
      vertex   8.554745e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.554745e+01 7.772787e+01 0.000000e+00
      vertex   8.565000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
      vertex   1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   1.446003e+01 1.075931e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
      vertex   1.446003e+01 1.075931e+00 0.000000e+00
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 0.000000e+00
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
      vertex   1.422801e+01 2.795281e+00 0.000000e+00
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 0.000000e+00
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
      vertex   1.379231e+01 4.474613e+00 0.000000e+00
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 0.000000e+00
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
      vertex   1.315915e+01 6.089885e+00 0.000000e+00
      vertex   1.233760e+01 7.617973e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
      vertex   1.233760e+01 7.617973e+00 0.000000e+00
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 0.000000e+00
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
      vertex   1.133943e+01 9.037000e+00 0.000000e+00
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 0.000000e+00
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
      vertex   1.017891e+01 1.032665e+01 0.000000e+00
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 0.000000e+00
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
      vertex   8.872674e+00 1.146846e+01 0.000000e+00
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 0.000000e+00
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
      vertex   7.439412e+00 1.244609e+01 0.000000e+00
      vertex   5.899645e+00 1.324553e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
      vertex   5.899645e+00 1.324553e+01 0.000000e+00
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 0.000000e+00
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
      vertex   4.275417e+00 1.385535e+01 0.000000e+00
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 0.000000e+00
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
      vertex   2.589981e+00 1.426681e+01 0.000000e+00
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 0.000000e+00
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
      vertex   8.674668e-01 1.447403e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
      vertex   -8.674668e-01 1.447403e+01 0.000000e+00
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 0.000000e+00
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
      vertex   -2.589981e+00 1.426681e+01 0.000000e+00
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 0.000000e+00
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
      vertex   -4.275417e+00 1.385535e+01 0.000000e+00
      vertex   -5.899645e+00 1.324553e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
      vertex   -5.899645e+00 1.324553e+01 0.000000e+00
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 0.000000e+00
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
      vertex   -7.439412e+00 1.244609e+01 0.000000e+00
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 0.000000e+00
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
      vertex   -8.872674e+00 1.146846e+01 0.000000e+00
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 0.000000e+00
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
      vertex   -1.017891e+01 1.032665e+01 0.000000e+00
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 0.000000e+00
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
      vertex   -1.133943e+01 9.037000e+00 0.000000e+00
      vertex   -1.233760e+01 7.617973e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
      vertex   -1.233760e+01 7.617973e+00 0.000000e+00
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 0.000000e+00
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
      vertex   -1.315915e+01 6.089885e+00 0.000000e+00
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 0.000000e+00
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
      vertex   -1.379231e+01 4.474613e+00 0.000000e+00
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 0.000000e+00
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
      vertex   -1.422801e+01 2.795281e+00 0.000000e+00
      vertex   -1.446003e+01 1.075931e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
      vertex   -1.446003e+01 1.075931e+00 0.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 0.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 0.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 0.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 0.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 0.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 0.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 0.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 0.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 0.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 0.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 0.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 0.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   -1.430265e+01 -2.384144e+00 0.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 0.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   -1.391552e+01 -4.075334e+00 0.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   -1.332917e+01 -5.708180e+00 0.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 0.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   -1.255199e+01 -7.259306e+00 0.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 0.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -1.159512e+01 -8.706506e+00 0.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   -1.047225e+01 -1.002906e+01 0.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.897777e+00 -2.377646e+01 0.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   -9.199450e+00 -1.120804e+01 0.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   -7.794953e+00 -1.222656e+01 0.000000e+00
      vertex   -6.278861e+00 -1.307004e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -6.278861e+00 -1.307004e+01 0.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -3.000000e+00 -2.300000e+01 0.000000e+00
      vertex   -4.672880e+00 -1.372640e+01 0.000000e+00
      vertex   -3.000000e+00 -1.418626e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.598076e+00 -2.450000e+01 0.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -1.836970e-16 -2.600000e+01 0.000000e+00
      vertex   7.764571e-01 -2.589778e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   7.764571e-01 -2.589778e+01 0.000000e+00
      vertex   1.500000e+00 -2.559808e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   1.500000e+00 -2.559808e+01 0.000000e+00
      vertex   2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   2.121320e+00 -2.512132e+01 0.000000e+00
      vertex   2.598076e+00 -2.450000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   7.215000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   7.225255e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 0.000000e+00
      vertex   -8.565000e+01 7.890000e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.565000e+01 7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 7.772787e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.554745e+01 7.772787e+01 0.000000e+00
      vertex   -8.524293e+01 7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 0.000000e+00
      vertex   -7.890000e+01 7.215000e+01 0.000000e+00
      vertex   -1.448503e+01 -6.588226e-01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
      vertex   -7.225255e+01 8.007213e+01 0.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 0.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
      vertex   -7.255707e+01 8.120864e+01 0.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 0.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
      vertex   -7.305433e+01 8.227500e+01 0.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 0.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
      vertex   -7.372920e+01 8.323882e+01 0.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 0.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
      vertex   -7.456118e+01 8.407080e+01 0.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 0.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -7.552500e+01 8.474567e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -7.659136e+01 8.524293e+01 0.000000e+00
      vertex   -7.772787e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -7.772787e+01 8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 0.000000e+00
      vertex   -8.007213e+01 8.554745e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.007213e+01 8.554745e+01 0.000000e+00
      vertex   -8.120864e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.120864e+01 8.524293e+01 0.000000e+00
      vertex   -8.227500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 0.000000e+00
      vertex   -8.323882e+01 8.407080e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.323882e+01 8.407080e+01 0.000000e+00
      vertex   -8.407080e+01 8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.407080e+01 8.323882e+01 0.000000e+00
      vertex   -8.474567e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 0.000000e+00
      vertex   -8.524293e+01 8.120864e+01 0.000000e+00
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 8.890000e+01 0.000000e+00
      vertex   -8.524293e+01 8.120864e+01 0.000000e+00
      vertex   -8.554745e+01 8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   7.305433e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   7.372920e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   7.552500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   7.659136e+01 -8.524293e+01 0.000000e+00
      vertex   7.772787e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   7.890000e+01 -8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   8.120864e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   8.227500e+01 -8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   8.407080e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   8.474567e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   8.554745e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   8.565000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   8.524293e+01 -7.659136e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 0.000000e+00
      vertex   7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 0.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 0.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 0.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 0.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 0.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 0.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 0.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 0.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 0.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 0.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 0.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 0.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 0.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 0.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 0.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 0.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 0.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 0.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 0.000000e+00
      vertex   -2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 0.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 0.000000e+00
      vertex   -2.300000e+00 -7.155000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 0.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 0.000000e+00
      vertex   8.554745e+01 8.007213e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.554745e+01 8.007213e+01 0.000000e+00
      vertex   8.524293e+01 8.120864e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.524293e+01 8.120864e+01 0.000000e+00
      vertex   8.474567e+01 8.227500e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 0.000000e+00
      vertex   8.407080e+01 8.323882e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.407080e+01 8.323882e+01 0.000000e+00
      vertex   8.323882e+01 8.407080e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.323882e+01 8.407080e+01 0.000000e+00
      vertex   8.227500e+01 8.474567e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 0.000000e+00
      vertex   8.120864e+01 8.524293e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.120864e+01 8.524293e+01 0.000000e+00
      vertex   8.007213e+01 8.554745e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   8.007213e+01 8.554745e+01 0.000000e+00
      vertex   7.890000e+01 8.565000e+01 0.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 0.000000e+00
      vertex   7.772787e+01 8.554745e+01 0.000000e+00
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 8.890000e+01 0.000000e+00
      vertex   7.772787e+01 8.554745e+01 0.000000e+00
      vertex   7.659136e+01 8.524293e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   -8.890000e+01 -8.890000e+01 0.000000e+00
      vertex   2.300000e+00 -7.845000e+01 0.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   7.215000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   7.305433e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   7.552500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   7.890000e+01 -7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   8.227500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   8.474567e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   8.565000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   8.474567e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   8.227500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   7.890000e+01 -8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   7.552500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   7.305433e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   7.640000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   7.215000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -8.743077e-16
    outer loop
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -8.203445e-16
    outer loop
      vertex   7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   7.305433e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   7.552500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   7.890000e+01 -8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   8.227500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   8.474567e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   8.565000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   8.474567e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   8.227500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   7.890000e+01 -7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   7.552500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   7.305433e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 8.203445e-16
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 8.743077e-16
    outer loop
      vertex   7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   7.225255e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
      vertex   7.215000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
      vertex   7.215000e+01 7.890000e+01 3.540000e+01
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
      vertex   7.305433e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
      vertex   7.552500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
      vertex   7.890000e+01 8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
      vertex   8.227500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
      vertex   8.474567e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
      vertex   8.565000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
      vertex   8.474567e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
      vertex   8.227500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
      vertex   7.890000e+01 7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
      vertex   7.552500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
      vertex   7.305433e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 3.540000e+01
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 3.540000e+01
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 3.540000e+01
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 3.540000e+01
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 3.540000e+01
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 3.540000e+01
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 3.540000e+01
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 3.540000e+01
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 3.540000e+01
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 3.540000e+01
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 3.540000e+01
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 3.540000e+01
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 3.540000e+01
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 4.000000e+00
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.954705e+01 2.493333e+01
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
      vertex   7.648519e+01 7.954705e+01 1.446667e+01
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 8.015000e+01 2.493333e+01
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
      vertex   7.673494e+01 8.015000e+01 1.446667e+01
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 8.066777e+01 2.493333e+01
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
      vertex   7.713223e+01 8.066777e+01 1.446667e+01
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 8.106506e+01 2.493333e+01
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
      vertex   7.765000e+01 8.106506e+01 1.446667e+01
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 8.131481e+01 2.493333e+01
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
      vertex   7.825295e+01 8.131481e+01 1.446667e+01
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 8.140000e+01 2.493333e+01
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
      vertex   7.890000e+01 8.140000e+01 1.446667e+01
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   7.954705e+01 8.131481e+01 2.493333e+01
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
      vertex   7.954705e+01 8.131481e+01 1.446667e+01
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.015000e+01 8.106506e+01 2.493333e+01
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
      vertex   8.015000e+01 8.106506e+01 1.446667e+01
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.066777e+01 8.066777e+01 2.493333e+01
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
      vertex   8.066777e+01 8.066777e+01 1.446667e+01
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.106506e+01 8.015000e+01 2.493333e+01
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
      vertex   8.106506e+01 8.015000e+01 1.446667e+01
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.131481e+01 7.954705e+01 2.493333e+01
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
      vertex   8.131481e+01 7.954705e+01 1.446667e+01
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.140000e+01 7.890000e+01 2.493333e+01
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
      vertex   8.140000e+01 7.890000e+01 1.446667e+01
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.131481e+01 7.825295e+01 2.493333e+01
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
      vertex   8.131481e+01 7.825295e+01 1.446667e+01
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.106506e+01 7.765000e+01 2.493333e+01
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
      vertex   8.106506e+01 7.765000e+01 1.446667e+01
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.066777e+01 7.713223e+01 2.493333e+01
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
      vertex   8.066777e+01 7.713223e+01 1.446667e+01
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   8.015000e+01 7.673494e+01 2.493333e+01
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
      vertex   8.015000e+01 7.673494e+01 1.446667e+01
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.954705e+01 7.648519e+01 2.493333e+01
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
      vertex   7.954705e+01 7.648519e+01 1.446667e+01
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.640000e+01 2.493333e+01
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
      vertex   7.890000e+01 7.640000e+01 1.446667e+01
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.825295e+01 7.648519e+01 2.493333e+01
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
      vertex   7.825295e+01 7.648519e+01 1.446667e+01
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.765000e+01 7.673494e+01 2.493333e+01
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
      vertex   7.765000e+01 7.673494e+01 1.446667e+01
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.713223e+01 7.713223e+01 2.493333e+01
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
      vertex   7.713223e+01 7.713223e+01 1.446667e+01
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.673494e+01 7.765000e+01 2.493333e+01
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
      vertex   7.673494e+01 7.765000e+01 1.446667e+01
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.648519e+01 7.825295e+01 2.493333e+01
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   7.640000e+01 7.890000e+01 2.493333e+01
      vertex   7.648519e+01 7.825295e+01 1.446667e+01
      vertex   7.640000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
      vertex   7.215000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 3.540000e+01
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 3.540000e+01
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 3.540000e+01
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -8.743077e-16
    outer loop
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -8.203445e-16
    outer loop
      vertex   7.255707e+01 7.659136e+01 3.540000e+01
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
      vertex   7.305433e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 3.540000e+01
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 3.540000e+01
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 3.540000e+01
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 3.540000e+01
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
      vertex   7.552500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 3.540000e+01
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 3.540000e+01
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 3.540000e+01
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 3.540000e+01
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
      vertex   7.890000e+01 7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 3.540000e+01
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 3.540000e+01
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 3.540000e+01
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 -8.743077e-16
    outer loop
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 -8.203445e-16
    outer loop
      vertex   8.120864e+01 7.255707e+01 3.540000e+01
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
      vertex   8.227500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 3.540000e+01
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 3.540000e+01
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 3.540000e+01
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 3.540000e+01
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
      vertex   8.474567e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 3.540000e+01
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 3.540000e+01
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 3.540000e+01
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 3.540000e+01
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
      vertex   8.565000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 3.540000e+01
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 3.540000e+01
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 3.540000e+01
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 3.540000e+01
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
      vertex   8.474567e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 3.540000e+01
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 3.540000e+01
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 3.540000e+01
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 3.540000e+01
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
      vertex   8.227500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 3.540000e+01
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 3.540000e+01
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 3.540000e+01
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 3.540000e+01
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
      vertex   7.890000e+01 8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 3.540000e+01
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 3.540000e+01
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 3.540000e+01
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 3.540000e+01
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
      vertex   7.552500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 3.540000e+01
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 3.540000e+01
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 3.540000e+01
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 3.540000e+01
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
      vertex   7.305433e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 3.540000e+01
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 3.540000e+01
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 3.540000e+01
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 3.540000e+01
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.215000e+01 7.890000e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.215000e+01 7.890000e+01 1.970000e+01
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 8.007213e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.225255e+01 8.007213e+01 1.970000e+01
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.255707e+01 8.120864e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.255707e+01 8.120864e+01 1.970000e+01
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 8.227500e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.305433e+01 8.227500e+01 1.970000e+01
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 8.323882e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.372920e+01 8.323882e+01 1.970000e+01
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 8.407080e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.456118e+01 8.407080e+01 1.970000e+01
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 8.474567e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.552500e+01 8.474567e+01 1.970000e+01
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 8.524293e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.659136e+01 8.524293e+01 1.970000e+01
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 8.554745e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.772787e+01 8.554745e+01 1.970000e+01
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   7.890000e+01 8.565000e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   7.890000e+01 8.565000e+01 1.970000e+01
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.007213e+01 8.554745e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.007213e+01 8.554745e+01 1.970000e+01
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.120864e+01 8.524293e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.120864e+01 8.524293e+01 1.970000e+01
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.227500e+01 8.474567e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.227500e+01 8.474567e+01 1.970000e+01
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.323882e+01 8.407080e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.323882e+01 8.407080e+01 1.970000e+01
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.407080e+01 8.323882e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.407080e+01 8.323882e+01 1.970000e+01
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.474567e+01 8.227500e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.474567e+01 8.227500e+01 1.970000e+01
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.524293e+01 8.120864e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.524293e+01 8.120864e+01 1.970000e+01
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.554745e+01 8.007213e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.554745e+01 8.007213e+01 1.970000e+01
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.565000e+01 7.890000e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.565000e+01 7.890000e+01 1.970000e+01
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.554745e+01 7.772787e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.554745e+01 7.772787e+01 1.970000e+01
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.524293e+01 7.659136e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.524293e+01 7.659136e+01 1.970000e+01
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.474567e+01 7.552500e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.474567e+01 7.552500e+01 1.970000e+01
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.407080e+01 7.456118e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.407080e+01 7.456118e+01 1.970000e+01
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.323882e+01 7.372920e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.323882e+01 7.372920e+01 1.970000e+01
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   8.227500e+01 7.305433e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 8.203445e-16
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.227500e+01 7.305433e+01 1.970000e+01
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 8.743077e-16
    outer loop
      vertex   8.120864e+01 7.255707e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   8.120864e+01 7.255707e+01 1.970000e+01
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   8.007213e+01 7.225255e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   8.007213e+01 7.225255e+01 1.970000e+01
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.890000e+01 7.215000e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.890000e+01 7.215000e+01 1.970000e+01
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.772787e+01 7.225255e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.772787e+01 7.225255e+01 1.970000e+01
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.659136e+01 7.255707e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.659136e+01 7.255707e+01 1.970000e+01
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.552500e+01 7.305433e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.552500e+01 7.305433e+01 1.970000e+01
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.456118e+01 7.372920e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.456118e+01 7.372920e+01 1.970000e+01
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.372920e+01 7.456118e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.372920e+01 7.456118e+01 1.970000e+01
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   7.305433e+01 7.552500e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 8.203445e-16
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.305433e+01 7.552500e+01 1.970000e+01
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 8.743077e-16
    outer loop
      vertex   7.255707e+01 7.659136e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   7.225255e+01 7.772787e+01 4.000000e+00
      vertex   7.255707e+01 7.659136e+01 1.970000e+01
      vertex   7.225255e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   -8.565000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   -8.474567e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   -8.227500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   -7.890000e+01 -7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.552500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   -7.305433e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.215000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   -7.305433e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.552500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   -7.890000e+01 -8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   -8.227500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   -8.474567e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 3.540000e+01
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 3.540000e+01
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 3.540000e+01
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 3.540000e+01
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 3.540000e+01
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 3.540000e+01
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 3.540000e+01
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 3.540000e+01
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 3.540000e+01
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 3.540000e+01
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 3.540000e+01
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 3.540000e+01
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 4.000000e+00
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.825295e+01 2.493333e+01
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.825295e+01 1.446667e+01
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -7.765000e+01 2.493333e+01
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   -8.106506e+01 -7.765000e+01 1.446667e+01
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -7.713223e+01 2.493333e+01
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   -8.066777e+01 -7.713223e+01 1.446667e+01
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -7.673494e+01 2.493333e+01
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   -8.015000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -7.648519e+01 2.493333e+01
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   -7.954705e+01 -7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.640000e+01 2.493333e+01
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 -7.640000e+01 1.446667e+01
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 -7.648519e+01 2.493333e+01
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.825295e+01 -7.648519e+01 1.446667e+01
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 -7.673494e+01 2.493333e+01
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   -7.765000e+01 -7.673494e+01 1.446667e+01
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 -7.713223e+01 2.493333e+01
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   -7.713223e+01 -7.713223e+01 1.446667e+01
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 -7.765000e+01 2.493333e+01
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   -7.673494e+01 -7.765000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.825295e+01 2.493333e+01
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.825295e+01 1.446667e+01
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 -7.890000e+01 2.493333e+01
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   -7.640000e+01 -7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 -7.954705e+01 2.493333e+01
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   -7.648519e+01 -7.954705e+01 1.446667e+01
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 -8.015000e+01 2.493333e+01
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   -7.673494e+01 -8.015000e+01 1.446667e+01
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 -8.066777e+01 2.493333e+01
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   -7.713223e+01 -8.066777e+01 1.446667e+01
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 -8.106506e+01 2.493333e+01
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   -7.765000e+01 -8.106506e+01 1.446667e+01
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 -8.131481e+01 2.493333e+01
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   -7.825295e+01 -8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.140000e+01 2.493333e+01
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 -8.140000e+01 1.446667e+01
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 -8.131481e+01 2.493333e+01
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   -7.954705e+01 -8.131481e+01 1.446667e+01
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 -8.106506e+01 2.493333e+01
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   -8.015000e+01 -8.106506e+01 1.446667e+01
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 -8.066777e+01 2.493333e+01
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   -8.066777e+01 -8.066777e+01 1.446667e+01
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 -8.015000e+01 2.493333e+01
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   -8.106506e+01 -8.015000e+01 1.446667e+01
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 -7.954705e+01 2.493333e+01
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 -7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 -7.954705e+01 1.446667e+01
      vertex   -8.140000e+01 -7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   -8.565000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 3.540000e+01
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 3.540000e+01
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 3.540000e+01
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   -8.474567e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 3.540000e+01
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 3.540000e+01
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 3.540000e+01
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   -8.227500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 3.540000e+01
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 3.540000e+01
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 3.540000e+01
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 -8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 3.540000e+01
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 3.540000e+01
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 3.540000e+01
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   -7.552500e+01 -8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 3.540000e+01
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 3.540000e+01
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 3.540000e+01
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   -7.305433e+01 -8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 3.540000e+01
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 -8.203445e-16
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 -8.743077e-16
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 3.540000e+01
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 3.540000e+01
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   -7.215000e+01 -7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 3.540000e+01
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 3.540000e+01
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 3.540000e+01
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   -7.305433e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 3.540000e+01
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 3.540000e+01
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 3.540000e+01
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   -7.552500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 3.540000e+01
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 3.540000e+01
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 3.540000e+01
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 -7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 3.540000e+01
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 3.540000e+01
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 3.540000e+01
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   -8.227500e+01 -7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 3.540000e+01
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 -7.414555e-16
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 -6.400377e-16
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 3.540000e+01
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 3.540000e+01
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   -8.474567e+01 -7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 3.540000e+01
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 3.540000e+01
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 3.540000e+01
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 -7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.565000e+01 -7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -7.772787e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.554745e+01 -7.772787e+01 1.970000e+01
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -7.659136e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.524293e+01 -7.659136e+01 1.970000e+01
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -7.552500e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.474567e+01 -7.552500e+01 1.970000e+01
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -7.456118e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 6.400377e-16
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.407080e+01 -7.456118e+01 1.970000e+01
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 7.414555e-16
    outer loop
      vertex   -8.323882e+01 -7.372920e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.323882e+01 -7.372920e+01 1.970000e+01
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -7.305433e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.227500e+01 -7.305433e+01 1.970000e+01
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -7.255707e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -8.120864e+01 -7.255707e+01 1.970000e+01
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -7.225255e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -8.007213e+01 -7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 -7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 -7.215000e+01 1.970000e+01
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 -7.225255e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.772787e+01 -7.225255e+01 1.970000e+01
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 -7.255707e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.659136e+01 -7.255707e+01 1.970000e+01
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 -7.305433e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.552500e+01 -7.305433e+01 1.970000e+01
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 -7.372920e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.456118e+01 -7.372920e+01 1.970000e+01
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 -7.456118e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.372920e+01 -7.456118e+01 1.970000e+01
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 -7.552500e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.305433e+01 -7.552500e+01 1.970000e+01
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.255707e+01 -7.659136e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.255707e+01 -7.659136e+01 1.970000e+01
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 -7.772787e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 -7.772787e+01 1.970000e+01
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 -7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.215000e+01 -7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 -8.007213e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 8.743077e-16
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.225255e+01 -8.007213e+01 1.970000e+01
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 8.203445e-16
    outer loop
      vertex   -7.255707e+01 -8.120864e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.255707e+01 -8.120864e+01 1.970000e+01
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 -8.227500e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.305433e+01 -8.227500e+01 1.970000e+01
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 -8.323882e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.372920e+01 -8.323882e+01 1.970000e+01
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 -8.407080e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.456118e+01 -8.407080e+01 1.970000e+01
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 -8.474567e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.552500e+01 -8.474567e+01 1.970000e+01
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 -8.524293e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.659136e+01 -8.524293e+01 1.970000e+01
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 -8.554745e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.772787e+01 -8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 -8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 -8.565000e+01 1.970000e+01
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 -8.554745e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.007213e+01 -8.554745e+01 1.970000e+01
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 -8.524293e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.120864e+01 -8.524293e+01 1.970000e+01
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 -8.474567e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.227500e+01 -8.474567e+01 1.970000e+01
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 -8.407080e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.323882e+01 -8.407080e+01 1.970000e+01
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 -8.323882e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.407080e+01 -8.323882e+01 1.970000e+01
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 -8.227500e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.474567e+01 -8.227500e+01 1.970000e+01
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 -8.120864e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 -8.007213e+01 4.000000e+00
      vertex   -8.524293e+01 -8.120864e+01 1.970000e+01
      vertex   -8.554745e+01 -8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
      vertex   -8.565000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
      vertex   -8.565000e+01 7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
      vertex   -8.474567e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
      vertex   -8.227500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
      vertex   -7.890000e+01 8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
      vertex   -7.552500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
      vertex   -7.305433e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
      vertex   -7.215000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
      vertex   -7.305433e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
      vertex   -7.552500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
      vertex   -7.890000e+01 7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
      vertex   -8.227500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
      vertex   -8.474567e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 1.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 -0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 0.000000e+00 0.000000e+00 -1.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 3.540000e+01
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 3.540000e+01
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 3.540000e+01
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 3.540000e+01
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 3.540000e+01
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 3.540000e+01
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 3.540000e+01
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 3.540000e+01
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 3.540000e+01
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 3.540000e+01
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 3.540000e+01
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 3.540000e+01
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 3.540000e+01
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 4.000000e+00
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 -1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.954705e+01 2.493333e+01
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 -3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
      vertex   -8.131481e+01 7.954705e+01 1.446667e+01
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 8.015000e+01 2.493333e+01
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 -6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
      vertex   -8.106506e+01 8.015000e+01 1.446667e+01
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 8.066777e+01 2.493333e+01
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 -7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
      vertex   -8.066777e+01 8.066777e+01 1.446667e+01
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 8.106506e+01 2.493333e+01
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 -9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
      vertex   -8.015000e+01 8.106506e+01 1.446667e+01
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 8.131481e+01 2.493333e+01
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 -9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
      vertex   -7.954705e+01 8.131481e+01 1.446667e+01
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.140000e+01 2.493333e+01
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 -9.914449e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
      vertex   -7.890000e+01 8.140000e+01 1.446667e+01
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.825295e+01 8.131481e+01 2.493333e+01
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 -9.238795e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
      vertex   -7.825295e+01 8.131481e+01 1.446667e+01
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.765000e+01 8.106506e+01 2.493333e+01
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 -7.933533e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
      vertex   -7.765000e+01 8.106506e+01 1.446667e+01
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.713223e+01 8.066777e+01 2.493333e+01
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 -6.087614e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
      vertex   -7.713223e+01 8.066777e+01 1.446667e+01
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.673494e+01 8.015000e+01 2.493333e+01
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 -3.826834e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
      vertex   -7.673494e+01 8.015000e+01 1.446667e+01
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.954705e+01 2.493333e+01
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 -1.305262e-01 -0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
      vertex   -7.648519e+01 7.954705e+01 1.446667e+01
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.640000e+01 7.890000e+01 2.493333e+01
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
      vertex   -7.640000e+01 7.890000e+01 1.446667e+01
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.648519e+01 7.825295e+01 2.493333e+01
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
      vertex   -7.648519e+01 7.825295e+01 1.446667e+01
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.673494e+01 7.765000e+01 2.493333e+01
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal -7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
      vertex   -7.673494e+01 7.765000e+01 1.446667e+01
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.713223e+01 7.713223e+01 2.493333e+01
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal -6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
      vertex   -7.713223e+01 7.713223e+01 1.446667e+01
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.765000e+01 7.673494e+01 2.493333e+01
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal -3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
      vertex   -7.765000e+01 7.673494e+01 1.446667e+01
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.825295e+01 7.648519e+01 2.493333e+01
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
    endloop
  endfacet
  facet normal -1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
      vertex   -7.825295e+01 7.648519e+01 1.446667e+01
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.640000e+01 2.493333e+01
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
    endloop
  endfacet
  facet normal 1.305262e-01 9.914449e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
      vertex   -7.890000e+01 7.640000e+01 1.446667e+01
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -7.954705e+01 7.648519e+01 2.493333e+01
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
    endloop
  endfacet
  facet normal 3.826834e-01 9.238795e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
      vertex   -7.954705e+01 7.648519e+01 1.446667e+01
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.015000e+01 7.673494e+01 2.493333e+01
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
    endloop
  endfacet
  facet normal 6.087614e-01 7.933533e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
      vertex   -8.015000e+01 7.673494e+01 1.446667e+01
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.066777e+01 7.713223e+01 2.493333e+01
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 7.933533e-01 6.087614e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
      vertex   -8.066777e+01 7.713223e+01 1.446667e+01
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.106506e+01 7.765000e+01 2.493333e+01
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.238795e-01 3.826834e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
      vertex   -8.106506e+01 7.765000e+01 1.446667e+01
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.131481e+01 7.825295e+01 2.493333e+01
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
    endloop
  endfacet
  facet normal 9.914449e-01 1.305262e-01 0.000000e+00
    outer loop
      vertex   -8.140000e+01 7.890000e+01 2.493333e+01
      vertex   -8.131481e+01 7.825295e+01 1.446667e+01
      vertex   -8.140000e+01 7.890000e+01 1.446667e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
      vertex   -8.565000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 3.540000e+01
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 3.540000e+01
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 -0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 3.540000e+01
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 -0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 3.540000e+01
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
      vertex   -8.474567e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 -0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 3.540000e+01
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 3.540000e+01
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 -0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 3.540000e+01
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 -6.400377e-16
    outer loop
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -7.414555e-16
    outer loop
      vertex   -8.323882e+01 7.372920e+01 3.540000e+01
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
      vertex   -8.227500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 -0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 3.540000e+01
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 3.540000e+01
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 -8.203445e-16
    outer loop
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -8.743077e-16
    outer loop
      vertex   -8.120864e+01 7.255707e+01 3.540000e+01
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 -0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 3.540000e+01
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 7.215000e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 3.540000e+01
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 3.540000e+01
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 3.540000e+01
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 3.540000e+01
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
      vertex   -7.552500e+01 7.305433e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 3.540000e+01
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 3.540000e+01
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 3.540000e+01
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 3.540000e+01
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
      vertex   -7.305433e+01 7.552500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 3.540000e+01
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 3.540000e+01
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 3.540000e+01
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 3.540000e+01
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
      vertex   -7.215000e+01 7.890000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 3.540000e+01
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 3.540000e+01
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 3.540000e+01
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -8.743077e-16
    outer loop
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -8.203445e-16
    outer loop
      vertex   -7.255707e+01 8.120864e+01 3.540000e+01
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
      vertex   -7.305433e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 3.540000e+01
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 3.540000e+01
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 3.540000e+01
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 3.540000e+01
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
      vertex   -7.552500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 3.540000e+01
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 3.540000e+01
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 3.540000e+01
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 3.540000e+01
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 8.565000e+01 3.540000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 3.540000e+01
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 3.540000e+01
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 3.540000e+01
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 3.540000e+01
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
      vertex   -8.227500e+01 8.474567e+01 3.540000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 3.540000e+01
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 3.540000e+01
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 3.540000e+01
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 3.540000e+01
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
      vertex   -8.474567e+01 8.227500e+01 3.540000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 3.540000e+01
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 3.540000e+01
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 3.540000e+01
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 3.540000e+01
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.565000e+01 7.890000e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.961947e-01 8.715574e-02 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.565000e+01 7.890000e+01 1.970000e+01
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 8.007213e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.554745e+01 8.007213e+01 1.970000e+01
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 8.120864e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.524293e+01 8.120864e+01 1.970000e+01
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 8.227500e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.474567e+01 8.227500e+01 1.970000e+01
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 8.323882e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.407080e+01 8.323882e+01 1.970000e+01
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.323882e+01 8.407080e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.323882e+01 8.407080e+01 1.970000e+01
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 8.474567e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.227500e+01 8.474567e+01 1.970000e+01
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.120864e+01 8.524293e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -8.120864e+01 8.524293e+01 1.970000e+01
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 8.554745e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -8.007213e+01 8.554745e+01 1.970000e+01
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.890000e+01 8.565000e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 9.961947e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.890000e+01 8.565000e+01 1.970000e+01
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.772787e+01 8.554745e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 9.659258e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.772787e+01 8.554745e+01 1.970000e+01
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.659136e+01 8.524293e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 9.063078e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.659136e+01 8.524293e+01 1.970000e+01
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.552500e+01 8.474567e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 8.191520e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.552500e+01 8.474567e+01 1.970000e+01
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.456118e+01 8.407080e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 7.071068e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.456118e+01 8.407080e+01 1.970000e+01
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.372920e+01 8.323882e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 5.735764e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.372920e+01 8.323882e+01 1.970000e+01
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 -0.000000e+00
    outer loop
      vertex   -7.305433e+01 8.227500e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 4.226183e-01 8.203445e-16
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.305433e+01 8.227500e+01 1.970000e+01
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 8.743077e-16
    outer loop
      vertex   -7.255707e+01 8.120864e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 2.588190e-01 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.255707e+01 8.120864e+01 1.970000e+01
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.225255e+01 8.007213e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 8.715574e-02 -0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.225255e+01 8.007213e+01 1.970000e+01
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.215000e+01 7.890000e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.961947e-01 -8.715574e-02 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.215000e+01 7.890000e+01 1.970000e+01
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.225255e+01 7.772787e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.225255e+01 7.772787e+01 1.970000e+01
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.255707e+01 7.659136e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal 9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.255707e+01 7.659136e+01 1.970000e+01
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.305433e+01 7.552500e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.305433e+01 7.552500e+01 1.970000e+01
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.372920e+01 7.456118e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal 7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.372920e+01 7.456118e+01 1.970000e+01
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.456118e+01 7.372920e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal 5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.456118e+01 7.372920e+01 1.970000e+01
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.552500e+01 7.305433e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal 4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.552500e+01 7.305433e+01 1.970000e+01
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.659136e+01 7.255707e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal 2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.659136e+01 7.255707e+01 1.970000e+01
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.772787e+01 7.225255e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
    endloop
  endfacet
  facet normal 8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.772787e+01 7.225255e+01 1.970000e+01
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -7.890000e+01 7.215000e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.715574e-02 -9.961947e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -7.890000e+01 7.215000e+01 1.970000e+01
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 0.000000e+00
    outer loop
      vertex   -8.007213e+01 7.225255e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
    endloop
  endfacet
  facet normal -2.588190e-01 -9.659258e-01 8.743077e-16
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.007213e+01 7.225255e+01 1.970000e+01
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 8.203445e-16
    outer loop
      vertex   -8.120864e+01 7.255707e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
    endloop
  endfacet
  facet normal -4.226183e-01 -9.063078e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.120864e+01 7.255707e+01 1.970000e+01
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 0.000000e+00
    outer loop
      vertex   -8.227500e+01 7.305433e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
    endloop
  endfacet
  facet normal -5.735764e-01 -8.191520e-01 7.414555e-16
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.227500e+01 7.305433e+01 1.970000e+01
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 6.400377e-16
    outer loop
      vertex   -8.323882e+01 7.372920e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
    endloop
  endfacet
  facet normal -7.071068e-01 -7.071068e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.323882e+01 7.372920e+01 1.970000e+01
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.407080e+01 7.456118e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
    endloop
  endfacet
  facet normal -8.191520e-01 -5.735764e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.407080e+01 7.456118e+01 1.970000e+01
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.474567e+01 7.552500e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.063078e-01 -4.226183e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.474567e+01 7.552500e+01 1.970000e+01
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.524293e+01 7.659136e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
    endloop
  endfacet
  facet normal -9.659258e-01 -2.588190e-01 0.000000e+00
    outer loop
      vertex   -8.554745e+01 7.772787e+01 4.000000e+00
      vertex   -8.524293e+01 7.659136e+01 1.970000e+01
      vertex   -8.554745e+01 7.772787e+01 1.970000e+01
    endloop
  endfacet
endsolid

```

- [Audodesk Fusion 360 file](case/pov-clock-case.f3d)
- [STL file](case/pov-clock-case.stl)
- [FBX file](case/pov-clock-case.fbx)

## 6. Power Supply <a name="powersupply"></a>
## 7. Schematics <a name="schematics"></a>
### ESC Driver Schematic<a name="escschematic"></a>
<div>
    <img src = "images/esc-driver-circuit-schematic.JPG">
</div>

### Clock Schematic<a name="clockschematic"></a>
<div>
    <img src = "images/led-circuit-schematic.jpg">
</div>



















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
