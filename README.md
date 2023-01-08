# Maturaprojekt
A machine that uses image processing to find magnetic objects and automatically remove them from an area

The magnetic objects might look something like this, they are iron washers with a piece of coloured paper attached to them: <br />
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/IMG-20220521-WA0000.jpg" width=350>

First, we use the DroidCam app to send an image of this area to a desktop software, which processes it to find the locations of the objects: <br />
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/IMG-20220521-WA0001.jpg" width=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/IMG-20220521-WA0003.jpg" width=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/IMG-20220521-WA0004.jpg" width=200>

The image processing is quite the procedure, but the most important steps look like this: <br />
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Presentation/input.PNG" height=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Presentation/canny.PNG" height=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Presentation/canny-corners.png" height=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Presentation/input-perspective.png" height=200>
<br />

The area that has been found on the image is perspective-transformed into an upright square with side length 256, and the coordinates of the metal objects within this square are sent to a microcontroller. The X and Y coordinates of the objects use 1 byte each, as more precision is unnecessary for a small area like this, and makes data transfer using UART very easy. Upon receiving the coordinates of the objects, the microcontroller (Atmega328p) starts controlling 2 stepper motors to approach each object individually and pick it up using an electromagnet. For this, we build a machine that looks like this: <br />
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/Bild1.png" width=350>

We made a circuit diagram containing a microcontroller, 2 TMC2226 stepper drivers on custom PCBs that we designed and an electromagnet. Then we designed a PCB for it and ordered it from china: <br /> 
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/Schaltung.PNG" height=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/copperbottom.png" height=200>
<img src="https://github.com/BetterRage/Maturaprojekt/blob/master/Bilder/coppertop.PNG" height=200>
