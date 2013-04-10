#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define F(b, c, d) (((b) & (c)) | ((~b) & (c)))
#define G(b, c, d) (((b) & (d)) | ((c) & (!d)))
#define H(b, c, d) ((b) ^ (c) ^ (d))
#define I(b, c, d) ((c) ^ ((b) | (~d)))

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
      
int m_idx[] = {	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
				1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 
				5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 
				0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 };

const uint32_t h0 = 0x67452301;
const uint32_t h1 = 0xefcdab89;
const uint32_t h2 = 0x98badcfe;
const uint32_t h3 = 0x10325476;

void md5_round(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, char* m, int r) {
	
}

void md5_round_backwards(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, char* m, int r) {
	
}

char * md5(char * input)
{
	uint32_t a = h0;
	uint32_t b = h1;
	uint32_t c = h2;
	uint32_t d = h3;

	char * m = (char*) calloc(4, 16);
	int input_length = strlen(input);
	char * pad_ptr = m;
	
	int i = 0;

	// We don't want inputs larger than what fits in the first three message words
	// (we can use strlen because there's no chance of 0x00s just yet.
	if (input_length > 3*sizeof(uint32_t)) {
		fprintf(stderr, "Password candidate too long.");
		exit(255);
	}
	
	// Do the padding. 
	memcpy(m, input, input_length);
	pad_ptr += input_length;
	*pad_ptr = (char) 0x80000000; // 0001 0000, but we're little endian
	
	for (i = 0; i<64; i++) {
		printf("%.2x ", m[i]);
	}
	
	// Calculate the hash value
	for (i = 0; i<64; i++) {
		md5_round(&a, &b, &c, &d, (m+m_idx[i]), i);
	}
	
}