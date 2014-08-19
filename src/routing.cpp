#include <iostream>
#include "routing.hpp"
#include "http_request.h"
#include "http_response.h"
#include "callback.hpp"
#include "util.hpp"
#include <cstring>

using std::cout;

void send_error_method_not_allowed(const int &sock);
void send_error_not_found(const int &sock);

void routing_excute(const int &sock, const Request &rq)
{

	const string method 	= rq.get_method();
	const string url		= rq.get_url();

    // cout << method << "\n" << url << "\n";

    if (method == "Undefined") return; // response

    if (!method.compare("GET"))
    {
        if (!url.compare("/slideshow-features"))
        {
            //A client can fetch the list of available transitions for slideshows.
            cout << "GET /slideshow-features\n";
            get_slideshow_features(sock);
        }
        else if (!url.compare("/server-info"))
        {
            // Fetch general informations about the AirPlay server.
            cout << "GET  /server-info\n";
            get_server_info(sock);

        }
        else if (!url.compare("/scrub"))
        {
            //Retrieve the current playback position.
            cout << "GET /scrub\n";
            get_scrub(sock);
        }
        else if (!url.compare("/playback-info"))
        {
            //Retrieve playback informations such as position, duration, rate, buffering status and more.
            cout << "GET /playback-info\n";
            get_playback_info(sock);
        }
        else if (str_starts_with(url.c_str(), "/getProperty?"))
        {
            //Get playback property.
            cout << "GET /getProperty\n";
            get_getProperty(sock, url.c_str() + strlen("/getProperty?"));
        }
        else if (!url.compare("/stream.xml"))
        {
            //fetch mirroring server informations
            cout << "GET /stream.xml\n";
            get_stream(sock);
        }
        else
        {
            send_error_not_found(sock);
        }
    }
    else if (!method.compare("POST"))
    {
        if (!url.compare("/stop"))
        {
            //Stop a photo or slideshow session.
            cout << "POST /stop\n";
            post_stop(sock);
        }
        else if (!url.compare("/reverse"))
        {
            cout << "POST /reverse\n";
            post_reverse(sock, rq);
        }
        else if (!url.compare("/play"))
        {
            //Start video playback.
            cout << "POST /play\n";
            post_play(sock, rq);
        }
        else if (str_starts_with(url.c_str(), "/scrub?"))
        {
            //Seek at an arbitrary location in the video.
            cout << "POST /scrub\n";
            post_scrub(sock, url.c_str() + strlen("/scrub?"));
        }
        else if (str_starts_with(url.c_str(), "/rate?"))
        {
            //Change the playback rate.
            cout << "POST /rate\n";
            post_rate(sock, url.c_str() + strlen("/rate?"));
        }
        else if (!url.compare("/stream"))
        {
            //Start the live video transmission.
            cout << "POST /stream\n";
            post_stream(sock, rq);
        }
        else
        {
            send_error_not_found(sock);
        }
    }
    else if (!method.compare("PUT"))
    {
        if (!url.compare("/photo"))
        {
            //Send a JPEG picture to the server.
            cout << "PUT /photo\n";
            put_photo(sock, rq);
        }
        else if (!url.compare("/slideshows/1"))
        {

            //Start or stop a slideshow session.
            cout << "PUT /slideshows/1\n";
            put_slideshow_session(sock, rq);
        }
        else if (str_starts_with(url.c_str(), "/setProperty?"))
        {
            //Set playback property.
            cout << "PUT /setProperty\n";
            put_setProperty(sock, url.c_str() + strlen("/setProperty?"), rq);
        }
        else
        {
            send_error_not_found(sock);
        }
    }
    else
    {
        send_error_method_not_allowed(sock);
    }
}


void send_error_method_not_allowed(const int &sock)
{
    http_response_t *res = http_response_init("HTTP/1.1", 405, "Method Not Allowed");
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

void send_error_not_found(const int &sock)
{
    http_response_t *res = http_response_init("HTTP/1.1", 404, "Not Found");
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}
