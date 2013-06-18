#include <stdint.h>
#include <string.h>
#include "md5.h"

uint32_t get_candidate_word(char bytes_begin, int bytes_base, int number) {
	uint8_t ba, bb, bc, bd;
	
	bd = number % bytes_base; number /= bytes_base;
	bc = number % bytes_base; number /= bytes_base;
	bb = number % bytes_base; number /= bytes_base;
	ba = number % bytes_base; number /= bytes_base;
	
	return (bytes_begin + bd) << 24 | (bytes_begin + bc) << 16 | (bytes_begin + bb) << 8 | (bytes_begin + ba);
}

uint32_t byteswap(uint32_t i) {
    uint8_t c1, c2, c3, c4;    
 
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
 
    return ((uint32_t)c1 << 24) + ((uint32_t)c2 << 16) + ((uint32_t)c3 << 8) + c4;
}

void strtomd5(char* string, md5_state* target) {
	char tmp[8];
	
	strncpy(tmp, string, 8);
	target->a = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+8, 8);
	target->b = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+16, 8);
	target->c = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+24, 8);
	target->d = byteswap(strtol(tmp, NULL, 16));
}
