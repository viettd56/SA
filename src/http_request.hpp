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
    Http_request(int sk);
    ~Http_request();
    //char *http_request_uploads_path(http_request *request);
    void http_request_excute();
  private:
    //url *url;
    int sock;
    std::map<string, string> headers_map;
    std::map<string, string> params_map;
};

#endif
