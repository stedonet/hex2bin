#ifndef STEDO_HEX2BIN_H
#define STEDO_HEX2BIN_H
/*
MIT License

Copyright (c) 2022 Tero 'stedo' Liukko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/**
 * @brief decode a single case-insensitive hexadecimal ASCII character to its decimal value
 * @param[in] h ASCII character to decode
 * @return decimal value of the input
 * @note the input is assumed to be a valid
**/
static unsigned char unhex(char h){
  return ((h & 0x0f) + (h >> 6) * 9);
}


/**
 * @brief decode an array of hexadecimal ASCII characters to an array of bytes
 * @param[in]   hex   array of hex characters to decode
 * @param[in]   hlen  number of hex characters to decode
 * @param[out]  bin   buffer to write to
 * @param[in]   blen  maximum number of bytes to write
 * @return number of bytes written
 * @note hex input is assumed valid and its length a multiple of two
**/
static unsigned hex2bin(const char* hex, unsigned hlen, void* bin, unsigned blen){
  unsigned i, j;
  for(i = 0, j = 0; (i < blen) && (j+1 < hlen); ++i, j+=2){
    unsigned char hi = unhex(hex[j+0]);
    unsigned char lo = unhex(hex[j+1]);
    ((unsigned char*)bin)[i] = (hi << 4) | lo;
  }
  return i;
}


#endif /* STEDO_HEX2BIN_H */

