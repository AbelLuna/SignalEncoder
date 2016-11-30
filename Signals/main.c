/*
 ********************************Signal Encoder*******************************
 * 
 * The purpose of this program is to encode a string of binary numbers, to 
 * NRZ-L, NRZ-I, B-AMI, Manchester, and Differential Manchester.
 * 
 ***************************************************************************** 
 */

/* 
 * File:   main.c
 * Author: Abel Luna
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct A {
    unsigned char fileName[256]; //Name of the file to get binary code from.
    unsigned char scheme; //The encoding scheme that will be used.
    unsigned char code[100]; //The string of numbers in the text file.
};
typedef struct A Command;
FILE *fp, *fOUT;

/*
 * @param err This is the number that will correspond with the error displayed.
 * 
 * Will print an error to the console that corresponds to the number provided.
 */
void errorLog(unsigned char err) {
    switch (err) {
        case 0: puts("Error: Scheme not recognized.");
            break;
        case 1: puts("Error: Not enough parameters entered.");
            break;
        case 2: puts("Error: Too many parameters entered.");
            break;
        case 3: puts("Error: File not found. Make sure extension is written.");
            break;
        case 4: puts("Error: Data is not in binary format.");
            break;
        case 5: puts("Output file not written.");
            break;
        default: puts("Error: Unexpected error occurred.");
    }
}

/**
 * 
 * @param c This is the string of characters that is to be checked for binary format.
 * @return Returns 1 if all the chars in the char array are either 0 or 1, else it returns 0.
 * 
 * Checks a char array for binary format.
 */
unsigned char isBinary(char* c) {
    unsigned int i;
    for (i = 0; i < (strlen(c)); i++) {
        if (c[i] != '1' && c[i] != '0') {
            return 0;
        }
    }
    return 1;
}

/**
 * @param c The struct where members filename, and code of type char* are present.
 * @return Returns 1 if the file was successfully opened and created an output file, else returns 0.
 * 
 * Opens the file with the specified file name and creates an output file.
 * It then reads a single line from the file and places it inside the struct.
 */
unsigned char openFile(Command* c) {
    if ((fp = fopen(c->fileName, "r")) != NULL) {
        char code[100];
        fgets(code, sizeof code, fp);
        fclose(fp); //Will only read the first line of file.
        sscanf(code, "%s", code);
        if (isBinary(code)) {
            fOUT = fopen("OUTPUT.SIGNAL", "w");
            strncpy(c->code, code, sizeof code);
            return 1;
        } else {
            errorLog(4);
            return 0;
        }
    } else {
        errorLog(3);
        return 0;
    }
}

/**
 * 
 * @param c A char representing what to write to the file.
 * 
 * Will write "0V", "+V|", or "-V|" to the file in fOUT depending on what was 
 * specified by char c.
 */
void encode(char c) {
    if (c == '0') {
        fputs("0V|", fOUT);
    } else if (c == '+') {
        fputs("+V|", fOUT);
    } else if (c == '-') {
        fputs("-V|", fOUT);
    }
}

/**
 * @param c struct that contains the line of binary numbers to be encoded.
 * 
 * Will encode the signal, represented by binary numbers, in NRZ-L.
 */
void NRZ_L(Command* c) {
    if (openFile(c)) {
        fputs("|", fOUT); //To start the output with |
        unsigned int i;
        for (i = 0; i < strlen(c->code); i++) {
            if (c->code[i] == '0') encode('+');
            else encode('-');
        }
        fclose(fOUT);
        puts("Done.");
    } else {
        errorLog(5);
    }
}

/**
 * 
 * @param c struct that contains the line of binary numbers to be encoded.
 * 
 * Will encode the signal, represented by binary numbers, in NRZ-I. 
 */
void NRZ_I(Command* c) {
    if (openFile(c)) {
        fputs("|", fOUT); //To start the output with |
        unsigned int i;
        unsigned char prevBit;
        if (c->code[0] == '0') {
            encode('-'); //starting low
            prevBit = '-';
        } else {
            encode('+'); //Starting high
            prevBit = '+';
        }
        for (i = 1; i < strlen(c->code); i++) {
            if (c->code[i] == '1') {
                if (prevBit == '+') {
                    encode('-');
                    prevBit = '-';
                } else {
                    encode('+');
                    prevBit = '+';
                }
            }
            else {
                encode(prevBit);
            }
        }
        fclose(fOUT);
        puts("Done.");
    } 
    else {
        errorLog(5);
    }
}

/**
 * 
 * @param c struct that contains the line of binary numbers to be encoded.
 * 
 * Will encode the signal, represented by binary numbers, in B-AMI. 
 */
void B_AMI(Command* c) {
    if (openFile(c)) {
        fputs("|", fOUT); //To start the output with |
        unsigned int i;
        unsigned char prevBit;
        if (c->code[0] == '1') {
            encode('+'); //starting High
            prevBit = '+';
        } else {
            encode('0');
        }
        for (i = 1; i < strlen(c->code); i++) {
            if (c->code[i] == '1') {
                if (prevBit == '+') {
                    encode('-');
                    prevBit = '-';
                } else {
                    encode('+');
                    prevBit = '+';
                }
            }
            else {
                encode('0');
            }
        }
        fclose(fOUT);
        puts("Done.");
    } else {
        errorLog(5);
    }

}

/**
 * 
 * @param c struct that contains the line of binary numbers to be encoded.
 * 
 * Will encode the signal, represented by binary numbers, in Manchester. 
 */
void Manchester(Command* c) {
    if (openFile(c)) {
        fputs("|", fOUT); //To start the output with |
        unsigned int i;
        for (i = 0; i < strlen(c->code); i++) {
            if (c->code[i] == '0') {
                fputs("|", fOUT);
                encode('+');
                encode('-');
            } else {
                fputs("|", fOUT);
                encode('-');
                encode('+');
            }
        }
        fputs("|", fOUT);
        fclose(fOUT);
        puts("Done.");
    } else {
        errorLog(5);
    }

}

/**
 * 
 * @param c struct that contains the line of binary numbers to be encoded.
 * 
 * Will encode the signal, represented by binary numbers, in Differential Manchester. 
 */
void D_Manchester(Command* c) {
    if (openFile(c)) {
        fputs("|", fOUT); //To start the output with |
        unsigned int i;
        unsigned char prevEncoding = '-'; //D-Manchester starts as positive.
        for (i = 0; i < strlen(c->code); i++) {
            if (c->code[i] == '0') {
                if (prevEncoding == '-') {
                    fputs("|", fOUT);
                    encode('-');
                    encode('+');
                } else {
                    fputs("|", fOUT);
                    encode('+');
                    encode('-');
                }
            } else {
                if (prevEncoding == '-') {
                    fputs("|", fOUT);
                    encode('+');
                    encode('-');
                    prevEncoding = '+';
                } else {
                    fputs("|", fOUT);
                    encode('-');
                    encode('+');
                    prevEncoding = '-';
                }
            }
        }
        fputs("|", fOUT);
        fclose(fOUT);
        puts("Done.");
    } else {
        errorLog(5);
    }

}
/**
 * 
 * @param c struct that has member definition, of type unsigned char, that specifies the type of encoding.
 * 
 * Will execute the specified encoding given by the user.
 * 0 will encode NRZ-L
 * 1 will encode NRZ-I
 * 2 will encode B-AMI
 * 3 will encode Manchester
 * 4 will encode Differential Manchester
 */
void executeCommand(Command* c) {
    if (c->scheme == '0') NRZ_L(c);
    else if (c->scheme == '1') NRZ_I(c);
    else if (c->scheme == '2') B_AMI(c);
    else if (c->scheme == '3') Manchester(c);
    else if (c->scheme == '4') D_Manchester(c);
    else {
        errorLog(0);
    }
}

/**
 * 
 * @param argc The number of parameters given by the user.
 * @param argv The parameters provided given as a string of pointers. 
 * @return Returns EXIT_SUCCESS, if the program terminates correctly.
 * 
 * Gets a string of pointers from the user and makes a string of chars.
 * Checks to make sure the number of parameters provided is correct. 2 parameters 
 * are required by the user, however, 3 are specified in the code in order to 
 * ignore the name of the executable.
 */
int main(int argc, char* argv[]) {
    Command command;
    if (argc > 3) errorLog(2);
    else if (argc < 3) errorLog(1);
    else {
        unsigned char fileName[strlen(argv[1])];
        unsigned char scheme[strlen(argv[2])];
        sscanf(argv[1], "%s", fileName);
        sscanf(argv[2], "%s", scheme);
        if (isdigit(scheme[0]) && strlen(scheme) == 1) {
            strncpy(command.fileName, fileName, sizeof command.fileName);
            command.scheme = scheme[0];
            executeCommand(&command);
        } else {
            errorLog(0);
        }
    }

    return (EXIT_SUCCESS);
}

