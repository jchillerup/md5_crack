#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "md5.h"

int  mitm_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	uint32_t *m;
	uint8_t ba, bb, bc, bd;

	int fsize, bsize, i;
	
	/* These structures contain the forward and backward strands of MD5 calculation */
	md5_state *forward_chain, *backward_chain, *fptr, *bptr;
	
	fsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 4);
	bsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 1);
	
	/* Allocs, be sure to free these */
	m = (uint32_t*) calloc(16, sizeof(uint32_t));

	forward_chain  = (md5_state*) calloc(fsize, sizeof(md5_state));
	backward_chain = (md5_state*) calloc(bsize, sizeof(md5_state));

	fptr = forward_chain;
	bptr = backward_chain;

	m[1]  = 0x41414141; 
	m[14] = length*8;

	// FORWARD CHAIN
	printf("  + Calculating forward chain.\n");
	for (ba = BYTES_BEGIN; ba <= BYTES_END; ba++) {
	for (bb = BYTES_BEGIN; bb <= BYTES_END; bb++) {
	for (bc = BYTES_BEGIN; bc <= BYTES_END; bc++) {
	for (bd = BYTES_BEGIN; bd <= BYTES_END; bd++) {
		m[0] = bd << 24 | bc << 16 | bb << 8 | ba;
		
		md5_truncated(fptr, m, 1);

		fptr++;
	}
	}
	}
	}
	

	// BACKWARD CHAIN
	printf("  + Calculating backward chain.\n");
	for  (ba = BYTES_BEGIN; ba <= BYTES_END; ba++) {
		m[2] = 0x00008000 | ba;
		
		bptr->a = a - h0;
		bptr->b = b - h1;
		bptr->c = c - h2;
		bptr->d = d - h3;

		for (i = 63; i > 48; i--) {
			md5_round_backwards(bptr, m, i);
		}
		
		bptr++;
	}


	// ONLINE PHASE
	printf("  + Online phase.\n");
	for(bptr = backward_chain; bptr < (backward_chain + bsize); bptr++) {
		md5_state tmp;
		printf(".");
		
		tmp.a = bptr->a;
		tmp.b = bptr->b;
		tmp.c = bptr->c;
		tmp.d = bptr->d;

		for (i = 48; i > 1; i--) {
			md5_round_backwards(&tmp, m, i);
		}


		// So we have the value for this particular value for m2. Check
		// if it matches a value from m0:
		for (fptr = forward_chain; fptr < (forward_chain + fsize); fptr++) {
			if (tmp.a == fptr->a && tmp.b == fptr->b && tmp.c == fptr->c && tmp.d == fptr->d) {
				// If it does, we found the preimage.
				printf("Found!\n");
				return TRUE;
			}
		}

	}
	

	return FALSE;

}