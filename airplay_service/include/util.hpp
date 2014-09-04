#ifndef _util_hpp_
#define _util_hpp_
#include <string>
#include <map>
#include <cstdarg>
#include "http_request.h"
#include "http_response.h"

using std::string;

void 	error(const char* msg);
string 	str_nconcat_char(const string &des, const char *src, const int &n);
void 	attrs_quotes_map_str_parse(std::map<string, string> &map,const char *str);
void 	attrs_brackets_map_str_parse(std::map<string, string> &map,const char *str);
void 	msgs_map_str_parse(std::map<string, string> &map,const char *str);
char* 	str_trim(char *str);
int 	str_starts_with(const char *str, const char *substr);
char* 	str_strip_quotes(char *str);
char* 	str_strip_brackets(char *str);
string 	convert_method(const int &m);
void 	send_to_socket(const int &sock,const char *msg, const int &len_msg);
void	send_res_to_socket(const int &sock, http_response_t *res);
void	send_req_to_socket(const int &sock, http_request_t *req);
void 	http_parse_from_socket(const int &sock);
void 	safe_copy(char *&des, const char *src, const int &len_src, int &size_des, int &len_des);
string 	doubletostr(const double &t);
char* 	str_concat(const char *str1, const char *str2);
void 	write_to_file(FILE *fr,const char *buffer, const int &length);
string 	exec(const char* cmd);
void 	log(string msg);
void 	nprintln(const char *chr, const int length);
void	print_debug(const char *format, ...);
int 	parse_options(int argc, char *argv[]);
void	run_server_mirror();
string 	mac_eth0();
string 	mac_wlan0();
#endif