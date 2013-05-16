#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int iterate(int width) {
	int i, a;
	uint32_t counter;
	
	counter = 0x80;
	
	if (width < 2) { return -1; }
	
	for (i = 0; i < width; i++) {
		counter = counter << 8;
		
		for (a = 0x20; a < 0x7f; a++) {
			printf("%.8x\n", counter+a);
		}
	}
	
	
}


int main(int argc, char** argv) {

	iterate(2);

	return(0);

}
