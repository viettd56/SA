#include "request_parser.hpp"
#include "http_parser.h"
#include "util.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

using std::cout;

string last_header_field = "";
string last_header_value = "";
enum state {FIELD, VALUE};
state last_state;
http_parser_settings settings;
bool is_complete;

map<string, string> headers_map_parsed;
map<string, string> params_map_parsed;
string url = "";
char *data = NULL;
int data_len = 0;
int data_size = 0;

// http parser callbacks
int request_url_cb(http_parser *p, const char *buf, size_t len);
int header_field_cb(http_parser *p, const char *buf, size_t len);
int header_value_cb(http_parser *p, const char *buf, size_t len);
int body_cb(http_parser *p, const char *buf, size_t len);
int headers_complete_cb(http_parser *p);
int message_complete_cb(http_parser *p);

int request_url_cb(http_parser *p, const char *buf, size_t len)
{
    url = str_nconcat_char(url, buf, len);
    return 0;
}


int header_field_cb(http_parser *p, const char *buf, size_t len)
{
    if (last_state == FIELD)
    {
        last_header_field = str_nconcat_char(last_header_field, buf, len);
    }
    else
    {
        last_header_field.clear();
        last_header_value.clear();
        last_header_field = str_nconcat_char(last_header_field, buf, len);
    }
    last_state = FIELD;
    return 0;
}

int header_value_cb(http_parser *p, const char *buf, size_t len)
{
    last_header_value = str_nconcat_char(last_header_value, buf, len);
    if (headers_map_parsed.count(last_header_field))
    {
        headers_map_parsed.erase(last_header_field);
    }
    headers_map_parsed.insert (std::pair<string, string>(last_header_field, last_header_value));
    last_state = VALUE;
    return 0;
}

int body_cb(http_parser *p, const char *buf, size_t len)
{
    if (data_len < data_size){
        memcpy(data+data_len, buf, len);
        data_len += len;
    }
    return 0;
}

int headers_complete_cb(http_parser *p)
{
    data_size = atoi(headers_map_parsed["Content-Length"].c_str());
    data = new char[data_size]();
    return 0;
}


int message_complete_cb(http_parser *p)
{
    is_complete = true;
    return 0;
}

void request_parser_init()
{

    settings.on_header_field = header_field_cb;
    settings.on_header_value = header_value_cb;
    settings.on_url = request_url_cb;
    settings.on_body = body_cb;
    settings.on_headers_complete = headers_complete_cb;
    settings.on_message_complete = message_complete_cb;

    last_state = FIELD;
    last_header_field = "";
    last_header_value = "";
    url = "";
    is_complete = false;
    data_len = 0;
    data_size = 0;

    headers_map_parsed.clear();
    params_map_parsed.clear();
}
int request_parser_excute(http_parser *parser, Request &rq, char *buf, char n)
{
    int nparsed = http_parser_execute(parser, &settings, buf, n);
    if (nparsed != n)
    {
        error("http parser error");
    }

    if (is_complete)
    {
        rq.set_headers_map(headers_map_parsed);
        rq.set_params_map(params_map_parsed);
        rq.set_url(url);
        rq.set_method(convert_method(parser->method));
        rq.set_data(data, data_len);
        if (data != NULL){
            delete[] data;
        }
        return 0;
    }

    return 1;
}
map<string, string> get_headers_map()
{
    return headers_map_parsed;
}
map<string, string> get_params_map()
{
    return params_map_parsed;
}

string get_url(){
    return url;
}