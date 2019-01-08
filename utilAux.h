#pragma once
#include <math.h>
#include "bmpio.h"

//helper functions

//this function returns the bytes of an unsigned int
unsigned char *getBytes(unsigned int x){
    unsigned char* bytes = (unsigned char*) malloc(sizeof(unsigned char) * 4);
    int k = 0;

    for(int i = 0; i < 4; i++){
        bytes[k] = *((unsigned char*) &x + i);
        k++;
    }

    return bytes;
}

//this XORs the first pixel with the starting value provided in the key
pixel firstXOR(unsigned int SV, pixel P, unsigned int R){
    unsigned char *bytes = getBytes(SV);

    pixel tmp;

    tmp.R = bytes[2] ^ P.R;
    tmp.G = bytes[1] ^ P.G;
    tmp.B = bytes[0] ^ P.B;

    free(bytes);


    bytes = getBytes(R);

    tmp.R = tmp.R ^ bytes[2];
    tmp.G = tmp.G ^ bytes[1];
    tmp.B = tmp.B ^ bytes[0];

    free(bytes);

    return tmp;
}

//this applies XOR to the other pixels
pixel iterXOR(pixel P1, pixel P2, unsigned int R){
    unsigned char *bytes = getBytes(R);

    pixel tmp;


    tmp.R = P1.R ^ P2.R;
    tmp.G = P1.G ^ P2.G;
    tmp.B = P1.B ^ P2.B;

    tmp.R = tmp.R ^ bytes[2];
    tmp.G = tmp.G ^ bytes[1];
    tmp.B = tmp.B ^ bytes[0];

    free(bytes);

    return tmp;
}
