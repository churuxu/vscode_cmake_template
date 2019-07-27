#include <stdio.h>
#include "crc16.h"
#include "zlib.h"
#include "json.h"
#include "duktape.h"
#include "curl/curl.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("info:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("cert: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("pub: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("not cert！\n");
}


int test_ssl()
{
    int sockfd, len;   
    char buffer[1024];
    SSL_CTX *ctx;
    SSL *ssl;
	struct addrinfo* ai = NULL;
   
	printf("------------ test ssl -----------------\n");
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
   
	len = getaddrinfo("www.baidu.com","443",NULL,&ai);
	if(!ai){
		printf("dns error\n");
		return 1;
	}

    if ((sockfd = socket(ai->ai_family, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        return 1;
    }
    printf("socket created\n");  
   

    if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) != 0) {
        perror("Connect ");
        return 1;
    }
    printf("server connected\n");

   
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
   
    if (SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }
	int vresult = SSL_get_verify_result(ssl);
 	//TODO: X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY//

	char msg[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n";	 
    len = SSL_write(ssl, msg, sizeof(msg));
	if(len<0){
		printf("ssl write error\n");
		return 1;
	}
	while(1){
		len = SSL_read(ssl, buffer, 1024);
		if(len<0){
			printf("ssl read error\n");
			return 1;
		}else if(len==0){
			printf("ssl read finish\n");
			break;
		}else{
			fwrite(buffer,1,len,stdout);
		}

	}
	    
    SSL_shutdown(ssl);
    SSL_free(ssl);
    //close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}



static size_t WriteFunction(void *input, size_t uSize, size_t uCount, void *avg)
{
	fwrite(input,1,uCount,stdout);
	return uCount;
}
int test_http(){	
	CURL* pCurl;
	char errbuf[256];
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	printf("------------ test curl -----------------\n");
	pCurl = curl_easy_init();
	if (pCurl == NULL) {
		printf("curl_easy_init() error\n");
		return -1;
	}
	

	curl_easy_setopt(pCurl, CURLOPT_URL, "https://www.baidu.com"); 	
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST,0);
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);
	curl_easy_setopt(pCurl, CURLOPT_ERRORBUFFER, errbuf);
	CURLcode ret = curl_easy_perform(pCurl);
	if(ret){
		printf("%s\n",errbuf);
	}else{
		printf("http ok\n");
	}
	return 0;
}


int main(){
	uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
	printf("123456\n");
	printf("crc16=%04x\n", (int)crc);	
	
	uint32_t crc32 = crc32_z(0, (uint8_t*)"123456", 6);
	printf("crc32=%08x\n", crc32);
	/*auto json = */json_parse("{\"id\":123}", 3);
	//printf("%d\n", (int)(*json)["id"]);

	auto duk = duk_create_heap_default();
	duk_eval_string(duk, "1+2");
	printf("%d\n", duk_to_int(duk, -1));

	test_http();
	test_ssl();
	return 0;
	
}




