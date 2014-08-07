#include <iostream>
#include <unistd.h>
#include "http_request_parse.hpp"
#include "util.hpp"
#include "request_parser.hpp"
#include "http_response.h"
#include "callback.hpp"

const int BUFFER_SIZE = 80;
char buffer[BUFFER_SIZE];

using std::cout;

Http_request_parse::Http_request_parse(int sk): sock(sk)
{

}

Http_request_parse::~Http_request_parse()
{

}

void Http_request_parse::http_request_parse_excute()
{
    while (1)
    {
        http_parser parser;
        http_parser_init(&parser, HTTP_REQUEST);
        Request rq;

        request_parser_init();
        int n, nparsed;
        while (n = read(sock, buffer, BUFFER_SIZE))
        {
            if (n < 0) error("ERROR reading from socket");
            //std::cout << "-----------data: " << buffer << "\n";
            if (request_parser_excute(&parser, rq, buffer, n) == 0)
            {
                break;
            }
        }
        
        callback_excute(sock, rq);
        
    }
}

