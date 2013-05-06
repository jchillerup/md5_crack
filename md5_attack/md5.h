#include <stdint.h>

struct md5_state {
	uint32_t* a;
	uint32_t* b;
	uint32_t* c;
	uint32_t* d;
};

void md5_round(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, char* m, int r);
char * md5(char * input);