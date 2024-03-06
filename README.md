# MBedOS_LED_Sequencer

This project implements a basic LED sequencer program for an mbed microcontroller. It allows you to control an LED (defaulting to LED1) by sending commands through the serial port.

Features
Control an LED by sending sequences of characters over serial communication.
Change the blinking frequency of the LED.
Pre-defined example sequences are included in the code.
Hardware Requirements
mbed microcontroller board (any board with a serial port and an LED)
Serial cable to connect the microcontroller to your computer
Software Requirements
mbed compiler (https://os.mbed.com/docs/mbed-os/v6.9/build-tools/mbed-online-compiler.html)
Using the Project
Download the code: Copy the code provided in this repository or obtain it from another source.
Compile the code: Use the mbed compiler to compile the code for your specific mbed platform. Refer to the mbed documentation for compilation instructions (https://os.mbed.com/docs/mbed-studio/current/getting-started/index.html).
Connect the hardware: Connect the mbed board to your computer using the serial cable.
Open a serial terminal: Use a serial terminal application like PuTTY or the mbed Serial Monitor to connect to the board's serial port. The default baud rate is typically 115200.
Send commands:
To start a new sequence, send a message starting with a + symbol followed by the desired sequence of characters. The characters in the sequence will be interpreted as binary values, controlling the on/off state of the LED based on the rising and falling edges of the bits.
To change the blinking frequency, send a message starting with a * symbol followed by a number representing the desired frequency in Hz (e.g., *1.0 for 1 Hz).
Example Usage
Send the message +102040 to turn on the LED for a short duration, then off for a longer duration, repeated in a sequence.
Send the message *0.5 to change the blinking frequency to half a second (2 Hz).
Code Structure
The code consists of several main parts:

Initialization: Sets up the serial communication, LEDs, timers, and pre-defined sequences.
Main Loop: Listens for serial activity and processes received messages.
Parses messages starting with + to control the LED based on the sequence.
Updates the blinking frequency based on messages starting with *.
Loops through the current sequence and controls the LED based on the binary representation of each character.
char_to_bin function: Converts a character to its 8-bit binary representation.
led_on function: Takes an LED object and a character as input, converts it to binary, and controls the LED based on the rising and falling edges of the bits in the binary representation.
Additional Notes
This is a basic example and can be extended to support more features, such as controlling multiple LEDs, using different pre-defined sequences, and implementing error handling for invalid commands.
The code uses a static variable prev_bit in the led_on function to store the previous bit value for comparison across iterations within a sequence.