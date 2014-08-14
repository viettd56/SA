#include "request_parser.hpp"
#include "http_parser.h"
#include "util.hpp"
#include "routing.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

using std::cout;

string  last_header_field       =   "";
string  last_header_value       =   "";
string  url                     =   "";
int     sock                    =   0;
char    *data                   =   NULL;
int     data_len                =   0;
int     data_size               =   0;

enum state {FIELD, VALUE};
state last_state;
http_parser_settings settings;

map<string, string> headers_map_parsed;
map<string, string> params_map_parsed;
// char *data2 = new char[128]();
// int data_len2 = 0;
// int data_size2 = 0;

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
    // cout << "------------------------url:" << url << "\n";
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
    //cout << "F:" << last_header_field << "\n" << "V:" << last_header_value << "\n";
    last_state = VALUE;
    return 0;
}

int body_cb(http_parser *p, const char *buf, size_t len)
{

    // cout << "---------Body----------\n" << buf << "\n";

    if (data_len + len <= data_size)
    {
        memcpy(data + data_len, buf, len);
        data_len += len;
    }
    // }
    // safe_copy(data, buf, len, data_size, data_len);
    // cout << "size:" << data_size << "\n" << "len:" << data_len << "\n" << len << "\n";
    // cout << "size2:" << data_size2 << "\n" << "len2:" << data_len2 << "\n" << len << "\n";

    //cout << "data:" << data << "\n";
    return 0;
}

int headers_complete_cb(http_parser *p)
{
    // cout << "on_headers_complete" << "\n";
    data_size   =   atoi(headers_map_parsed["Content-Length"].c_str());
    //cout << "--------------------" << data_size << "------------------------\n";
    data        =   new char[data_size]();
    //cout << "-------------------" << (int *)data << "\n";
    return 0;
}


int message_complete_cb(http_parser *p)
{
    // cout << "message_complete_cb" << "\n";

    Request rq;
    rq.set_headers_map(headers_map_parsed);
    rq.set_params_map(params_map_parsed);
    rq.set_url(url);
    rq.set_method(convert_method(p->method));
    // cout << "-----method:" << rq.get_method() << "\n";
    rq.set_data(data, data_len);
    //cout << "URL: " << url << "\nBody: " <<  data << "\n";
    rq.set_len_data(data_len);
    if (data != NULL)
    {
        delete[] data;
        data = NULL;
    }

    routing_excute(sock, rq);
    request_parser_init(sock);

    return 0;
}

void request_parser_init(const int &sk)
{

    settings.on_header_field        =   header_field_cb;
    settings.on_header_value        =   header_value_cb;
    settings.on_url                 =   request_url_cb;
    settings.on_body                =   body_cb;
    settings.on_headers_complete    =   headers_complete_cb;
    settings.on_message_complete    =   message_complete_cb;

    last_state                      =   FIELD;
    last_header_field               =   "";
    last_header_value               =   "";
    url                             =   "";
    sock                            =   sk;
    data_len                        =   0;
    data_size                       =   0;

    if (data != NULL)
    {
        delete[] data;
        data = NULL;
    }

    headers_map_parsed.clear();
    params_map_parsed.clear();
}
void request_parser_excute(http_parser *parser, char *buf, int n)
{
    int nparsed = http_parser_execute(parser, &settings, buf, n);
    // cout << "HTTP: " << buf << "\n";
    cout << "nparsed: " << nparsed << "\nn:" << n << "\n";
    if (nparsed != n)
    {
        error("http parser error");
    }

}
map<string, string> get_headers_map()
{
    return headers_map_parsed;
}
map<string, string> get_params_map()
{
    return params_map_parsed;
}

string get_url()
{
    return url;
}
