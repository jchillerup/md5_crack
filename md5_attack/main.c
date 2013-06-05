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
	md5_state test_target;
	clock_t cl;
	int i = 0;
	char * input;
	int ret = FALSE;

	input = (char*) calloc(11, sizeof(char));
	get_candidate(450, input);

	printf("Benchmarking with password: %s\n", input);

	test_target = md5(input);


	printf("Looking for %.08x %.08x %.08x %.08x\n", test_target.a, test_target.b, test_target.c, test_target.d);

		/*
	printf("\nNaively searching...\n");
	cl = clock();
	ret = naive_search(test_target.a, test_target.b,  test_target.c, test_target.d, 4);
	cl = clock() - cl;
	
	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n\n", ((float)cl)/CLOCKS_PER_SEC);
	*/


	printf("Performing cache attack...\n");
	cl = clock();
	ret = cache_attack(test_target.a, test_target.b,  test_target.c, test_target.d, 9);
	cl = clock() - cl;

	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n\n", ((float)cl)/CLOCKS_PER_SEC);
	
	
	printf("Performing meet-in-the-middle attack...\n");
	cl = clock();
	ret = mitm_attack(test_target.a, test_target.b,  test_target.c, test_target.d, 9);
	cl = clock() - cl;

	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n\n", ((float)cl)/CLOCKS_PER_SEC);
	

}

uint32_t byteswap(uint32_t i) {
    char *c = (char *) &i;
    return * (uint32_t *) (char[]) {c[3], c[2], c[1], c[0] };
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
	
	if (argc < 2) {
		benchmark();
	} else {
		clock_t cl;
		int ret;
		md5_state target;
		printf("%s;", argv[1]);
		
		strtomd5(argv[1], &target);
		
		cl = clock();
		ret = cache_attack(target.a, target.b, target.c, target.d, 9);
		cl = clock() - cl;
		if (ret == 1) {
			printf("%f;", ret, ((float)cl)/CLOCKS_PER_SEC);
		} else {
			printf("ERROR;");
		}
		
		cl = clock();
		ret = mitm_attack(target.a, target.b, target.c, target.d, 9);
		cl = clock() - cl;
		if (ret == 1) {
			printf("%f\n", ret, ((float)cl)/CLOCKS_PER_SEC);
		} else {
			printf("ERROR\n");
		}
	}
	
#ifdef _WIN32
	scanf("%d", &input);
#endif

	return(EXIT_SUCCESS);
}
