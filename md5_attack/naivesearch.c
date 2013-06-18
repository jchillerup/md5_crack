#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "md5.h"

extern int get_candidate_word(char bytes_begin, int bytes_base, int strength);

int  naive_search(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	md5_state tmp;
	uint32_t *m;
	int m1cnt, m1num ;
	uint8_t b1, b2, b3, b4;
	uint8_t b9;
	int i;
	int bytes_base;

	bytes_base = bytes_end - bytes_begin + 1;

	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));
	
	/* Define the length of the message. */
	m[14] = length*8;

	m1num = (int) pow(bytes_base, 4);
	for (m1cnt = 0; m1cnt < m1num; m1cnt++) {	
		m[1] = get_candidate_word(bytes_begin, bytes_base, m1cnt);

		for (b9 = bytes_begin; b9 <= bytes_end; b9++) {
			/* Fix some m2 */
			m[2] = 0x00008000 | b9;

			/* Iterate over all m0s */
			for (b1 = bytes_begin; b1 <= bytes_end; b1++) {
			for (b2 = bytes_begin; b2 <= bytes_end; b2++) {
			for (b3 = bytes_begin; b3 <= bytes_end; b3++) {
			for (b4 = bytes_begin; b4 <= bytes_end; b4++) {
				/* Establish current m0 */
				m[0] = b4 << 24 | b3 << 16 | b2 << 8 | b1;

				md5_truncated(&tmp, m, 63);

				if (a-h0 == tmp.a &&
					b-h1 == tmp.b &&
					c-h2 == tmp.c &&
					d-h3 == tmp.d) {
					free(m);
					return TRUE;
				}
			}
			}
			}
			}
		}
	}
	free(m);
	return FALSE;

}
