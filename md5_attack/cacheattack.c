#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int  cache_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	uint32_t *ap, *bp, *cp, *dp;
	uint32_t *m;
	uint8_t b1, b2, b3, b4;
	int i;
	
#ifdef DEBUG
	printf("Attack called with the following parameters:\n");
	printf("%.8x %.8x %.8x %.8x\n", a, b, c, d); 
#endif

	ap = &a;
	bp = &b;
	cp = &c;
	dp = &d;

	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));

	/* Fix some m1, m2 */
	m[1] = 0x00000080; /* TODO: padding for a 4-char message */
	m[2] = 0x00000000;

	/* Define the length of the message. */
	m[14] = length*8;

	/* Subtract the h0..3 */
	*ap -= h0;
	*bp -= h1;
	*cp -= h2;
	*dp -= h3;

	/* Calculate backwards and store the result */
	/* (note that m[0] is not of any importance during these calculations) */
	for (i = 63; i > 48; i--) {
		md5_round_backwards(&ap, &bp, &cp, &dp, m, i);
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
					md5_state forward_result;

					/* Establish current m0 */
					m[0] = b4 << 24 | b3 << 16 | b2 << 8 | b1;

					/* Calculate forward until after round 48 */
					forward_result = md5_truncated(m, 48);

					if (*(forward_result.a) == *ap &&
						*(forward_result.b) == *bp &&
						*(forward_result.c) == *cp &&
						*(forward_result.d) == *dp) {
							free(m);
							return TRUE;
					}

					/* Remember to free */
					free(forward_result.a);
					free(forward_result.b);
					free(forward_result.c);
					free(forward_result.d);
				}
			}
		}
	}

	free(m);
	return FALSE;
}
