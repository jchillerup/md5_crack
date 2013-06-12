#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "md5.h"

int  naive_search(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	md5_state tmp;
	uint32_t *m;
	int m1cnt, m1num ;
	uint8_t b1, b2, b3, b4;
	uint8_t b9;
	int i;
	
	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));
	
	m1num = (int) pow(BYTES_BASE, 4);
	for (m1cnt = 0; m1cnt < m1num; m1cnt++) {	
		m[1] = get_candidate_word(m1cnt);

		for (b9 = BYTES_BEGIN; b9 <= BYTES_END; b9++) {
			/* Fix some m2 */
			m[2] = 0x00008000 | b9;

			/* Define the length of the message. */
			m[14] = length*8;

			/* Iterate over all m0s */
			for (b1 = BYTES_BEGIN; b1 <= BYTES_END; b1++) {
			for (b2 = BYTES_BEGIN; b2 <= BYTES_END; b2++) {
			for (b3 = BYTES_BEGIN; b3 <= BYTES_END; b3++) {
			for (b4 = BYTES_BEGIN; b4 <= BYTES_END; b4++) {
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
