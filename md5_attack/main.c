#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "md5.h"
#include "main.h"

#ifdef asdf
#include "cacheattack.c"
#include "naivesearch.c"
#include "mitmattack.c"
#endif



int main(int argc, char ** argv) {
	md5_state test_target;
	clock_t cl;
	int i = 0;
	char * input;
	int ret = FALSE;

	input = "AA  ";
	test_target = md5(input);
	
	/*

	printf("\nNaively searching...\n");
	cl = clock();
	ret = naive_search(*test_target.a, *test_target.b,  *test_target.c, *test_target.d, 4);
	cl = clock() - cl;
	
	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n", ((float)cl)/CLOCKS_PER_SEC);



	printf("Performing cache attack...\n");
	cl = clock();
	ret = cache_attack(*test_target.a, *test_target.b,  *test_target.c, *test_target.d, 4);
	cl = clock() - cl;

	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n", ((float)cl)/CLOCKS_PER_SEC);

	*/ 
	
	printf("Performing meet-in-the-middle attack...\n");
	cl = clock();
	ret = mitm_attack(*test_target.a, *test_target.b,  *test_target.c, *test_target.d, 4);
	cl = clock() - cl;

	if (ret == TRUE) {
		printf("\n  Preimage found ");
	} else {
		printf("\n  Preimage not found ");
	}
	printf("after %f seconds.\n", ((float)cl)/CLOCKS_PER_SEC);


#ifdef WINDOWS
	scanf("%d", &input);
#endif

	return(EXIT_SUCCESS);
}
