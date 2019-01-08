#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    unsigned char B;
    unsigned char G;
    unsigned char R;
}pixel;

typedef struct{
    unsigned int    type;
    unsigned int    size;
    unsigned int    reserved;
    unsigned int    offset;
    unsigned int    dibSize;
    unsigned int    width;
    unsigned int    height;
    unsigned int    colorPlanes;
    unsigned int    bitsPerPixel;
    unsigned int    compression;
    unsigned int    imageSize;
    unsigned int    xResolution;
    unsigned int    yResolution;
    unsigned int    numColors;
    unsigned int    importantColors;
}headerData;

typedef struct{
    headerData header;
    pixel *data;
}bmpFile;

headerData readHeader(FILE* in){
    headerData h;
    fread(&h.type, 2, 1, in);
    fread(&h.size, 4, 1, in);
    fread(&h.reserved, 4, 1, in);
    fread(&h.offset, 4, 1, in);
    fread(&h.dibSize, 4, 1, in);
    fread(&h.width, 4, 1, in);
    fread(&h.height, 4, 1, in);
    fread(&h.colorPlanes, 2, 1, in);
    fread(&h.bitsPerPixel, 2, 1, in);
    fread(&h.compression, 4, 1, in);
    fread(&h.imageSize, 4, 1, in);
    fread(&h.xResolution, 4, 1, in);
    fread(&h.yResolution, 4, 1, in);
    fread(&h.numColors, 4, 1, in);
    fread(&h.importantColors, 4, 1, in);

    return h;
}

void writeHeader(FILE* out, headerData h){
    fwrite(&h.type, 2, 1, out);
    fwrite(&h.size, 4, 1, out);
    fwrite(&h.reserved, 4, 1, out);
    fwrite(&h.offset, 4, 1, out);
    fwrite(&h.dibSize, 4, 1, out);
    fwrite(&h.width, 4, 1, out);
    fwrite(&h.height, 4, 1, out);
    fwrite(&h.colorPlanes, 2, 1, out);
    fwrite(&h.bitsPerPixel, 2, 1, out);
    fwrite(&h.compression, 4, 1, out);
    fwrite(&h.imageSize, 4, 1, out);
    fwrite(&h.xResolution, 4, 1, out);
    fwrite(&h.yResolution, 4, 1, out);
    fwrite(&h.numColors, 4, 1, out);
    fwrite(&h.importantColors, 4, 1, out);
}

//this function loads a BMP file into the internal memory
bmpFile loadBmpFile(char *path){
    FILE* in = fopen(path, "rb");

    if(in==NULL){
        printf("\nFile %s does not exist!\n", path);
        exit(2);
    }

    bmpFile img;

    img.header = readHeader(in);
    img.data = (pixel*) malloc(sizeof(pixel) * img.header.height * img.header.width);

    pixel* interImg = (pixel* ) malloc(sizeof(pixel) * img.header.height * img.header.width);

    //BMP is little-endian, so it is flipped vertically
    for(int i = img.header.height - 1; i >= 0; i--){
        for(int j = 0; j < img.header.width; j++){
            pixel tmp;
            fread(&tmp, 3, 1, in);
            interImg[i * img.header.width + j] = tmp;
        }

        //we ignore the padding and write it later
        int padding = 0;
        if(img.header.width % 4)
            padding = 4 - (3 * img.header.width) % 4;

        int tmp;
        for(int k = 0; k < padding; k++)
            fread(&tmp, 1, 1, in);
    }

    for(int i = 0; i < img.header.height * img.header.width; i++){
        img.data[i] = interImg[i];
    }

    free(interImg);
    fclose(in);

    return img;
}

//saves a BMP file to the specified path
void saveBmpFile(char *path, bmpFile img){
    FILE* out = fopen(path, "wb");
    if(out == NULL){
        printf("\nError saving the file.\n");
        exit(2);
    }
    writeHeader(out, img.header);


    for(int i = img.header.height - 1; i >= 0; i--){
        for(int j = 0; j < img.header.width; j++){
            pixel tmp = img.data[i * img.header.width + j];
            fwrite(&tmp, 3, 1, out);
        }
        //padding
        int padding = 0;
        if(img.header.width % 4)
            padding = 4 - (3 * img.header.width) % 4;

        int tmp = 0;
        for(int k = 0; k < padding; k++){
            fwrite(&tmp, 1, 1, out);
        }
    }

    fclose(out);

}
