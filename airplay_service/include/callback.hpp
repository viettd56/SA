#ifndef _callback_hpp_
#define _callback_hpp_

#include <string>
#include "http_response.h"
#include "http_request.h"
#include "request.hpp"

//List callback
// A client can fetch the list of available transitions for slideshows.
void get_slideshow_features(const int &sock);
//Send a JPEG picture to the server.
void put_photo(const int &sock, const Request &rq);
// Start or stop a slideshow session.
void put_slideshow_session(const int &sock, const Request &rq);
//notifies a client that a photo session has ended.
void post_event_photo(const int &sock);
// fetch a new picture
void get_slideshow(const int &sock);
// Fetch general informations about the AirPlay server.
void get_server_info(const int &sock);
//Start video playback.
void post_play(const int &sock, const Request &rq);
//Seek at an arbitrary location in the video.
void post_scrub(const int &sock, const char *argument);
//Change the playback rate.
void post_rate(const int &sock, const char *argument);
//Stop a photo or slideshow session.
void post_stop(const int &sock);
//Retrieve the current playback position.
void get_scrub(const int &sock);
//Retrieve playback informations such as position, duration, rate, buffering status and more.
void get_playback_info(const int &sock);
//Set playback property.
void put_setProperty(const int &sock, const char *argument, const Request &rq);
//Get playback property.
void get_getProperty(const int &sock, const char *argument);
// notify the server about the playback state.
void post_event_slideshow(const int &sock);
// send the playback state to the client.
void post_event_video(const int &sock);

void post_reverse(const int &sock, const Request &rq);

//Retrieve information about the server capabilities.
void get_stream(const int &sock);

//The client sends a binary property list with information about the stream
void post_stream(const int &sock, const Request &rq);

//server send command to remote control
//list command: http://nto.github.io/AirPlay.html#audio-remotecontrol
void send_command_to_remote_control(const string &command);

#endif