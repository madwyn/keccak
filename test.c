/*
** FILE:        test.c
** AUTHOR:      incertia
** PURPOSE:     Testing the keccak hash function
*/

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "keccak.h"

int main(int argc, char **argv){
    uint8_t hash[64];
    char s[1025];
    register int i, j;
    size_t digestsize[4] = {224 / 8, 256 / 8, 384 / 8, 512 / 8};

    memset(s, 0, sizeof(s));
    printf("Enter a string (1024 chars max): ");
    for(i = 0; i < 1024; i++){
        s[i] = getchar();
        if(isspace(s[i])){
            s[i] = 0;
            break;
        }
    }

    printf("Found: '%s'\n", s);

    for(j = 0; j < 4; j++){
        memset(hash, 0, sizeof(hash));

        keccak((uint8_t *)s, strlen(s), hash, digestsize[j]);

        printf("Keccak-%lu: ", digestsize[j] * 8);
        for(i = 0; i < digestsize[j]; i++)
            printf("%02x", hash[i]);

        printf("\n");
    }

    return 0;
}
