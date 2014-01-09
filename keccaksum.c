/*
** FILE:		keccaksum.c
** AUTHOR:		incertia
** PURPOSE:		Produce a hash digest from a file or stdin
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "keccak.h"


/* USAGE: keccaksum <digest_size> <files> */
int main(int argc, char **argv){
	char *buf;				/* char buffer */
	register size_t n = 0;	/* char counter */

	/* First check to see if all our base is in order */
	if(argc < 3){
		printf("Usage: keccaksum <digest_size> <files>\n");
		return -1;
	}

	return 0;
}
