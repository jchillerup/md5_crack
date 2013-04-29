#include <stdio.h>
#include <stdlib.h>
#include "md5.h"


void attack(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
	
} 


int main(int argc, char ** argv) {
	int i = 0;
	char * input;
	input = "ABCDABCDABCD";
	
	for (i = 0; i<1; i++) {
		md5(input);
	}
	
	return(EXIT_SUCCESS);
}
