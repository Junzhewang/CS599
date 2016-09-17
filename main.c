#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Image{
    int rownum, colnum, maxnum;
    char filetype[3];
}Image;


int main(int argc, char * argv[])
{
    //argv[1] is the format we want, and is an integer(3,6,7).
    //argv[2] is the old file name.
    //argv[3] is the new file name.
    if(argc < 4){
        perror("We need four arguments");
        exit(1);
    }
    else if(argc > 4){
        perror("we need four arguments");
        exit(1);
    }
    int type = atoi(argv[1]);
    const char * inputName = argv[2];
    const char * outputName = argv[3];

    Image image;
    FILE * oldfile = fopen(inputName, "rb");

    if(oldfile == NULL) {
            fprintf(stderr, "Could not open file %s.", inputName);
            exit(1);
    }
    // Check whether the file is opened correctly.

    // delete the space before file type, and store the file type into filetype.

    fscanf(oldfile, "%s", image.filetype);
    char c;
    c = fgetc(oldfile); // ch is int
    while(c=='#'){
        if(c == '#'){
            while(c!='\n'){
                c =fgetc(oldfile);
            }

        }
        c = fgetc(oldfile);
    }
    ungetc(c, oldfile);
    fscanf(oldfile, "%d", &image.rownum);
    fscanf(oldfile, "%d", &image.colnum);
    fscanf(oldfile, "%d", &image.maxnum);
    int size = image.rownum * image.colnum * 3;
    int RGBA =0;
    int buffer[size];
    int *bufferpointer;
    bufferpointer=&buffer[0];
    if (strcmp(image.filetype, "P3") == 0){

        int i =0;
        for(i<size;i++;){
            fscanf(oldfile,"%d ",buffer[i]);
        }
    }
    else if (strcmp(image.filetype, "P6") == 0){
        int i=0;
        for (i<size;i=i+1;){
            fread(bufferpointer,1,1,oldfile);
        }

    }
    else if (strcmp(image.filetype, "P7") == 0){
        int i=0;
        for(i<size; i++;){
            fscanf(oldfile,"%d ",buffer[i]);
        }
        if (!feof(oldfile)){
            i=size;
            for (i<(size/3*4); i++;){
                fscanf(oldfile,"%d ",buffer[i]);
            }
            int RGBA =1;
        }
    }
    fclose(oldfile);

    FILE *newfile = fopen (argv[3],"wb");
    fprintf(newfile, "P%s\n",argv[1]);
    fprintf(newfile, "%d ",image.rownum);
    fprintf(newfile, "%d\n",image.colnum);
    fprintf(newfile, "%d\n",image.maxnum);

    int *bufferpointer2;
    bufferpointer2=&buffer[0];

    if(atoi(argv[1])==3){
        int i = 0;
        for(i<size; i++;){
            fprintf(newfile, "%d", buffer[i] );
        }

    }
    else if(atoi(argv[1])==6){
        int i =0;
        for(i<size;i++;){
            fwrite(bufferpointer2,1,1,newfile);
        }

    }
    else if(atoi(argv[1])==7){

        if(RGBA==0){
            int i =0;
            for(i<size; i++;){
                fprintf(newfile, "%d", buffer[i]);
            }
        }
        else{
            int i=0;
            for(i<size/3*4; i++;){
                if(((i+1)%4)!=0){
                    fprintf(newfile, "%d", buffer[i]);
                }

            }
        }

    }
    fclose (newfile);
}
