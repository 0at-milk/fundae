A short joystick controlled game using an esp32.

Materials used: power source, arduino ESP32, joystick, breadboard and button (not pictured).

This work is free to be reproduced, replicate, or be edited. 

In order to test this code you may follow the documentation linked in this file, which contains images and a short walk-through. You can download this code, and run it through the arduino IDE once your hardware has been properly connected. once the coad is loaded onto the esp32, you may begin running the processing code.

The code was made possible by using a ball game source code linked here: http://processing.flosscience.com/processing-for-android/macul-2012/simple-interaction.

I adjusted the code to account for movement in the joystick, as well as a button press (i.e the joystick press).


The main purpose of the functions listed in the code is to receive information from the hardware, and turn it into input that is used in draw(). setup() obtains the information, and sets up the UI specific dimentions. Draw() itself draws the ball and displays messages (helped functions are also used), and subsequenly takes in the input from setup to create visual change by altering the ball itself.

More code specifics can be explored by examining the functions.
