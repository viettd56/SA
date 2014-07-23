#include <iostream>
#include <unistd.h>
#include "http_request.hpp"
#include "util.hpp"
#include "request_parser.hpp"

const int BUFFER_SIZE = 80;
char buffer[BUFFER_SIZE];

using std::cout;

Http_request::Http_request(int sk): sock(sk)
{

}

Http_request::~Http_request()
{

}

void Http_request::http_request_excute()
{
    http_parser parser;
    http_parser_init(&parser, HTTP_REQUEST);
    Request rq;

    request_parser_init();

    int n, nparsed;

    while (n = read(sock, buffer, BUFFER_SIZE))
    {

        if (n < 0) error("ERROR reading from socket");

        if (request_parser_excute(&parser, rq, buffer, n) == 0)
        {
            break;
        }
    }


    // headers_map = get_headers_map();
    // params_map = get_params_map();
    string method = rq.get_method();
    string url = rq.get_url();
    // cout << method << "\n" << url << "\n";

    if (!method.compare("GET"))
    {
        if (!url.compare("/slideshow-features"))
        {
            cout << "GET /slideshow-features\n";
        }
        else if (!url.compare("/server-info"))
        {
            cout << "GET  /server-info\n";
        }
        else if (!url.compare("/scrub"))
        {
            cout << "GET /scrub\n";
        }
        else if (!url.compare("/playback-info"))
        {
            cout << "GET /playback-info\n";
        }
        else if (!url.compare("/getProperty"))
        {
            cout << "GET /getProperty\n";
        }
    }
    else if (!method.compare("POST"))
    {
        if (!url.compare("/stop"))
        {
            cout << "POST /stop\n";
        }
        else if (!url.compare("/event"))
        {
            cout << "POST /event\n";
        }
        else if (!url.compare("/play"))
        {
            cout << "POST /play\n";
        }
        else if (!url.compare("/scrub"))
        {
            cout << "POST /scrub\n";
        }
        else if (!url.compare("/rate"))
        {
            cout << "POST /rate\n";
        }
    }
    else if (!method.compare("PUT"))
    {
        if (!url.compare("/photo"))
        {
            cout << "PUT /photo\n";
        }
        else if (!url.compare("/slideshows/1"))
        {
            cout << "PUT /slideshows/1\n";
        }
        else if (!url.compare("/setProperty"))
        {
            cout << "PUT /setProperty\n";
        }
    }
}
