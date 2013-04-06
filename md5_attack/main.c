#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int main(int argc, char ** argv) {
	char * hash = (char*) malloc(16);
	char * input = "AAAAAAAAAAAA";
	
	hash = md5(input);
	
	return(EXIT_SUCCESS);
}
