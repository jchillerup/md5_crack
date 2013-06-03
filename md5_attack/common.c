#include <stdint.h>
#include "md5.h"

uint32_t get_candidate_word(int number) {
	uint8_t ba, bb, bc, bd;

	bd = number % BYTES_BASE; number /= BYTES_BASE;
	bc = number % BYTES_BASE; number /= BYTES_BASE;
	bb = number % BYTES_BASE; number /= BYTES_BASE;
	ba = number % BYTES_BASE; number /= BYTES_BASE;
	
	return (BYTES_BEGIN + bd) << 24 | (BYTES_BEGIN + bc) << 16 | (BYTES_BEGIN + bb) << 8 | (BYTES_BEGIN + ba);
}
