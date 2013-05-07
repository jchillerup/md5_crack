#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

#define TRUE 1
#define FALSE 0

/* The main attack. Takes a hash value a,b,c,d and tries to determine a preimage. */
int  attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
	uint32_t *ap, *bp, *cp, *dp;
	uint32_t *m;

	uint8_t b1, b2, b3, b4;
	int i;

	ap = &a;
	bp = &b;
	cp = &c;
	dp = &d;

	/* Initialize the message data structure. */
	m = (uint32_t *) calloc(16, sizeof(uint32_t));

	/* Fix some m1, m2 */
	m[1] = 0x00000000;
	m[2] = 0x00000000;

	/* Define the length of the message. TODO: fix */
	m[14] = 4*8;

	/* Subtract the h0..3 */
	*ap -= h0;
	*bp -= h1;
	*cp -= h2;
	*dp -= h3;

	/* Calculate backwards and store the result */
	/* (note that m[0] is not of any importance during these calculations) */
	for (i = 63; i > 48; i--) {
		md5_round_backwards(&ap, &bp, &cp, &dp, m, i);
		printf("%d: %.8x %.8x %.8x %.8x\n", i, *ap, *bp, *cp, *dp); 
	}

	printf("Value at pivot: \n");
	printf("%.8x %.8x %.8x %.8x\n", *ap, *bp, *cp, *dp); 

	/* At this point, we're looking for a value that after round 48 
	 * has state *ap *bp *cp *dp. */

	/* Iterate over all m0s */
	/* TODO: before looping through any of the inner loops, do a test with subsequent bytes = 0x00 */
	for (b1 = 0x20; b1 < 0x7f; b1++) {
		float percentage_done = 100* ((float) b1-0x20) / 0x7f;
		printf("%.2f\n", percentage_done );

		for (b2 = 0x20; b2 < 0x7f; b2++) {

			for (b3 = 0x20; b3 < 0x7f; b3++) {

				for (b4 = 0x20; b4 < 0x7f; b4++) {
					md5_state forward_result;

					/* Establish current m0 */
					m[0] = b1 << 24 | b2 << 16 | b3 << 8 | b4;

					/* Calculate forward until after round 48 */
					forward_result = md5_truncated((char*) m, 48);

					/* printf("%.8x %.8x %.8x\n", m[0], m[1], m[2]); */

					if (b1 == 0x20 && b2 == 0x20 && b3 == 0x20 && b4 == 0x20) 
						printf("%.8x %.8x %.8x %.8x\n", *(forward_result.a), *(forward_result.b), *(forward_result.c), *(forward_result.d)); 

					if (*(forward_result.a) == *ap &&
						*(forward_result.b) == *bp &&
						*(forward_result.c) == *cp &&
						*(forward_result.d) == *dp) {
							printf("found");
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

	return FALSE;
}


int main(int argc, char ** argv) {
	md5_state test_target;
	int i = 0;
	char * input;
	int ret = FALSE;

	input = "    ";
	test_target = md5(input);
	
	printf("Searching for:\n");
	printf("%.8x %.8x %.8x %.8x\n", *test_target.a, *test_target.b, *test_target.c, *test_target.d); 
	
	// TODO: Make the output of the initial MD5 call act as 
	//       the input to the attack.
	ret = attack(*test_target.a, *test_target.b,  *test_target.c, *test_target.d);

	printf("%d", ret);
	scanf("%d", &input);

	return(EXIT_SUCCESS);
}
