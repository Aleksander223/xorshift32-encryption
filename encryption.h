#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xorshift32.h"
#include "bmpio.h"
#include "utilAux.h"

typedef struct{
    unsigned int seed;
    unsigned int startingValue;
}secretKey;

secretKey readKey(char *path){
    FILE *in = fopen(path, "r");
    secretKey s;

    fscanf(in,"%u %u", &s.seed, &s.startingValue);
    fclose(in);
    return s;
}

void encrypt(char *inPath, char *outPath, char *keyPath){
    bmpFile img = loadBmpFile(inPath);
    secretKey sk = readKey(keyPath);
    unsigned int size = img.header.height * img.header.width;

    unsigned int* rArray = randomArray(sk.seed, 2 * size - 1);
    unsigned int* permArray = generatePermutation(rArray, size);

    pixel *interImg = (pixel*) malloc(sizeof(pixel) * size);

    //permutation
    for(unsigned int i = 0; i < size; i++){
        int j = permArray[i];
        interImg[j] = img.data[i];
    }

    pixel *cryptImg = (pixel*) malloc(sizeof(pixel) * size);

    //xor
    cryptImg[0] = firstXOR(sk.startingValue, interImg[0], rArray[size]);

    for(unsigned int i = 1; i < size; i++){
        cryptImg[i] = iterXOR(cryptImg[i-1], interImg[i], rArray[size + i]);
    }

    for(unsigned int i = 0; i < size; i++)
        img.data[i] = cryptImg[i];

    saveBmpFile(outPath, img);

    free(img.data);
    free(rArray);
    free(permArray);
    free(interImg);
    free(cryptImg);
}


void decrypt(char *inPath, char *outPath, char *keyPath){
    bmpFile img = loadBmpFile(inPath);
    secretKey sk = readKey(keyPath);
    unsigned int size = img.header.height * img.header.width;

    unsigned int* rArray = randomArray(sk.seed, 2 * size - 1);

    unsigned int* permArray = generatePermutation(rArray, size);
    unsigned int* invPerm   = inversePermutation(permArray, size);

    //xor
    pixel *interImg = (pixel*) malloc(sizeof(pixel) * size);

    interImg[0] = firstXOR(sk.startingValue, img.data[0], rArray[size]);
    for(unsigned int i = 1; i < size; i++){
        interImg[i] = iterXOR(img.data[i-1], img.data[i], rArray[size + i]);
    }

    //inverse permutation
    for(unsigned int i = 0; i < size; i++){
        unsigned int j = invPerm[i];
        img.data[j] = interImg[i];
    }

    saveBmpFile(outPath, img);

    free(img.data);
    free(rArray);
    free(permArray);
    free(invPerm);
    free(interImg);
}

