#ifndef USING_HTTPREQ
#define USING_HTTPREQ

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "http.h"

struct httpReq {
	char* data;
	int data_size;
};

struct httpReq httpRequest(char* web) {
	#ifdef WIN32
	{
		WSADATA wsaData = {0};
		int result;

		if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData))) {
			printf("WSAStartup failed: %d\n", result);
			struct httpReq hR;
			return hR;
		}
	}
#endif
	int sd;
	struct http_message msg;

	if ((sd = http_request(web)) < 1) {
		perror("http_request");
	}

	memset(&msg, 0, sizeof(msg));
	uint8_t* result;
	int total = 0;
	char isMalloc = 0;
	while (http_response(sd, &msg) > 0) {
		if (msg.content) {
			if (isMalloc == 0) {
				result = malloc(msg.header.length);
				isMalloc = 1;
			}
			memcpy(result + total, msg.content, msg.length);
			total += msg.length;
		}
	}
	total = 0;
	isMalloc = 0;
	close(sd);
#ifdef WIN32
	WSACleanup();
#endif
	struct httpReq hR;
	hR.data = result;
	hR.data_size = msg.header.length;
	return hR;
}

#endif