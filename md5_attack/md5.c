#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

#ifdef _LIBC
# include <endian.h>
# if __BYTE_ORDER == __BIG_ENDIAN
#  define WORDS_BIGENDIAN 1
# endif
#endif

#ifdef WORDS_BIGENDIAN
# define SWAP(n)							\
    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#else
# define SWAP(n) (n)
#endif


#define F(b, c, d) (((b) & (c)) | ((~(b)) & (d)))
#define G(b, c, d) (((b) & (d)) | ((c) & (~(d))))
#define H(b, c, d) ((b) ^ (c) ^ (d))
#define I(b, c, d) ((c) ^ ((b) | (~(d))))

#define ROTATE_LEFT(w, s)  (w = (w << s) | (w >> (32 - s)))
#define ROTATE_RIGHT(w, s) (w = (w >> s) | (w << (32 - s)))

short s[] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
			  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
			  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
			  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

uint32_t k[] = 
    { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
      0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
      0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
      0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
      0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
      0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
      0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
      0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
      

const int m_idx[] = {	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
				1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 
				5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 
				0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 };

const uint32_t h0 = 0x67452301;
const uint32_t h1 = 0xefcdab89;
const uint32_t h2 = 0x98badcfe;
const uint32_t h3 = 0x10325476;

/* This array contains the bytes used to pad the buffer to the next
   64-byte boundary.  (RFC 1321, 3.1: Step 1)  */
static const uint32_t fillbuf[64] = { 0x80, 0 /* , 0, 0, ...  */ };

void md5_round(md5_state *state_ptr, uint32_t* m, int r) {
	uint32_t f_val;
	uint32_t new_b;
	uint32_t old_a;
	
	if      (r < 16) f_val = F(state_ptr->b, state_ptr->c, state_ptr->d);
	else if (r < 32) f_val = G(state_ptr->b, state_ptr->c, state_ptr->d);
	else if (r < 48) f_val = H(state_ptr->b, state_ptr->c, state_ptr->d);
	else             f_val = I(state_ptr->b, state_ptr->c, state_ptr->d);
	
	new_b  = state_ptr->a; 
	
	new_b += f_val;
	new_b += k[r];
	new_b += m[m_idx[r]];
	
	ROTATE_LEFT(new_b, s[r]);
	
	old_a = state_ptr->a;
	state_ptr->a = state_ptr->d;
	state_ptr->d = state_ptr->c;
	state_ptr->c = state_ptr->b;
	state_ptr->b = old_a;
	
	state_ptr->b = new_b + (state_ptr->c);
}

void md5_round_backwards(md5_state *state_ptr, uint32_t* m, int r) {
	uint32_t f_val;
	uint32_t new_a;
	uint32_t old_b;
	
	old_b = state_ptr->b;
	state_ptr->b = state_ptr->c;
	state_ptr->c = state_ptr->d;
	state_ptr->d = state_ptr->a;
	state_ptr->a = old_b;
	
	if      (r < 16) f_val = F(state_ptr->b, state_ptr->c, state_ptr->d);
	else if (r < 32) f_val = G(state_ptr->b, state_ptr->c, state_ptr->d);
	else if (r < 48) f_val = H(state_ptr->b, state_ptr->c, state_ptr->d);
	else             f_val = I(state_ptr->b, state_ptr->c, state_ptr->d);
	
	new_a = state_ptr->a - state_ptr->b;
	ROTATE_RIGHT(new_a, s[r]);
	
	new_a -= f_val;
	new_a -= k[r];
	new_a -= m[m_idx[r]];
	
	state_ptr->a = new_a;

	
	//printf("%.08x %.08x %.08x %.08x\n", state_ptr->a, state_ptr->b, state_ptr->c, state_ptr->d);
}

md5_state md5(char * input)
{
	md5_state ret;
	
	uint32_t *m = (uint32_t *) calloc(16, sizeof(uint32_t));
	int input_length = strlen(input);

	// We don't want inputs larger than what fits in the first three message words
	// (we can use strlen because there's no chance of 0x00s just yet.
	if (input_length > 3 * sizeof(uint32_t)) {
		fprintf(stderr, "Password candidate too long.");
		exit(255);
	}
	
	/* Do the padding. We need to consider m as a char pointer in order to properly
	 * point out the right address for the padding to start. */
	memcpy(m,                            input,      input_length);
	memcpy((char *) m + input_length,    fillbuf,    64 - input_length);
	
	// Add the length of the plaintext in bits
	m[14] = input_length * 8;
	
	// Calculate the hash value
	md5_truncated(&ret, m, 63);

#ifdef DEBUGS
	for (i = 0; i<16; i++) {
		printf("%.2x ", m[i]);
	}
	printf("\n");
#endif

	ret.a += h0;
	ret.b += h1;
	ret.c += h2;
	ret.d += h3;

	free(m);

	return ret;
}

void md5_truncated(md5_state* state_ptr, uint32_t * m, int stop_after_round)
{
	int i;

	state_ptr->a = h0;
	state_ptr->b = h1;
	state_ptr->c = h2;
	state_ptr->d = h3;

	// Calculate the hash value
	for (i = 0; i <= stop_after_round; i++) {
		md5_round(state_ptr, m, i);
	}
}