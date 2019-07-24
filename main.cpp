#include <stdio.h>
#include "crc16.h"
#include "json-c/json.h"


int main(){
	uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
	printf("hello world\n");
	printf("crc=%d\n", (int)crc);	
	struct json_object* obj = json_tokener_parse("{\"id\":123,\"name\":\"Abc\"}");
	
	printf("str :%s\n", json_object_to_json_string(obj));
	return 0;
}




