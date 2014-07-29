#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

typedef struct http_request_s http_request_t;

http_request_t *http_request_init(const char *protocol, const char *method, const char *url);

void http_request_add_header(http_request_t *request, const char *name, const char *value);
void http_request_finish(http_request_t *request, const char *data, int datalen);

void http_request_set_disconnect(http_request_t *request, int disconnect);
int http_request_get_disconnect(http_request_t *request);

const char *http_request_get_data(http_request_t *request, int *datalen);

void http_request_destroy(http_request_t *request);

#endif
