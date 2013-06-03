#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "md5.h"
#include <time.h>

int  mitm_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	clock_t cl;
	uint32_t *m;
	uint8_t ba, bb, bc, bd;
	int m1cnt, m1num ;
	int fsize, bsize, i;
	float online_time;
	
	/* These structures contain the forward and backward strands of MD5 calculation */
	md5_state *forward_chain, *backward_chain, *fptr, *bptr;
	
	fsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 4);
	bsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 1);
	
	/* Allocs, be sure to free these */
	m = (uint32_t*) calloc(16, sizeof(uint32_t));

	forward_chain  = (md5_state*) calloc(fsize, sizeof(md5_state));
	backward_chain = (md5_state*) calloc(bsize, sizeof(md5_state));

	online_time = 0;
	m1num = (int) pow(BYTES_BASE, 4);
	for (m1cnt = 0; m1cnt < m1num; m1cnt++) {
	
	m[1] = get_candidate_word(m1cnt);

	m[14] = length*8;

	// FORWARD CHAIN
	printf("  + Calculating forward chain.\n");
	fptr = forward_chain;
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
	bptr = backward_chain;
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
	cl = clock();
	printf("  + Online phase.\n");
	bptr = backward_chain;
	for(bptr = backward_chain; bptr < (backward_chain + bsize); bptr++) {
		md5_state tmp;
		m[2] = 0x00008000 | (BYTES_BEGIN + (bptr - backward_chain));

		// So we have the value for this particular value for m2. Check
		// if it matches a value from m0:
		for (fptr = forward_chain; fptr < (forward_chain + fsize); fptr++) {
			int fpl;
			uint32_t ba, bb, bc, bd;

			tmp.a = bptr->a;
			tmp.b = bptr->b;
			tmp.c = bptr->c;
			tmp.d = bptr->d;

			fpl = fptr - forward_chain;	
			
			// Update m[0] and m[2] to corresponding values in base-BYTES_LENGTH
			bd = fpl % BYTES_BASE; fpl /= BYTES_BASE;
			bc = fpl % BYTES_BASE; fpl /= BYTES_BASE;
			bb = fpl % BYTES_BASE; fpl /= BYTES_BASE;
			ba = fpl % BYTES_BASE; fpl /= BYTES_BASE;
			
			assert(fpl == 0);

			m[0] = (BYTES_BEGIN + bd) << 24 | (BYTES_BEGIN + bc) << 16 | (BYTES_BEGIN + bb) << 8 | (BYTES_BEGIN + ba);
			
			for (i = 48; i > 1; i--) {
				md5_round_backwards(&tmp, m, i);
			}


			if (tmp.a == fptr->a && tmp.b == fptr->b && tmp.c == fptr->c && tmp.d == fptr->d) {
				// If it does, we found the preimage.
				printf("Found!\n");
				cl = clock() - cl;

				online_time += ((float)cl)/CLOCKS_PER_SEC;

				printf("Online time: %f sec.\n", online_time) ;

				return TRUE;
			}
		}

	}

	online_time += ((float)cl)/CLOCKS_PER_SEC;

	}
	cl = clock() - cl;
	online_time +=  ((float)cl)/CLOCKS_PER_SEC;
	printf("Online time: %f sec.\n", online_time);

	return FALSE;

}