# Signal Encoder 
This is a program written in C. It encodes a string of binary numbers in a file with 5 different encoding schemes.
Encoding schemes are as follows: NRZ-L, NRZ-I, B-AMI, Manchester, and Differential Manchester.

| ![signal0](http://i.imgur.com/Q4T0uj4.png) 
| --- |
| **Digital Signal Encoding Formats.** |

## How to use 
In order to use the program, travel to the directory: `dist/Debug/MinGW-Windows/`  
Open the executable using the command prompt when you have built the project. 
Write the following on the command prompt: `executable.exe myInputData.txt 0`  
Note: The number 0 can be replaced with 4 other numbers. The number corresponds to the type of encoding you wish to do.
Also, the file containing the binary numbers you wish to encode must all be either binary 0 or 1.

Here are the numbers that relate to its encoding: 

0. NRZ-L
1. NRZ-I
2. B-AMI
3. Manchester
4. Differential Manchester

## Output
The output file will be named `OUTPUT.SIGNAL`.   
The output file will look as follows for NRZ-L with binary numbers 0110100: `|-V|+V|+V|-V|+V|-V|-V|`, where `|+V|` means HIGH and `|-V|` means LOW.  
Depending on the encoding, you might see: `|+V-V|`, which means HIGH to LOW, `|-V+V|` means LOW to HIGH, and `|0V|` means 0.
