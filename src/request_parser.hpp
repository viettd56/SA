#ifndef _request_parser_hpp_
#define _request_parser_hpp_

#include <map>
#include <string>
#include "http_parser.h"
#include "request.hpp"

using std::string;
using std::map;

void request_parser_init(const int &sk);
void request_parser_excute(http_parser* parser, char* buf, int n);
string get_url();

#endif