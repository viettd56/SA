#ifndef _callback_hpp_
#define _callback_hpp_

#include <string>
#include "request.hpp"
#include "http_response.h"
#include "http_request.h"

//List callback
// A client can fetch the list of available transitions for slideshows.
void get_slideshow_features(http_response_t *res);
//Send a JPEG picture to the server.
void put_photo(const Request &rq, http_response_t *res);
// Start or stop a slideshow session.
void put_slideshow_session(const Request &rq, http_response_t *res);
//notifies a client that a photo session has ended.
http_request_t *post_event_photo();
// fetch a new picture
http_request_t *get_slideshow();
// Fetch general informations about the AirPlay server.
void get_server_info(http_response_t *res);
//Start video playback.
void post_play(const Request &rq, http_response_t *res);
//Seek at an arbitrary location in the video.
void post_scrub(const char *argument, http_response_t *res);
//Change the playback rate.
void post_rate(const char *argument, http_response_t *res);
//Stop a photo or slideshow session.
void post_stop(http_response_t *res);
//Retrieve the current playback position.
void get_scrub(http_response_t *res);
//Retrieve playback informations such as position, duration, rate, buffering status and more.
void get_playback_info(http_response_t *res);
//Set playback property.
void put_setProperty(const char *argument, const Request &rq, http_response_t *res);
//Get playback property.
void get_getProperty(const char *argument, http_response_t *res);
// notify the server about the playback state.
http_request_t *post_event_slideshow();
// send the playback state to the client.
http_request_t *post_event_video();

void post_reverse(const Request &rq, http_response_t *res);

//Retrieve information about the server capabilities.
void get_stream(http_response_t *res);

//The client sends a binary property list with information about the stream
void post_stream(const Request &rq, http_response_t *res, const int &sock);

//For each request rq, excute callback
//sock: socket client
void callback_excute(const int &sock, const Request &rq);

#endif