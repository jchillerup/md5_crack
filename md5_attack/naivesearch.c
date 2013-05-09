#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int  naive_search(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	uint8_t b1, b2, b3, b4;
	uint32_t* m;

	m = (uint32_t*) calloc(16, sizeof(uint32_t));

	/* Fix some m1, m2 */
	m[1] = 0x00000080; /* TODO: padding for a 4-char message */
	m[2] = 0x00000000;

	/* Define the length of the message. */
	m[14] = length*8;
	
	for (b1 = 0x20; b1 < 0x7f; b1++) {
		float percentage_done = 100* ((float) b1-0x20) / 0x7f;
		printf("  Percentage through the whole space: %.2f\r", percentage_done );

		for (b2 = 0x20; b2 < 0x7f; b2++) {

			for (b3 = 0x20; b3 < 0x7f; b3++) {

				for (b4 = 0x20; b4 < 0x7f; b4++) {
					md5_state forward_result;

					/* Establish current m0 */
					m[0] = b4 << 24 | b3 << 16 | b2 << 8 | b1;
					
					forward_result = md5_truncated(m, 63);

					if (*(forward_result.a) + h0 == a &&
						*(forward_result.b) + h1 == b &&
						*(forward_result.c) + h2 == c &&
						*(forward_result.d) + h3 == d) {
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
