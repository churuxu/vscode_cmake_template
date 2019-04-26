#include <stdio.h>
#include "crc16.h"


#ifndef UNITTEST
int main(){
	uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
	printf("hello world\n");
	printf("crc=%d\n", (int)crc);

	return 0;
}
#endif



