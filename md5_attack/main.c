#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "md5.h"
#include "main.h"

extern int        get_candidate_word(char bytes_begin, int bytes_base, int strength);
extern int        cache_attack(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length);
extern int        mitm_attack(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length);
extern int        naive_search(char bytes_begin, char bytes_end, uint32_t a, uint32_t b, uint32_t c, uint32_t d, int length);
extern void       strtomd5(char* string, md5_state* target);
extern uint32_t   byteswap(uint32_t i);

void get_candidate(char bytes_begin, int bytes_base, int strength, char* target) {
	char* ptr;
	uint32_t m1;
	ptr = target;
	m1 = get_candidate_word(bytes_begin, bytes_base, strength);
	memcpy(ptr, "AAAA", 4); ptr+=4;
	memcpy(ptr, &m1, 4); ptr += 4;
	*ptr = 'A';
}

void benchmark() {
	char * input;
	md5_state test_target;
	int i = 0;
	int ret = FALSE;

	char bytes_begin = 'A';
	char bytes_end   = 'G';
	int  bytes_base   = bytes_end - bytes_begin;

	input = (char*) calloc(11, sizeof(char));
	get_candidate(bytes_begin, bytes_base, 450, input);

	printf("Benchmarking with password: %s\n", input);

	test_target = md5(input);

	printf("Looking for %.08x %.08x %.08x %.08x\n", test_target.a, test_target.b, test_target.c, test_target.d);

	ret = naive_search(bytes_begin, bytes_end, test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
	ret = cache_attack(bytes_begin, bytes_end, test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
	ret = mitm_attack(bytes_begin, bytes_end, test_target.a, test_target.b,  test_target.c, test_target.d, 9); printf("%d\n", ret);
}


int main(int argc, char ** argv) {
	char * input;
	
	char bytes_begin, bytes_end;
	int bytes_base;

	bytes_begin = 'A';
	bytes_end   = 'G';

	if (argc < 3) {
		benchmark();
#ifdef _WIN32
		scanf("%d", &input);
#endif

	} else {
		clock_t cl;
		int ret;
		md5_state target;
		
		strtomd5(argv[4], &target);
		
		printf("Looking for %.08x %.08x %.08x %.08x\n", target.a, target.b, target.c, target.d);

		bytes_begin = argv[2][0];
		bytes_end   = argv[3][0];

		switch(argv[1][0]) {
		case 'n':
			ret = naive_search(bytes_begin, bytes_end, target.a, target.b, target.c, target.d, 9);
                        printf("%d", ret);
			break;
		case 'c':
			ret = cache_attack(bytes_begin, bytes_end, target.a, target.b, target.c, target.d, 9);
                        printf("%d", ret);
			break;
		case 'm':
			ret = mitm_attack(bytes_begin, bytes_end, target.a, target.b, target.c, target.d, 9);
                        printf("%d", ret);
			break;
		}

	}
	
	return(EXIT_SUCCESS);
}
