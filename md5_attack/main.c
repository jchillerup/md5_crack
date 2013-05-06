#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

#define TRUE 1
#define FALSE 0

/* The main attack. Takes a hash value a,b,c,d and tries to determine a preimage. */
int  attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
	uint32_t m0, m1, m2;
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
	m1 = 0x00000000;
	m2 = 0x00000000;

	/* Subtract the h0..3 */


	/* Calculate backwards and store the result */
	for (i = 63; i > 48; i--) {
		md5_round_backwards(&ap, &bp, &cp, &dp, m, i);
	}

	/* At this point, we're looking for a value that after round 48 
	 * has state *ap *bp *cp *dp. */

	/* Iterate over all m0s */
	for (b1 = 0x20; b1 < 0x7f; b1++)
	for (b2 = 0x20; b2 < 0x7f; b2++)
	for (b3 = 0x20; b3 < 0x7f; b3++)
	for (b4 = 0x20; b4 < 0x7f; b4++) {
		/* Establish current m0 */
		m0 = b1 << 24 | b2 << 16 | b3 << 8 | b4;
		m0 = m0;

		/* Calculate forward until after round 48 */
	}

	return FALSE;
}


int main(int argc, char ** argv) {
	int i = 0;
	char * input;
	input = "ABC";
	
	for (i = 0; i<1; i++) {
		md5(input);
	}
	
	// TODO: Make the output of the initial MD5 call act as 
	//       the input to the attack.

	attack(0xd2bd2f90, 0x4f0cdfb1, 0xd2a5b470, 0x32e92535);
	
	scanf("%d", &input);

	return(EXIT_SUCCESS);
}
