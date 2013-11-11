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
void keccak(const uint8_t *in, const size_t len, uint8_t* hash, const size_t hashlen);


#endif /* #ifndef KECCAK_H */
