#include <stdint.h>

extern const uint32_t h0;
extern const uint32_t h1;
extern const uint32_t h2;
extern const uint32_t h3;
extern const int m_idx[];

typedef struct  {
	uint32_t* a;
	uint32_t* b;
	uint32_t* c;
	uint32_t* d;
} md5_state;



void md5_round(uint32_t** a, uint32_t** b, uint32_t** c, uint32_t** d, uint32_t* m, int r);
void md5_round_backwards(uint32_t** a, uint32_t** b, uint32_t** c, uint32_t** d, uint32_t* m, int r);
md5_state md5(char * input);
md5_state md5_truncated(char * input, int stop_after_round);