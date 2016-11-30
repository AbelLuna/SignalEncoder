/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: abelluna75
 *
 * Created on November 29, 2016, 2:17 PM
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
struct A{
    char fileName[256];//Name of the file to get binary code from.
    char scheme[2]; //The encoding scheme that will be used.
    char code[100];
};
typedef struct A Command;
FILE *fp, *fOUT;

void errorLog(unsigned int err){
    switch(err){
        case 0: puts("Error: Scheme not recognized.");break;
        case 1: puts("Error: Not enough parameters entered.");break;
        case 2: puts("Error: Too many parameters entered.");break;
        case 3: puts("Error: File not found. Make sure extension is written.");break;
        case 4: puts("Error: Data is not in binary format.");break;
        case 5: puts("Output file not written.");break;
        default: puts("Error: Unexpected error occurred.");
    }
}

unsigned char isBinary(char* c){
    unsigned int i;
    for(i=0;i<(strlen(c));i++){
        if(c[i]!='1'&& c[i]!='0'){
            return 0;
        }
    }
    return 1;
}

unsigned char openFile(Command* c){
    if((fp=fopen(c->fileName,"r"))!=NULL){
        char code[100];
        fgets(code,sizeof code, fp);
        fclose(fp);//Will only read the first line of file.
        sscanf(code,"%s",code);
        if(isBinary(code)){
            fOUT = fopen("OUTPUT.SIGNAL","w");
            strncpy(c->code,code,sizeof code);
            return 1;
        }
        else{
            errorLog(4);
            return 0;
        }
    }
    else{
        errorLog(3);
        return 0;
    }
}

void encode(char c){
    if(c=='0'){
        fputs("0V|",fOUT);
    }
    else if(c=='+'){
        fputs("+V|",fOUT);
    }
    else if(c=='-'){
        fputs("-V|",fOUT);
    }
}

void NRZ_L(Command* c){
    if(openFile(c)){
        fputs("|",fOUT);//To start the output with |
        unsigned int i;
        for(i=0;i<strlen(c->code);i++){
            if(c->code[i]=='0') encode('+');
            else encode('-');
        }
        fclose(fOUT);
    }
    else{
        errorLog(5);
    }
    
}
void NRZ_I(Command* c){
    if(openFile(c)){
        fputs("|",fOUT);//To start the output with |
        unsigned int i;
        unsigned char prevBit;
        if(c->code[0]=='0'){
            encode('-');//starting low
            prevBit='-';
        }
        else {
            encode('+');//Starting high
            prevBit='+';
        }
        for(i=1;i<strlen(c->code);i++){
            if(c->code[i]=='1'){
                if(prevBit=='+'){
                    encode('-');
                    prevBit='-';
                }
                else{
                    encode('+');
                    prevBit='+';
                }
            } 
            else{
                encode(prevBit);
            }
        }
        fclose(fOUT);
    }
    else{
        errorLog(5);
    }
    
}
void B_AMI(Command* c){
    if(openFile(c)){
       fputs("|",fOUT);//To start the output with |
       unsigned int i;
       unsigned char prevBit;
       if(c->code[0]=='1'){
           encode('+');//starting High
           prevBit='+';
       }
       else {
           encode('0');
       }
       for(i=1;i<strlen(c->code);i++){
           if(c->code[i]=='1'){
               if(prevBit=='+'){
                   encode('-');
                   prevBit='-';
               }
               else{
                   encode('+');
                   prevBit='+';
               }
           } 
           else{
               encode('0');
           }
       }
       fclose(fOUT);
   }
    else{
        errorLog(5);
    }
    
}
void Manchester(Command* c){
    if(openFile(c)){
        fputs("|",fOUT);//To start the output with |
        unsigned int i;
        for(i=0;i<strlen(c->code);i++){
            if(c->code[i]=='0') {
                fputs("|",fOUT);
                encode('+');
                encode('-');
            }
            else {
                fputs("|",fOUT);
                encode('-');
                encode('+');
            }
        }
        fputs("|",fOUT);
        fclose(fOUT);
    }
    else{
        errorLog(5);
    }
    
}
void D_Manchester(Command* c){
    if(openFile(c)){
        fputs("|",fOUT);//To start the output with |
        unsigned int i;
        unsigned char prevEncoding='-';//D-Manchester starts as positive.
        
        for(i=0;i<strlen(c->code);i++){
            if(c->code[i]=='0') {
                if(prevEncoding=='-'){
                    fputs("|",fOUT);
                    encode('-');
                    encode('+');
                }
                else{
                    fputs("|",fOUT);
                    encode('+');
                    encode('-');
                }
            }
            else {
                if(prevEncoding=='-'){
                    fputs("|",fOUT);
                    encode('+');
                    encode('-');
                    prevEncoding='+';
                }
                else{
                    fputs("|",fOUT);
                    encode('-');
                    encode('+');
                    prevEncoding='-';
                }
            }
        }
        fputs("|",fOUT);
        fclose(fOUT);
    }
    else{
        errorLog(5);
    }
    
}
void executeCommand(Command* c){
    if (c->scheme[0]=='0') NRZ_L(c);
    else if (c->scheme[0]=='1') NRZ_I(c);
    else if (c->scheme[0]=='2') B_AMI(c);
    else if (c->scheme[0]=='3') Manchester(c);
    else if (c->scheme[0]=='4') D_Manchester(c);
    else {errorLog(0);}
}


int main(int argc, char* argv[]) {
//    for(int i = 0; i < argc; i++){
//        
//        printf("\nArg %d: %s\n", i, argv[i]);
//    }
//    
    Command command;
    if(argc>3) errorLog(2);
    else if(argc<3) errorLog(1);
    else{
        char fileName[1000];
        char scheme[1000];
        sscanf(argv[1],"%s", fileName);
        sscanf(argv[2],"%s", scheme);
        int n = strlen(scheme);
        if(isdigit(scheme[0]) && strlen(scheme)==1){
            strncpy(command.fileName,fileName,sizeof command.fileName);
            strncpy(command.scheme,scheme,sizeof command.scheme);
            executeCommand(&command);
        }
        else{
            errorLog(0);
        }
    }
    
    return (EXIT_SUCCESS);
}

