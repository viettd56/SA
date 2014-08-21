#include <iostream>
#include <unistd.h>
#include "http_request_parse.hpp"
#include "util.hpp"
#include "request_parser.hpp"
#include "http_response.h"
#include "callback.hpp"
#include "routing.hpp"

const int BUFFER_SIZE = 128;
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
    http_parser parser;
    http_parser_init(&parser, HTTP_BOTH);
    request_parser_init(sock);

    while (1)
    {
        int n;
        n = read(sock, buffer, BUFFER_SIZE);
        if (n < 0) error("ERROR reading from socket");
        print_debug("n = %d\n", n);
        if (n == 0) break;
        // nprintln(buffer, n);
        request_parser_excute(&parser, buffer, n);
    }
}

