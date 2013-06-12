#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "md5.h"
#include "main.h"

extern int get_candidate_word(int strength);

void get_candidate(int strength, char* target) {
	char* ptr;
	uint32_t m1;
	ptr = target;
	m1 = get_candidate_word(strength);
	memcpy(ptr, "AAAA", 4); ptr+=4;
	memcpy(ptr, &m1, 4); ptr += 4;
	*ptr = 'A';
}

void benchmark() {
	char * input;
	md5_state test_target;
	clock_t cl;
	int i = 0;
	int ret = FALSE;

	input = (char*) calloc(11, sizeof(char));
	get_candidate(450, input);

	printf("Benchmarking with password: %s\n", input);

	test_target = md5(input);


	printf("Looking for %.08x %.08x %.08x %.08x\n", test_target.a, test_target.b, test_target.c, test_target.d);

	ret = naive_search(test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
	ret = cache_attack(test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
	ret = mitm_attack(test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
}

uint32_t byteswap(uint32_t i) {
    uint8_t c1, c2, c3, c4;    
 
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
 
    return ((uint32_t)c1 << 24) + ((uint32_t)c2 << 16) + ((uint32_t)c3 << 8) + c4;
}

void strtomd5(char* string, md5_state* target) {
	char tmp[8];
	
	strncpy(tmp, string, 8);
	target->a = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+8, 8);
	target->b = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+16, 8);
	target->c = byteswap(strtol(tmp, NULL, 16));
	strncpy(tmp, string+24, 8);
	target->d = byteswap(strtol(tmp, NULL, 16));
}

int main(int argc, char ** argv) {
	char * input;
	
	if (argc < 3) {
		benchmark();
	} else {
		clock_t cl;
		int ret;
		md5_state target;
		
		strtomd5(argv[2], &target);
		
		switch(argv[1][0]) {
		case 'n':
			ret = naive_search(target.a, target.b, target.c, target.d, 9);
			break;
		case 'c':
			ret = cache_attack(target.a, target.b, target.c, target.d, 9);
			break;
		case 'm':
			ret = mitm_attack(target.a, target.b, target.c, target.d, 9);
			break;
		}
	}
	
#ifdef _WIN32
	scanf("%d", &input);
#endif

	return(EXIT_SUCCESS);
}
