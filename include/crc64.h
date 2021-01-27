#ifndef CRC64_H
#define CRC64_H

/* interface to CRC 64 module */

/* crc64 takes a string argument and computes a 64-bit hash based on */
/* cyclic redundancy code computation.                               */

unsigned long long crc64(char* string);

#endif
