#ifndef _callback_hpp_
#define _callback_hpp_

#include <string>
#include "request.hpp"
#include "http_response.h"

//List callback
void get_slideshow_features(http_response_t *res);
void put_photo(char *data, http_response_t *res);
void put_slideshow_session(char *data, http_response_t *res);
void fetch_photo(http_response_t *res);
void post_stop_photo_slideshow(char *data, http_response_t *res);
void post_event(char *data, http_response_t *res);
void get_slideshow(char *data, http_response_t *res);
void get_server_info(http_response_t *res);
void post_play(char *data, http_response_t *res);
void post_scrub(http_response_t *res);
void post_rate(http_response_t *res);
void post_stop(http_response_t *res);
void get_scrub(http_response_t *res);
void get_playback_info(http_response_t *res);
void put_setProperty(char *data, http_response_t *res);
void post_getProperty(http_response_t *res);
void notify_event(char *data, http_response_t *res);
void notify_slideshow(http_response_t *res);
void stop_photo_session(char *data, http_response_t *res);
void post_reverse(http_response_t *res);


//For each request rq, excute callback
//sock: socket client
void callback_excute(const int &sock, const Request &rq);

#endif