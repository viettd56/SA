#ifndef _request_hpp_
#define _request_hpp_

#include <map>
#include <string>

using std::map;
using std::string;

class Request
{
public:
	Request();
	~Request();
    map<string, string> get_headers_map();
    void set_headers_map(const map<string, string> &rhs);
    map<string, string> get_params_map();
    void set_params_map(const map<string, string> &rhs);
    string get_url();
    void set_url(const string &rhs);
    char *get_data();
    void set_data(const char *rhs, const int lenght);
    string get_method();
    void set_method(const string &rhs);
private:
    map<string, string> map_headers;
    map<string, string> map_params;
    string url;
    string method;
    char *data;
    int len_data;
};

#endif