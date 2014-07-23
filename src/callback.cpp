#include "callback.hpp"
#include <iostream>
#include "util.hpp"

using std::cout;

void callback_excute(const int &sock, const Request &rq)
{
    http_response_t *res = NULL;
    // headers_map = get_headers_map();
    // params_map = get_params_map();
    string method = rq.get_method();
    string url = rq.get_url();
    // cout << method << "\n" << url << "\n";
    char *data = rq.get_data();

    res = http_response_init("HTTP/1.1", 200, "OK");

    if (!method.compare("GET"))
    {
        if (!url.compare("/slideshow-features"))
        {
            cout << "GET /slideshow-features\n";
            get_slideshow_features(res);
        }
        else if (!url.compare("/server-info"))
        {
            cout << "GET  /server-info\n";
            get_server_info(res);

        }
        else if (!url.compare("/scrub"))
        {
            cout << "GET /scrub\n";
            get_scrub(res);
        }
        else if (!url.compare("/playback-info"))
        {
            cout << "GET /playback-info\n";
            get_playback_info(res);
        }
        
    }
    else if (!method.compare("POST"))
    {
        if (!url.compare("/stop"))
        {
            cout << "POST /stop\n";
            post_stop(res);
        }
        else if (!url.compare("/reverse"))
        {
            cout << "POST /reverse\n";
            post_reverse(res);
        }
        else if (!url.compare("/play"))
        {
            cout << "POST /play\n";
            post_play(data, res);
        }
        else if (str_starts_with(url.c_str(), "/scrub"))
        {
            cout << "POST /scrub\n";
            post_scrub(res);
        }
        else if (str_starts_with(url.c_str(), "/rate"))
        {
            cout << "POST /rate\n";
            post_rate(res);
        }
        else if (str_starts_with(url.c_str(), "/getProperty"))
        {
            cout << "POST /getProperty\n";
            post_getProperty(res);
        }
    }
    else if (!method.compare("PUT"))
    {
        if (!url.compare("/photo"))
        {
            cout << "PUT /photo\n";
            put_photo(data, res);
        }
        else if (!url.compare("/slideshows/1"))
        {
            cout << "PUT /slideshows/1\n";
            put_slideshow_session(data, res);
        }
        else if (str_starts_with(url.c_str(), "/setProperty"))
        {
            cout << "PUT /setProperty\n";
            put_setProperty(data, res);
        }
    }

    http_response_finish(res, NULL, 0);
    //cout << "res:" << res << "\n";
    int datalen;
    const char *data_res = http_response_get_data(res, &datalen);

    send_to_socket(sock, data_res, datalen);
    http_response_destroy(res);
}

void get_slideshow_features(http_response_t *res)
{

}
void put_photo(char *data, http_response_t *res)
{

}
void put_slideshow_session(char *data, http_response_t *res)
{

}
void fetch_photo(http_response_t *res)
{

}
void post_stop_photo_slideshow(char *data, http_response_t *res)
{

}
void post_event(char *data, http_response_t *res)
{

}
void get_slideshow(char *data, http_response_t *res)
{

}
void get_server_info(http_response_t *res)
{

}
void post_play(char *data, http_response_t *res)
{

}
void post_scrub(http_response_t *res)
{

}
void post_rate(http_response_t *res)
{

}
void post_stop(http_response_t *res)
{

}
void get_scrub(http_response_t *res)
{

}
void get_playback_info(http_response_t *res)
{

}
void put_setProperty(char *data, http_response_t *res)
{

}
void post_getProperty(http_response_t *res)
{

}
void notify_event(char *data, http_response_t *res)
{

}
void notify_slideshow(http_response_t *res)
{

}
void stop_photo_session(char *data, http_response_t *res)
{

}

void post_reverse(http_response_t *res)
{

}