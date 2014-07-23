#include "request.hpp"
#include <cstring>

Request::Request()
{
    map_headers.clear();
    map_params.clear();
    url = "";
    data = NULL;
    len_data = 0;
}

Request::~Request()
{
    if (data != NULL)
    {
        delete[] data;
    }
}

map<string, string> Request::get_headers_map() const
{
    return map_headers;
}


map<string, string> Request::get_params_map() const
{
    return map_params;
}

void Request::set_headers_map(const map<string, string> &rhs)
{
	map_headers = rhs;
}
void Request::set_params_map(const map<string, string> &rhs)
{
	map_params = rhs;
}
string Request::get_url() const
{
	return url;
}
void Request::set_url(const string &rhs)
{
	url = rhs;
}
char* Request::get_data() const
{
	return data;
}
void Request::set_data(const char *rhs, const int lenght)
{
	len_data = lenght;
	data = new char[len_data]();
	memcpy(data, rhs, len_data);
}

string Request::get_method() const
{
	return method;
}

void Request::set_method(const string &rhs){
	method = rhs;
}