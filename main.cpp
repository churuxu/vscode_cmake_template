#include <stdio.h>
#include "crc16.h"
#include "zlib.h"
#include "json.h"
#include "duktape.h"

int main(){
	uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
	printf("123456\n");
	printf("crc16=%04x\n", (int)crc);	
	
	uint32_t crc32 = crc32_z(0, (uint8_t*)"123456", 6);
	printf("crc32=%08x\n", crc32);
	auto json = json_parse("{\"id\":123}", 3);
	printf("%d\n", (int)(*json)["id"]);

	auto duk = duk_create_heap_default();
	duk_eval_string(duk, "1+2");
	printf("%d\n", duk_to_int(duk, -1));
	return 0;
	
}




