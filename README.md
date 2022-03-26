# hex string to byte array in C/C++

## tl;dr
```C
/* Copyright (c) 2022 Tero 'stedo' Liukko, MIT License */
static unsigned char unhex(char h){
  return ((h & 0x0f) + (h >> 6) * 9);
}

static unsigned hex2bin(const char* hex, unsigned hlen, void* bin, unsigned blen){
  unsigned i, j;
  for(i = 0, j = 0; (i < blen) && (j+1 < hlen); ++i, j+=2){
    unsigned char hi = unhex(hex[j+0]);
    unsigned char lo = unhex(hex[j+1]);
    ((unsigned char*)bin)[i] = (hi << 4) | lo;
  }
  return i;
}
```

## Features
- does exactly what it says on the tin
- single header file only
- no dependencies, not even the standard library
- written in ANSI C, can be used in C++
- cross-platform / platform-agnostic
- branchless implementation without look-up tables
- *no input validation whatsoever*
- suitable for embedded development and microcontrollers

## Background
The Web is full of examples (in various languages) on how to decode a hex
string to an array of bytes, but none has been as elegant as one would hope;
they either
- use libraries (like `sscanf` from `cstdio`)
- use look-up tables that take space
- use multiple branches to evaluate whether input is in the hexadecimal
  character set and whether it's in uppercase or lowercase

A more elegant elegant solution exists to map the input space to output space;
consider the ASCII character table and the characters we are interested in
| ASCII | hex | bin       | ASCII | hex | bin       | ASCII | hex | bin
| ----- | --- | --------- | ----- | --- | --------- | ----- | --- | ---------
| 0     | 30  | 0011 0000 |       |     |           |       |     |
| 1     | 31  | 0011 0001 | A     | 41  | 0100 0001 | a     | 61  | 0110 0001
| 2     | 32  | 0011 0010 | B     | 42  | 0100 0010 | b     | 62  | 0110 0010
| 3     | 33  | 0011 0011 | C     | 43  | 0100 0011 | c     | 63  | 0110 0011
| 4     | 34  | 0011 0100 | D     | 44  | 0100 0100 | d     | 64  | 0110 0100
| 5     | 35  | 0011 0101 | E     | 45  | 0100 0101 | e     | 65  | 0110 0101
| 6     | 36  | 0011 0110 | F     | 46  | 0100 0110 | f     | 66  | 0110 0110
| 7     | 37  | 0011 0111 |       |     |           |       |     |
| 8     | 38  | 0011 1000 |       |     |           |       |     |
| 9     | 39  | 0011 1001 |       |     |           |       |     |

From the above one can observe that the second most significant bit can be used
to determine whether the input is 0xA (10 in decimal) or above, and that the
lower nibble can be used (almost) as-is in either case, and by _almost_ I mean
that if the input is indeed 10 or above, we can always add 9 to the lower nibble
to get the actual value.

In (pseudo-)code this means
```C
// char hex = ...
uint8_t dec = (hex & 0x0f);   // always use the lowest 4 bits
bool add_more = (hex & 0x40); // ??
if(add_more) dec += 9;        // profit
```
which we can improve to make it a neat (or ugly, YMMV) one-liner by replacing
the `if` branch with some basic arithmetic, like
```C
(hex & 0x0f) + (!!(hex & 0x40)) * 9
```
or, even better like
```C
(hex & 0x0f) + (hex >> 6) * 9
```
which can be trivially used in a loop to decode a buffer of hex characters. The
latter variant also seems to compile without resulting in actual branching
instructions. The most significant bit is assumed to be always 0. See the
implementation in `hex2bin.h`.

It should be noted that the single mask bit (`0x40`) works for both lowercase
as well as uppercase hexadecimal characters, i.e. valid inputs are the
characters `0123456789ABCDEFabcdef`.

## Test
Run the snippet below for a simple test
```C
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hex2bin.h"

int main(void){
  /* check all possible 16-bit values in network byte order */
  unsigned i;
  for(i = 0; i <= 0xffff; ++i){
    /* encode to hex string using a function from the standard library */
    char hex[5];
    snprintf(hex, sizeof(hex), "%02hhX%02hhx", i>>8, i);

    /* decode from hex string using our implementation */
    unsigned char bin[2];
    unsigned n = hex2bin(hex, sizeof(hex), bin, sizeof(bin));
    assert(n == sizeof(bin));

    /* decoded matches the original */
    const unsigned char org[2] = {i>>8, i};
    assert(!memcmp(org, bin, sizeof(bin)));
  }
  return 0;
}
```

## Disclaimer
The algorithm assumes valid input, and no effort is made to detect invalid input.

## Author
Tero 'stedo' Liukko

## License
MIT
