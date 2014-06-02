/*
** FILE:		keccaksum.c
** AUTHOR:		incertia
** PURPOSE:		Produce a hash digest from a file or stdin
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "keccak.h"

/* Get rid of all previous definitions of KECCAK_ROUNDS */
#ifdef KECCAK_ROUNDS
#undef KECCAK_ROUNDS
#endif /* #ifdef KECCAK_ROUNDS */
#define KECCAK_ROUNDS	24

extern void keccakf(uint64_t _s[25], const int _rounds);

void printsum(const char *_fname, FILE *_f, size_t size);

/* USAGE: keccaksum <digest_size> <files> */
int main(int argc, char **argv){
	register int i;
	size_t digestsize;

	/* First check to see if all our base is in order */
	if(argc < 2){
		printf("Usage: %s <digest_size> <files>\n", argv[0]);
		return -1;
	} else if(argc == 2){
		sscanf(argv[1], "%lu", &digestsize);
		printsum("-", stdin, digestsize / 8);
	} else {
		sscanf(argv[1], "%lu", &digestsize);
		for(i = 2; i < argc; i++){
			if(strcmp(argv[i], "-") == 0) printsum("-", stdin, digestsize / 8);
			else if(access(argv[i], F_OK) != 0) fprintf(stderr, "Error: could not find %s\n", argv[i]);
			else if(access(argv[i], R_OK) != 0) fprintf(stderr, "Error: cannot read %s\n", argv[i]);
			else printsum(argv[i], fopen(argv[i], "r"), digestsize / 8);
		}
	}

	return 0;
}

void printsum(const char *_fname, FILE *_f, size_t size){	
	uint64_t state[25];
	uint8_t *hash = malloc(size * sizeof(uint8_t));
	uint8_t buf[25];
	uint8_t tmp[144];
	register int i, rsize, rsizew;
	register size_t j;

	rsize = 200 - 2 * size;
	rsizew = rsize / 8;

	//Clear the state
	memset(state, 0, sizeof(state));

	do {
		//read up to rsize bytes, then do work
		j = fread(buf, 1, rsize, _f);
		
		//check some stuff
		if(feof(_f)){
			break;
		} else if(ferror(_f)){
			fprintf(stderr, "Error when reading %s.\n", _fname);
			goto fin;
		} else {
			//First few blocks (i.e. not last block)
			for(; j >= rsize; j -= rsize){
				for(i = 0; i < rsizew; i++)
					state[i] ^= ((uint64_t *)buf)[i];
				keccakf(state, KECCAK_ROUNDS);
			}
		}
	} while(1);

	//Last block + padding
	memcpy(tmp, buf, j);
	tmp[j++] = 1;
	memset(tmp + j, 0, rsize - j);
	tmp[rsize - 1] |= 0x80;

	for(i = 0; i < rsizew; i++)
		state[i] ^= ((uint64_t *)tmp)[i];
	keccakf(state, KECCAK_ROUNDS);

	//copy hash
	memcpy(hash, state, size);

	//print
	for(i = 0; i < size; i++) printf("%02x", hash[i]);
	printf("  %s\n", _fname);

fin:	
	if(_f != stdin) fclose(_f);
	free(hash);
}
