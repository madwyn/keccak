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

void printsum(const char *_fname, FILE *_f, size_t size);

/* USAGE: keccaksum <digest_size> <files> */
int main(int argc, char **argv){
	register int i;
	size_t digestsize;

	/* First check to see if all our base is in order */
	if(argc < 2){
		printf("Usage: keccaksum <digest_size> <files>\n");
		return -1;
	} else if(argc == 2){
		sscanf(argv[1], "%lu", &digestsize);
		printsum("-", stdin, digestsize / 8);
	} else {
		sscanf(argv[1], "%lu", &digestsize);
		for(i = 2; i < argc; i++){
			if(access(argv[i], F_OK) != 0) fprintf(stderr, "Error: could not find %s\n", argv[i]);
			else if(access(argv[i], R_OK) != 0) fprintf(stderr, "Error: cannot read %s\n", argv[i]);
			else printsum(argv[i], fopen(argv[i], "r"), digestsize / 8);
		}
	}

	return 0;
}

void printsum(const char *_fname, FILE *_f, size_t size){
	register long len;
	register size_t i;
	register char *buf = NULL;
	uint8_t *hash = malloc(sizeof(char) * size);

	fseek(_f, 0, SEEK_END);
	len = ftell(_f);
	fseek(_f, 0, SEEK_SET);

	buf = malloc(sizeof(char) * len);
	memset(buf, 0, sizeof(char) * len);

	if(!buf){
		fprintf(stderr, "Error: not enough memory to load %s\n", _fname);
		goto fin;
	}

	fread(buf, sizeof(char), len, _f);
	//printf("%s", buf);
	keccak((uint8_t *)buf, len, hash, size);
	for(i = 0; i < size; i++) printf("%02x", hash[i]);
	printf("  %s\n", _fname);

fin:
	if(_f != stdin) fclose(_f);
	free(hash);
	free(buf);
}
