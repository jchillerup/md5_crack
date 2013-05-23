#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int  cache_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	md5_state target, tmp;
	uint32_t *m;
	uint8_t b1, b2, b3, b4;
	int i;
	
#ifdef DEBUG
	printf("Attack called with the following parameters:\n");
	printf("%.8x %.8x %.8x %.8x\n", a, b, c, d); 
#endif

	target.a = a - h0;
	target.b = b - h1;
	target.c = c - h2;
	target.d = d - h3;

	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));

	/* Fix some m1, m2 */
	m[1] = 0x00000080; /* TODO: padding for a 4-char message */
	m[2] = 0x00000000;

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
	/* TODO: before looping through any of the inner loops, do a test with subsequent bytes = 0x00 */
	for (b1 = 0x20; b1 < 0x7f; b1++) {
		float percentage_done = 100* ((float) b1-0x20) / 0x7f;
		printf("  Percentage through the whole space: %.2f\r", percentage_done );

		for (b2 = 0x20; b2 < 0x7f; b2++) {

			for (b3 = 0x20; b3 < 0x7f; b3++) {

				for (b4 = 0x20; b4 < 0x7f; b4++) {
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

	free(m);
	return FALSE;
}

