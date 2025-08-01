/* This file holds the arithmatic for checksum which is used to 
 * decet transmission errors. The algorithm sums all 16-bit words in 
 * the data, handles carries, and returns the one's complement 
 * of the result.
 */
#include "checksum.h"

/* Compute Internet Checksum for "count" bytes
 *         beginning at location "addr".
 * Taken from https://tools.ietf.org/html/rfc1071
 */
uint16_t ip_checksum(void *addr, int count)
{
    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)addr;

    // Sum all 16-bit words
    while (count > 1) {
        sum += *ptr++;
        count -= 2;
    }

    // Add left-over byte, if any
    if (count > 0) {
        sum += *(uint8_t*)ptr;
    }

    // Fold 32-bit sum to 16 bits: add carrier to result
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    // One's complement
    return (uint16_t)~sum;
}