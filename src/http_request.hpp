#ifndef _http_request_hpp_
#define _http_request_hpp_

#include <map>
#include <string>
#include <iostream>

using std::string;
using std::cout;

class Http_request
{
public:
	//sk: socket from client
    Http_request(int sk);
    ~Http_request();
    // Receive request from client and excute
    void http_request_excute();
  private:
    int sock;
    std::map<string, string> headers_map;
    std::map<string, string> params_map;
};

#endif
