/*
** FILE:		test.c
** AUTHOR:		incertia
** PURPOSE:		Testing the keccak hash function
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "keccak.h"

int main(int argc, char **argv){
	uint8_t hash[64];
	char *s = "Keccak-256 Test Hash";
	register int i;

	memset(hash, 0, sizeof(hash));
	
	keccak((uint8_t *)s, strlen(s), hash, 256 / 8);

	for(i = 0; i < 256 / 8; i++)
		printf("%02x", hash[i]);

	printf("\n");
	return 0;
}
