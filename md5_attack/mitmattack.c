#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int  mitm_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {

	uint32_t* m = (uint32_t*) calloc(16, sizeof(uint32_t));
	
	m[1] = 0x41414141; 
	
	printf("Calculating forward chain.\n");
	/* Make forward chain */
	for (int a = BYTES_BEGIN; a < BYTES_END; a++) {
	for (int b = BYTES_BEGIN; b < BYTES_END; b++) {
	for (int c = BYTES_BEGIN; c < BYTES_END; c++) {
	for (int d = BYTES_BEGIN; d < BYTES_END; d++) {
		md5_state res;
		m[0] = d << 24 | c << 16 | b << 8 | a;
		
		res = md5_truncated(m, 1);
	}
	}
	}
	}
	
	printf("Done.\n");

	return FALSE;

}