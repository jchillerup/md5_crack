#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "md5.h"
#include <time.h>

typedef struct  {
	uint32_t b;
	uint32_t c;
} md5_state_reduced;

extern int get_candidate_word(char bytes_begin, int bytes_base, int strength);

int  mitm_attack(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	//clock_t cl;
	uint32_t *m;
	uint8_t ba, bb, bc, bd;
	int m1cnt, m1num ;
	int fsize, bsize, i;
	float online_time;
	md5_state tmp;
	int bytes_base;

	/* These structures contain the forward and backward strands of MD5 calculation */
	md5_state_reduced  *forward_chain, *fptr;
	md5_state *backward_chain, *bptr;
	
	bytes_base = bytes_end - bytes_begin + 1;

	fsize = (int) pow(bytes_base, 4);
	bsize = (int) pow(bytes_base, 1);
	
	/* Allocs, be sure to free these */
	m = (uint32_t*) calloc(16, sizeof(uint32_t));
	
	m[14] = length*8;
	
	forward_chain  = (md5_state_reduced*) calloc(fsize, sizeof(md5_state_reduced));
	backward_chain = (md5_state*)         calloc(bsize, sizeof(md5_state));

	online_time = 0;
	m1num = (int) pow(bytes_base, 4);
	for (m1cnt = 0; m1cnt < m1num; m1cnt++) {

		m[1] = get_candidate_word(bytes_begin, bytes_base, m1cnt);

		// BACKWARD CHAIN
		bptr = backward_chain;
		for  (ba = bytes_begin; ba <= bytes_end; ba++) {
			m[2] = 0x00008000 | ba;

			bptr->a = a - h0;
			bptr->b = b - h1;
			bptr->c = c - h2;
			bptr->d = d - h3;

			md5_64to48_fast(bptr,m);

			bptr++;
		}

		// FORWARD CHAIN
		fptr = forward_chain;
		for (ba = bytes_begin; ba <= bytes_end; ba++) {
		for (bb = bytes_begin; bb <= bytes_end; bb++) {
		for (bc = bytes_begin; bc <= bytes_end; bc++) {
		for (bd = bytes_begin; bd <= bytes_end; bd++) {
			m[0] = bd << 24 | bc << 16 | bb << 8 | ba;
			
			md5_truncated(&tmp, m, 1);

			fptr->b = tmp.b;
			fptr->c = tmp.c;

			fptr++;
		}}}}
		
		// ONLINE PHASE
		bptr = backward_chain;
		for(bptr = backward_chain; bptr < (backward_chain + bsize); bptr++) {
			md5_state tmp;
			m[2] = 0x00008000 | (bytes_begin + (bptr - backward_chain));

			// So we have the value for this particular value for m2. Check
			// if it matches a value from m0:
			fptr = forward_chain; 
			
			for (ba = bytes_begin; ba <= bytes_end; ba++) {
			for (bb = bytes_begin; bb <= bytes_end; bb++) {
			for (bc = bytes_begin; bc <= bytes_end; bc++) {
			for (bd = bytes_begin; bd <= bytes_end; bd++) {
				tmp.a = bptr->a;
				tmp.b = bptr->b;
				tmp.c = bptr->c;
				tmp.d = bptr->d;

				m[0] = bd << 24 | bc << 16 | bb << 8 | ba;
				md5_48to1_fast(&tmp, m);

				if (tmp.a == 0x98badcfe && tmp.b == fptr->b && tmp.c == fptr->c && tmp.d == 0xefcdab89) {
					return TRUE;
				}
					
				fptr++;
			}}}}	

		}
				
	}
	return FALSE;

}