#include <stdio.h>
#include "crc16.h"
#include "zlib.h"


int main(){
	uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
	printf("123456\n");
	printf("crc16=%04x\n", (int)crc);	
	
	uint32_t crc32 = crc32_z(0, (uint8_t*)"123456", 6);
	printf("crc32=%08x\n", crc32);
	
	return 0;
}




