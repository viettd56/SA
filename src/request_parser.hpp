#ifndef _request_parser_hpp_
#define _request_parser_hpp_

#include <map>
#include <string>
#include "http_parser.h"
#include "request.hpp"

using std::string;
using std::map;

void request_parser_init();
int request_parser_excute(http_parser* parser, Request &rq, char* buf, char n); // return 0 if complete
string get_url();

#endif