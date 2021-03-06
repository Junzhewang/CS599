//
//  main.c
//  Project1  image converting
//  Junzhe Wang jw2323@nau.edu
//  2017/9/13
//
#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"

int main(int argc, char **argv){
    if(argc < 4){
        perror("We need more arguments");
        exit(1);//if the nunber is not 0, not access to error
    }
    else if(argc > 4){
        perror("Too many arguments");
        exit(1);//if the nunber is not 0, not access to error
    }
    int format =atoi(argv[1]); // change the first agument to the int, format shoule be 1-7;
    const char *inputNmae = argv[2];  //input.ppm
    const char *outputName = argv[3]; //output.ppm
    
    Image *image;
    
    /************************************************************************************************/
    image = ImageRead(inputNmae);
 
    ImageWrite(image, outputName, format);
    

}

