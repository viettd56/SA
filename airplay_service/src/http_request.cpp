
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "http_request.h"
#include "compat.h"

struct http_request_s {
	int complete;
	int disconnect;

	char *data;
	int data_size;
	int data_length;
};


static void
http_request_add_data(http_request_t *request, const char *data, int datalen)
{
	int newdatasize;

	assert(request);
	assert(data);
	assert(datalen > 0);

	newdatasize = request->data_size;
	while (request->data_size+datalen > newdatasize) {
		newdatasize *= 2;
	}
	if (newdatasize != request->data_size) {
		request->data = (char*)realloc(request->data, newdatasize);
		assert(request->data);
	}
	memcpy(request->data+request->data_length, data, datalen);
	request->data_length += datalen;
}

http_request_t *
http_request_init(const char *protocol, const char *method, const char *url)
{
	http_request_t *request;

	request = (http_request_t*)calloc(1, sizeof(http_request_t));
	if (!request) {
		return NULL;
	}

	/* Allocate request data */
	request->data_size = 1024;
	request->data = (char*)malloc(request->data_size);
	if (!request->data) {
		free(request);
		return NULL;
	}

	/* Add first line of request to the data array */
	http_request_add_data(request, method, strlen(method));
	http_request_add_data(request, " ", 1);
	http_request_add_data(request, url, strlen(url));
	http_request_add_data(request, " ", 1);
	http_request_add_data(request, protocol, strlen(protocol));
	http_request_add_data(request, "\r\n", 2);

	return request;
}

void
http_request_destroy(http_request_t *request)
{
	if (request) {
		free(request->data);
		free(request);
	}
}

void
http_request_add_header(http_request_t *request, const char *name, const char *value)
{
	assert(request);
	assert(name);
	assert(value);

	http_request_add_data(request, name, strlen(name));
	http_request_add_data(request, ": ", 2);
	http_request_add_data(request, value, strlen(value));
	http_request_add_data(request, "\r\n", 2);
}

void
http_request_finish(http_request_t *request, const char *data, int datalen)
{
	assert(request);
	assert(datalen==0 || (data && datalen > 0));

	if (data && datalen > 0) {
		const char *hdrname = "Content-Length";
		char hdrvalue[16];

		memset(hdrvalue, 0, sizeof(hdrvalue));
		snprintf(hdrvalue, sizeof(hdrvalue)-1, "%d", datalen);

		/* Add Content-Length header first */
		http_request_add_data(request, hdrname, strlen(hdrname));
		http_request_add_data(request, ": ", 2);
		http_request_add_data(request, hdrvalue, strlen(hdrvalue));
		http_request_add_data(request, "\r\n\r\n", 4);

		/* Add data to the end of request */
		http_request_add_data(request, data, datalen);
	} else {
		/* Add extra end of line after headers */
		http_request_add_data(request, "\r\n", 2);
	}
	request->complete = 1;
}

void
http_request_set_disconnect(http_request_t *request, int disconnect)
{
	assert(request);

	request->disconnect = !!disconnect;
}

int
http_request_get_disconnect(http_request_t *request)
{
	assert(request);

	return request->disconnect;
}

const char *
http_request_get_data(http_request_t *request, int *datalen)
{
	assert(request);
	assert(datalen);
	assert(request->complete);

	*datalen = request->data_length;
	return request->data;
}
