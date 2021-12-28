//
// Created by Ciprian TEODOROV on 27/12/2021.
// from http://c-faq.com/misc/bitsets.html
//

#ifndef FAST_INTERACTION_NETS_BITSET_H
#define FAST_INTERACTION_NETS_BITSET_H

#include <limits.h>		/* for CHAR_BIT */

/**
 * Gets a bitmask for the bit
 */
#define BITMASK(b) (1 << ((b) % CHAR_BIT))

/**
 * Gets the slot where b is stored
 */
#define BITSLOT(b) ((b) / CHAR_BIT)

/**
 * Sets the bth bit of the bitset a
 */
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))

/**
 * Clears the bth bit of the bitset a
 */
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))

/**
 * Checks the bth bit of the bitset
 */
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))

/**
 * used to compute the number of slots needed for storing nb elements
 */
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#endif //FAST_INTERACTION_NETS_BITSET_H
