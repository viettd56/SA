#ifndef _http_request_hpp_
#define _http_request_hpp_

#include <map>
#include <string>
#include <iostream>

using std::string;
using std::cout;

class Http_request_parse
{
public:
	//sk: socket from client
    Http_request_parse(int sk);
    ~Http_request_parse();
    // Receive request from client and excute
    void http_request_parse_excute();
  private:
    int sock;
    std::map<string, string> headers_map;
    std::map<string, string> params_map;
};

#endif
