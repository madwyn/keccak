/*
** FILE:		keccak.h
** AUTHOR:		incertia
** PURPOSE:		Basic include header for all things keccak related
*/

#ifndef KECCAK_H
#define KECCAK_H


#include <stdint.h>
#include <stddef.h>

/* Computes a keccak hash */
void keccak(uint8_t *in, size_t len, uint8_t* hash, size_t hashlen);


#endif /* #ifndef KECCAK_H */
