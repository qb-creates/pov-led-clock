# PovLedClock
Created a fully functional analog style pov(persistence of vision) clock. Persistence of vision displays are generally LED displays which 'shows' images by displaying a section of an image at a given time, in quick rapid succession. The human brain perceives this as a continuous image. I accomplish this by attaching my microcontroller board and LEDs to the motor of a hard drive. A hard drive motor was used by it is a three phase motor and it is also spins fast enough to make the image look complete and not have any stutters. An IR remote was used to set the time on the clock upon power up. A PWM signal made from a 555 timer was used to drive a speed controller which controls the speed of our motor. This project was designed, built, and programmed by me.

What I learned
* Learned more about 3 phase motors.
* Learned about infrared signals.
* Learned how to create a random number generator using a timer and the unpredictability of player input. 

<h2>Video Demonstrations</h2> 
<a href="https://www.youtube.com/watch?v=ZK3UpSKuKRA" target="blank" style="text-decoration:none;">    

<h2>Images</h2>
<div>
    <img src = "images/pov-led-clock.JPEG" width = "410" height = "500" style="padding: 0; margin: 0;">
    <img src = "images/case-inside.JPEG" width = "410" height = "500">
</div>
<div>
    <img src = "images/led-circuit-front.JPEG" width = "202.5" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/led-circuit-back.JPEG" width = "202.5" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/attiny-circuit-front.JPEG" width = "202.5" height = "270" style="padding: 0; margin: 0;">
    <img src = "images/attiny-circuit-back.JPEG" width = "202.5" height = "270" style="padding: 0; margin: 0;">
</div>

<h2>Schematic</h2>
<div>
    <img src = "images/led-circuit-schematic.jpg">
</div>