//  ppm.c
//  Project1 image converting. 
//  Junzhe Wang 2017/9/13
//
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ppm.h"
int readMagicNumber(FILE *fp) {   // first of all, get the magic number to see if it is a correct file
    int magic_number;
    if (!fscanf(fp, "P%d\n", &magic_number)){
        fprintf(stderr,"file is not in PPM format, please choose a PPM file");
        exit(1);
    }
    return magic_number;
}
/**************************************************************************************************************
 **************************************************************************************************************/
// readPPMHeader is used to determine the type of image P3 or P6
Image *readPPMHeader(FILE *f_source, Image *buffer){
    char ch;
    /*create a character to store the magic number*/
    ch = getc(f_source); // ch is int
    /*
     gets the next character (an unsigned char) from the specified
     stream and advances the position indicator for the stream.
     */
    while(ch =='#'){
        do{
            ch=getc(f_source);
        }
        while(ch!='\n'); //read to the end of the line
        ch=getc(f_source);
    }
    if(!isdigit(ch)){
        fprintf(stderr,"can't read header information from PPM format\n");
    }
    else{
        ungetc(ch, f_source); //put that digital back
    }
    /*default tupletype and depth for P3 is RGB and 3*/
    buffer->tupltype = "RGB";
    buffer->depth = 3;
    //read the width, height,amd maximum value for a pixel
    fscanf(f_source, "%d %d %d\n",&buffer->width,&buffer->height,&buffer->maxval);
    buffer->data = (u_char *)malloc(buffer->width*buffer->height*4);// G, R, B three colors
    if(buffer->maxval >= 65336 ||buffer->maxval <= 0){
        fprintf(stderr,"image is not ture-color(8byte), read failed\n");
        exit(1);
    }
    return buffer;
    
}
/**************************************************************************************************************
 **************************************************************************************************************/

Image *ImageRead(const char *filename){
    FILE *fp = fopen(filename,"r");
    if(!fp){
        fprintf(stderr,"can't open file for reading \n");
        exit(1);
    }
    Image *buffer = (Image *)malloc(sizeof(Image));
    if(!buffer){
        fprintf(stderr,"Can't allocate memory for new image");
        exit(1);
    }
    buffer->magic_number = readMagicNumber(fp);
    buffer->data = (u_char *)malloc(buffer->width*buffer->height*4);// G, R, B three colors and A
    if(buffer->magic_number==3){
        readPPMHeader(fp,buffer);
        buffer->data = (u_char *)malloc(buffer->width*buffer->height*4);
        int i,j, pixel[4];
        //In P3 file, we use nested loop, to copy image content to buffer
        for(i=0;i<buffer->height;i++){
            for(j=0;j<buffer->width;j++){
                fscanf(fp, "%d\n",pixel);
                buffer->data[i*buffer->width*4+4*j] = *pixel;//R
                fscanf(fp, "%d\n",pixel);
                buffer->data[i*buffer->width*4+4*j+1] = *pixel;//G
                fscanf(fp, "%d\n",pixel);
                buffer->data[i*buffer->width*4+4*j+2] = *pixel;//B
                buffer->data[i*buffer->width*4+4*j+3] = 255;  //A at here default value is 255
            }
        }
    }
    else if(buffer->magic_number==6){
        readPPMHeader(fp,buffer);
        buffer->data = (u_char *)malloc(buffer->width*buffer->height*4);
        int size1 = buffer->width * buffer->height*4;
        int PPM_size = buffer->height*buffer->width*3;//give the times for our for loop
        char c='A';//init
        int k=0;
        // for each llop we copy R, G, B to the buffer and let A =255
        for(int m=0;m<PPM_size;m+=3){
            fread(&c, 1, 1, fp);
            buffer->data[k] = c;
            k += 1;
            fread(&c, 1, 1, fp);
            buffer->data[k] = c;
            k += 1;
            fread(&c, 1, 1, fp);
            buffer->data[k] = c;
            k += 1;
            buffer->data[k] = 255;
            k += 1;
        }

            if (k!= size1) {
                printf("n is %d, size is %d \n",k,size1);
                fprintf(stderr,"cannot read image datat from file \n");
                exit(1);
            }
   
        }
		else{
			fprintf(stderr,"invalid image type \n");
                    exit(1);
		}
    return buffer;
}
/**************************************************************************************************************
 **************************************************************************************************************/
void ImageWrite(Image *buffer, const char *filename,int format){
    int num2;
    int size = buffer->width * buffer->height * 4;
    FILE *f_des = fopen(filename, "w");
    if (!f_des){
        fprintf(stderr,"cannot open file for writing");
    }
    if(format==6){
        fprintf(f_des, "P%d\n%d %d\n%d\n",format,buffer->width, buffer->height, buffer->maxval);
        for(int i=1; i<size+1;i++){    // for each for slots we skip it,
            char ch=buffer->data[i-1];
            if (i%4 !=0) {
               fwrite(&ch, 1, 1, f_des);  // which means we skip A at here
            }
        }
    }
    else if(format==3){
        fprintf(f_des, "P%d\n%d %d\n%d\n",format,buffer->width, buffer->height, buffer->maxval);
        int i,j;
        for(i=0;i<buffer->height;i++){
            for(j=0;j<buffer->width;j++){
                fprintf(f_des, "%d\n",buffer->data[i*buffer->width*4+4*j]); // We got R and put in to the Buffer
                fprintf(f_des, "%d\n",buffer->data[i*buffer->width*4+4*j+1]);// We got G and put in to the Buffer
                fprintf(f_des, "%d\n",buffer->data[i*buffer->width*4+4*j+2]);// We got B and put in to the Buffer
                //Skip the A
            }
            fprintf(f_des, "\n");
        }
    }
	else {
		fprintf(stderr,"invalid image type \n");
        exit(1);
	}

}