#include <stdio.h>
#include "crc16.h"
#include "zlib.h"
#include "json.h"
#include "duktape.h"
#include "curl/curl.h"


static size_t WriteFunction(void *input, size_t uSize, size_t uCount, void *avg)
{
	fwrite(input,1,uCount,stdout);
	return uCount;
}
int test_http(){	
	CURL* pCurl;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	
	pCurl = curl_easy_init();
	if (pCurl == NULL) {
		printf("curl_easy_init() error");
		return -1;
	}
	

	curl_easy_setopt(pCurl, CURLOPT_URL, "https://www.baidu.com"); 	// 访问的URL
	
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);	// !数据回调函数
	
	curl_easy_perform(pCurl);

	return 0;
}


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

	test_http();

	return 0;
	
}




