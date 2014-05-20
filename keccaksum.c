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

/* Get rid of all previous definitions of ROTL64 */
#ifdef ROTL64
#undef ROTL64
#endif /* #ifdef ROTL64 */
#define ROTL64(x, y)	(((x) << (y)) | ((x) >> (64 - (y))))

/* Get rid of all previous definitions of KECCAK_ROUNDS */
#ifdef KECCAK_ROUNDS
#undef KECCAK_ROUNDS
#endif /* #ifdef KECCAK_ROUNDS */
#define KECCAK_ROUNDS	24

const uint64_t keccakf_rndc[24] =
{
	0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
	0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
	0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
	0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
	0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
	0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
	0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
	0x8000000000008080, 0x0000000080000001, 0x8000000080008008,
};

const uint64_t keccakf_rotc[24] =
{
	1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
	27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44,
};

const uint64_t keccakf_piln[24] =
{
	10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
	15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1,
};

void keccakf(uint64_t _s[25], const int _rounds);

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

/* Updates the given state with a given number of rounds */
void keccakf(uint64_t _s[25], const int _rounds){
	register int i, j, round;
	register uint64_t t;
	uint64_t bc[5];

	for(round = 0; round < _rounds; round++){
		//Theta step
		for(i = 0; i < 5; i++)
			bc[i] = _s[i] ^ _s[i + 5] ^ _s[i + 10] ^ _s[i + 15] ^ _s[i + 20];
		for(i = 0; i < 5; i++){
			t = bc[(i + 4) % 5] ^ ROTL64(bc[(i + 1) % 5], 1);
			for(j = 0; j < 25; j += 5)
				_s[j + i] ^= t;
		}

		//Rho and Pi
		t = _s[1];
		for(i = 0; i < 24; i++){
			j = keccakf_piln[i];
			bc[0] = _s[j];
			_s[j] = ROTL64(t, keccakf_rotc[i]);
			t = bc[0];
		}

		//Chi
		for(j = 0; j < 25; j += 5){
			for(i = 0; i < 5; i++)
				bc[i] = _s[j + i];
			for(i = 0; i < 5; i++)
				_s[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
		}

		//Iota
		_s[0] ^= keccakf_rndc[round];
	}
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
