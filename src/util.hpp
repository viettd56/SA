#ifndef _util_hpp_
#define _util_hpp_
#include <string>
#include <map>

using std::string;

void error(const char* msg);
string str_nconcat_char(const string &des, const char *src, int n);
void attrs_map_str_parse(std::map<string, string> &map,const char *str);
void msgs_map_str_parse(std::map<string, string> &map,const char *str);
char *str_trim(char *str);
int str_starts_with(const char *str, const char *substr);
char *str_strip_quotes(char *str);
string convert_method(int m);
void send_to_socket(int sock,const char *msg, int len_msg);
void safe_copy(char *&des, const char *src, const int &len_src, int &size_des, int &len_des);
string doubletostr(const double &t);


#endif