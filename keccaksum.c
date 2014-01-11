/*
** FILE:		keccaksum.c
** AUTHOR:		incertia
** PURPOSE:		Produce a hash digest from a file or stdin
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "keccak.h"

void printsum(const char *_fname, FILE *_f);

/* USAGE: keccaksum <digest_size> <files> */
int main(int argc, char **argv){
	register int i;

	/* First check to see if all our base is in order */
	if(argc < 2){
		printf("Usage: keccaksum <digest_size> <files>\n");
		return -1;
	} else if(argc == 2){
		printsum("-", stdin);
	} else {
		for(i = 2; i < argc; i++){
			if(access(argv[i], F_OK) != 0) fprintf(stderr, "Error: could not find %s\n", argv[i]);
			else if(access(argv[i], R_OK) != 0) fprintf(stderr, "Error: cannot read %s\n", argv[i]);
			else printsum(argv[i], fopen(argv[i], "r"));
		}
	}

	return 0;
}

void printsum(const char *_fname, FILE *_f){
	printf("%s\n", _fname);
	if(_f != stdin) fclose(_f);
}
