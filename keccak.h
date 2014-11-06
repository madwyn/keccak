/*
** FILE:        keccak.h
** AUTHOR:      incertia
** PURPOSE:     Basic include header for all things keccak related
*/

#ifndef KECCAK_H
#define KECCAK_H


#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Computes a keccak hash 
 * Also const correctness is a headache */
void keccak(const uint8_t *_in, const size_t _len, uint8_t *const _hash, const size_t _hashlen);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef KECCAK_H */

