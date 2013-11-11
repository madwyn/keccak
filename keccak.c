#include "keccak.h"

#include <string.h>


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

/* Updates the given state with a given number of rounds */
void keccakf(uint64_t s[25], int rounds){
	register int i, j, round;
	register uint64_t t;
	uint64_t bc[5];

	for(round = 0; round < rounds; round++){
		//Theta step
		for(i = 0; i < 5; i++)
			bc[i] = s[i] ^ s[i + 5] ^ s[i + 10] ^ s[i + 15] ^ s[i + 20];
		for(i = 0; i < 5; i++){
			t = bc[(i + 4) % 5] ^ ROTL64(bc[(i + 1) % 5], 1);
			for(j = 0; j < 25; j += 5)
				s[j + i] ^= t;
		}

		//Rho and Pi
		t = s[1];
		for(i = 0; i < 24; i++){
			j = keccakf_piln[i];
			bc[0] = s[j];
			s[j] = ROTL64(t, keccakf_rotc[i]);
			t = bc[0];
		}

		//Chi
		for(j = 0; j < 25; j += 5){
			for(i = 0; i < 5; i++)
				bc[i] = s[j + i];
			for(i = 0; i < 5; i++)
				s[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
		}

		//Iota
		s[0] ^= keccakf_rndc[round];
	}
}

void keccak(const uint8_t *in, const size_t len, uint8_t *hash, const size_t hashlen){
	uint64_t state[25];
	uint8_t tmp[144];
	register int i, rsize, rsizew;
	register size_t j = len;

	rsize = 200 - 2 * hashlen;
	rsizew = rsize / 8;

	//Clear the state
	memset(state, 0, sizeof(state));

	//First few blocks (i.e. not last block)
	for(; j >= rsize; j -= rsize, in += rsize){
		for(i = 0; i < rsizew; i++)
			state[i] ^= ((uint64_t *)in)[i];
		keccakf(state, KECCAK_ROUNDS);
	}

	//Last block + padding
	memcpy(tmp, in, j);
	tmp[j++] = 1;
	memset(tmp + j, 0, rsize - j);
	tmp[rsize - 1] |= 0x80;

	for(i = 0; i < rsizew; i++)
		state[i] ^= ((uint64_t *)tmp)[i];
	keccakf(state, KECCAK_ROUNDS);

	memcpy(hash, state, hashlen);
}
