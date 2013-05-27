#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int  cache_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	md5_state target, tmp;
	uint32_t *m;
	uint8_t b1, b2, b3, b4;
	uint8_t b9;
	int i;
	
#ifdef DEBUG
	printf("Attack called with the following parameters:\n");
	printf("%.8x %.8x %.8x %.8x\n", a, b, c, d); 
#endif


	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));
	m[1] = 0x41414141;


	for (b9 = BYTES_BEGIN; b9 <= BYTES_END; b9++) {

		float percentage_done = 100* ((float) b9 - BYTES_BEGIN) / BYTES_BASE;
		printf("  Percentage through the whole space: %.2f\r", percentage_done );

		/* Reset the target */
		target.a = a - h0;
		target.b = b - h1;
		target.c = c - h2;
		target.d = d - h3;

		/* Fix some m2 */
		m[2] = 0x00008000 | b9;

		/* Define the length of the message. */
		m[14] = length*8;

		/* Calculate backwards and store the result */
		/* (note that m[0] is not of any importance during these calculations) */
		for (i = 63; i > 48; i--) {
			md5_round_backwards(&target, m, i);
		}

		/* At this point, we're looking for a value that after round 48 
		 * has state *ap *bp *cp *dp. */

		/* Iterate over all m0s */
		for (b1 = BYTES_BEGIN; b1 < BYTES_END; b1++) {

			for (b2 = BYTES_BEGIN; b2 < BYTES_END; b2++) {
				for (b3 = BYTES_BEGIN; b3 < BYTES_END; b3++) {
					for (b4 = BYTES_BEGIN; b4 < BYTES_END; b4++) {
						if (m[2] == 0x8042 && m[0] == 0x41414141) {
							// printf("%.08x %.08x %.08x %.08x %.08x \n", m[0], m[1], m[2], m[3], m[14]);
						}

						/* Establish current m0 */
						m[0] = b4 << 24 | b3 << 16 | b2 << 8 | b1;

						/* Calculate forward until after round 48 */
						md5_truncated(&tmp, m, 48);

						if (target.a == tmp.a &&
							target.b == tmp.b &&
							target.c == tmp.c &&
							target.d == tmp.d) {
								free(m);
								return TRUE;
						}
					}
				}
			}
		}
	}
	free(m);
	return FALSE;
}

