#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int main(int argc, char ** argv) {
	int i = 0;
	char * input;
	input = "ABCDABCDABCD";
	
	for (i = 0; i<10000000; i++) {
		md5(input);
	}
	return(EXIT_SUCCESS);
}
