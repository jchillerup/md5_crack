#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int main(int argc, char ** argv) {
	char * hash = (char*) malloc(16);
	char * input = "ABCDABCDABCD";
	
	hash = md5(input);
	//scanf("%d");

	return(EXIT_SUCCESS);
}
