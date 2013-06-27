#include <stdio.h>
#include <stdlib.h>
#include "md5.h"
#include <math.h>

extern int get_candidate_word(char bytes_begin, int bytes_base, int strength);

MAYBE_INLINE int  cache_attack(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	md5_state target, tmp;
	uint32_t *m;
	int m1cnt, m1num ;
	uint8_t b1, b2, b3, b4;
	uint8_t b9;
	int i;
	int bytes_base;
	
	bytes_base = bytes_end - bytes_begin + 1;

	i = 0;

	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));
	m[14] = length*8;

	m1num = (int) pow(bytes_base, 4);
	for (m1cnt = 0; m1cnt < m1num; m1cnt++) {	
		m[1] = get_candidate_word(bytes_begin, bytes_base, m1cnt);

		for (b9 = bytes_begin; b9 <= bytes_end; b9++) {
			/* Reset the target */
			target.a = a - h0;
			target.b = b - h1;
			target.c = c - h2;
			target.d = d - h3;

			/* Fix some m2 */
			m[2] = 0x00008000 | b9;

			/* Calculate backwards and store the result */
			/* (note that m[0] is not of any importance during these calculations) */
			md5_64to48_fast(&target,m);

			/* At this point, we're looking for a value that after round 48 
			 * has state *ap *bp *cp *dp. */

			/* Iterate over all m0s */
			for (b1 = bytes_begin; b1 <= bytes_end; b1++) {
			for (b2 = bytes_begin; b2 <= bytes_end; b2++) {
			for (b3 = bytes_begin; b3 <= bytes_end; b3++) {
			for (b4 = bytes_begin; b4 <= bytes_end; b4++) {
				i++;

				/* Establish current m0 */
				m[0] = b4 << 24 | b3 << 16 | b2 << 8 | b1;

				/* Calculate forward until after round 48 */
				md5_0to48_fast(&tmp, m);

				if (target.a == tmp.a &&
					target.b == tmp.b &&
					target.c == tmp.c &&
					target.d == tmp.d) {
						free(m);

						printf("Found at iteration %d\n", i);
						return TRUE;
				}
			}}}}
		}
	}
	free(m);
	return FALSE;
}

