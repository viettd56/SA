#ifndef _callback_hpp_
#define _callback_hpp_

#include <string>
#include "request.hpp"
#include "http_response.h"
#include "http_request.h"

//List callback
void get_slideshow_features(http_response_t *res);
void put_photo(const Request &rq, http_response_t *res);
void put_slideshow_session(const Request &rq, http_response_t *res);
void fetch_photo(http_response_t *res);
void post_stop_photo_slideshow(const Request &rq, http_response_t *res);
http_request_t* post_event_photo();
void get_slideshow(const Request &rq, http_response_t *res);
void get_server_info(http_response_t *res);
void post_play(const Request &rq, http_response_t *res);
void post_scrub(const char *argument, http_response_t *res);
void post_rate(const char *argument, http_response_t *res);
void post_stop(http_response_t *res);
void get_scrub(http_response_t *res);
void get_playback_info(http_response_t *res);
void put_setProperty(const char *argument, const Request &rq, http_response_t *res);
void get_getProperty(const char *argument, http_response_t *res);
void notify_event(const Request &rq, http_response_t *res);
http_request_t* post_event_slideshow();
void stop_photo_session(const Request &rq, http_response_t *res);
void post_reverse(http_response_t *res);


//For each request rq, excute callback
//sock: socket client
void callback_excute(const int &sock, const Request &rq);

#endif