#pragma once
#include <stdlib.h>
#include <stdio.h>

unsigned int xorshift32(unsigned int x){
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

//this generates an array of random numbers using XORSHIFT32
unsigned int* randomArray(unsigned int seed, unsigned int count){
	unsigned int* v;
	v = (unsigned int*) malloc(sizeof(unsigned int) * (count+1));

	v[0] = seed;

	for(unsigned int i = 1; i <= count; i++){
		seed = xorshift32(seed);
		v[i] = seed;
	}

	return v;
}

//this generates a permutation array, using an array as the random numbers
unsigned int* generatePermutation(unsigned int *randArray, unsigned int size){
    unsigned int *permArray = (unsigned int*) malloc(sizeof(unsigned int) * size);

	for(unsigned int i = 0; i < size; i++)
		permArray[i] = i;

	for(unsigned int i = size-1; i >= 1; i--){
		unsigned int j = randArray[size-i] % (i+1);

		unsigned int tmp = permArray[j];
		permArray[j] = permArray[i];
		permArray[i] = tmp;

	}

	return permArray;
}

//this generates the inverse of a permutation
unsigned int* inversePermutation(unsigned int *permArray, unsigned int size){
	unsigned int *invArray = (unsigned int*) malloc(sizeof(unsigned int) * size);

	for(unsigned int i = 0; i < size; i++)
		invArray[permArray[i]] = i;

	return invArray;
}
