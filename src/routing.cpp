#include <iostream>
#include "routing.hpp"
#include "http_request.h"
#include "http_response.h"
#include "callback.hpp"
#include "util.hpp"
#include <cstring>

using std::cout;

void send_error_method_not_allowed(http_response_t *res);
void send_error_not_found(http_response_t *res);

void routing_excute(const int &sock, const Request &rq)
{


	http_response_t 	*res 	= 	NULL;
	// headers_map = get_headers_map();
	// params_map = get_params_map();
	string 				method 	= 	rq.get_method();
	string 				url 	= 	rq.get_url();
	//cout << "----------method: " << method << "\n" << url << "\n";
	// char *data = rq.get_data();

	res 						= 	http_response_init("HTTP/1.1", 200, "OK");

	if (!method.compare("GET"))
	{
		if (!url.compare("/slideshow-features"))
		{
			//A client can fetch the list of available transitions for slideshows.
			cout << "GET /slideshow-features\n";
			get_slideshow_features(res);
		}
		else if (!url.compare("/server-info"))
		{
			// Fetch general informations about the AirPlay server.
			cout << "GET  /server-info\n";
			get_server_info(res);

		}
		else if (!url.compare("/scrub"))
		{
			//Retrieve the current playback position.
			cout << "GET /scrub\n";
			get_scrub(res);
		}
		else if (!url.compare("/playback-info"))
		{
			//Retrieve playback informations such as position, duration, rate, buffering status and more.
			cout << "GET /playback-info\n";
			get_playback_info(res);
		}
		else if (str_starts_with(url.c_str(), "/getProperty?"))
		{
			//Get playback property.
			cout << "GET /getProperty\n";
			get_getProperty(url.c_str() + strlen("/getProperty?"), res);
		}
		else if (!url.compare("/stream.xml"))
		{
			//fetch mirroring server informations
			cout << "GET /stream.xml\n";
			get_stream(res);
		}
		else
		{
			send_error_not_found(res);
		}
	}
	else if (!method.compare("POST"))
	{
		if (!url.compare("/stop"))
		{
			//Stop a photo or slideshow session.
			cout << "POST /stop\n";
			post_stop(res);
		}
		else if (!url.compare("/reverse"))
		{
			cout << "POST /reverse\n";
			post_reverse(rq, res);
		}
		else if (!url.compare("/play"))
		{
			//Start video playback.
			cout << "POST /play\n";
			post_play(rq, res);
		}
		else if (str_starts_with(url.c_str(), "/scrub?"))
		{
			//Seek at an arbitrary location in the video.
			cout << "POST /scrub\n";
			post_scrub(url.c_str() + strlen("/scrub?"), res);
		}
		else if (str_starts_with(url.c_str(), "/rate?"))
		{
			//Change the playback rate.
			cout << "POST /rate\n";
			post_rate(url.c_str() + strlen("/rate?"), res);
		}
		else if (!url.compare("/stream"))
		{
			//Start the live video transmission.
			cout << "POST /stream\n";
			post_stream(rq, res, sock);
			cout << "--------" << res << "\n";
		}
		else
		{
			send_error_not_found(res);
		}
	}
	else if (!method.compare("PUT"))
	{
		if (!url.compare("/photo"))
		{
			//Send a JPEG picture to the server.
			cout << "PUT /photo\n";
			put_photo(rq, res);
		}
		else if (!url.compare("/slideshows/1"))
		{

			//Start or stop a slideshow session.
			cout << "PUT /slideshows/1\n";
			put_slideshow_session(rq, res);
		}
		else if (str_starts_with(url.c_str(), "/setProperty?"))
		{
			//Set playback property.
			cout << "PUT /setProperty\n";
			put_setProperty(url.c_str() + strlen("/setProperty?"), rq, res);
		}
		else
		{
			send_error_not_found(res);
		}
	}
	else
	{
		send_error_method_not_allowed(res);
	}

	if (res != NULL)
	{
		int datalen;
		// http_response_finish(res, NULL, 0);
		const char *data_res = http_response_get_data(res, &datalen);

		send_to_socket(sock, data_res, datalen);
		http_response_destroy(res);
	}
	//------------------------------------------------

	// http_request_t *req = post_event_video();
	// int datalen;
	// const char *data_res = http_request_get_data(req, &datalen);

	// send_to_socket(sock, data_res, datalen);
	// http_request_destroy(req);
}


void send_error_method_not_allowed(http_response_t *res)
{
	http_response_destroy(res);
	res = http_response_init("HTTP/1.1", 405, "Method Not Allowed");
	http_response_finish(res, NULL, 0);
}

void send_error_not_found(http_response_t *res)
{
	http_response_destroy(res);
	res = http_response_init("HTTP/1.1", 404, "Not Found");
	http_response_finish(res, NULL, 0);
}
