#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "md5.h"

int  mitm_attack(uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length) {
	
	uint32_t* m = (uint32_t*) calloc(16, sizeof(uint32_t));
	uint32_t *pa, *pb, *pc, *pd;
	
	int fsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 4);
	int bsize = (int) pow(BYTES_END - BYTES_BEGIN + 1, 1);
	
	pa = (uint32_t*) malloc(sizeof(uint32_t));
	pb = (uint32_t*) malloc(sizeof(uint32_t));
	pc = (uint32_t*) malloc(sizeof(uint32_t));
	pd = (uint32_t*) malloc(sizeof(uint32_t));
	
	/* These structures contain the forward and backward strands of MD5 calculation */
	md5_state* forward_chain  = (md5_state*) calloc(fsize, sizeof(md5_state));
	md5_state* backward_chain = (md5_state*) calloc(bsize, sizeof(md5_state));
	
	md5_state* fptr = forward_chain;
	md5_state* bptr = backward_chain;

	/* */
	m[1] = 0x41414141; 
	
	printf("  + Calculating forward chain.\n");
	/* Make forward chain */
	for (uint8_t ba = BYTES_BEGIN; ba < BYTES_END; ba++) {
	for (uint8_t bb = BYTES_BEGIN; bb < BYTES_END; bb++) {
	for (uint8_t bc = BYTES_BEGIN; bc < BYTES_END; bc++) {
	for (uint8_t bd = BYTES_BEGIN; bd < BYTES_END; bd++) {
		md5_state res;
		m[0] = bd << 24 | bc << 16 | bb << 8 | ba;
		
		res = md5_truncated(m, 1);
		
		fptr->a = res.a;
		fptr->b = res.b;
		fptr->c = res.c;
		fptr->d = res.d;
		
		fptr++;
		
		free(res.a);
		free(res.b);
		free(res.c);
		free(res.d);
	}
	}
	}
	}
	
	
	printf("  + Calculating backward chain.\n");
	
	for  (uint8_t a = BYTES_BEGIN; a < BYTES_END; a++) {
		md5_state res;
		m[2] = 0x00008000 | a;
		
		*pa = a - h0;
		*pb = b - h1;
		*pc = c - h2;
		*pd = d - h3;
		
		for (int i = 63; i > 48; i--) {
			md5_round_backwards(&pa, &pb, &pc, &pd, m, i);
		}
		
		bptr->a = pa;
		bptr->b = pb;
		bptr->c = pc;
		bptr->d = pd;
	}
	
	printf("Done.\n");



	printf("  + Online phase.\n");
	for(fptr = forward_chain; fptr < forward_chain + fsize; fptr++) {
	for(bptr = backward_chain; bptr < backward_chain + bsize; bptr++) {
	
		
	
	}
	}


	free(pa);
	free(pb);
	free(pc);
	free(pd);
	

	return FALSE;

}