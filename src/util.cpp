#include "util.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


string str_nconcat_char(const string &des, const char *src, int n)
{
    if (src == NULL)
    {
        return des;
    }

    if (n == 0)
    {
        return des;
    }

    char *src_2 = new char[n + 1]();
    memcpy (src_2, src, n);
    *(src_2 + n + 1) = 0;

    string str_src(src_2);
    delete[] src_2;

    return des + str_src;
}

void attrs_map_str_parse(std::map<string, string> &map, const char *str)
{
    char *pair, *name, *value, *header_str, *original_ptr;
    header_str = strdup(str);
    original_ptr = header_str;

    while (isspace(*header_str)) header_str++;

    while ((pair = strsep(&header_str, ";")) && pair != NULL)
    {
        name = strsep(&pair, "=");
        value = strsep(&pair, "=");

        string str_name(str_trim(name));
        string str_value(str_trim(str_strip_quotes(value)));

        // map.insert (std::pair<string, string>(str_name, str_value));
        map[str_name] = str_value;
    }

    free(original_ptr);
}

void msgs_map_str_parse(std::map<string, string> &map, const char *str)
{
    char *pair, *name, *value, *header_str, *original_ptr;
    header_str = strdup(str);
    original_ptr = header_str;

    while (isspace(*header_str)) header_str++;

    while ((pair = strsep(&header_str, "\n")) && pair != NULL && strcmp(pair, ""))
    {
        //std::cout << "pair: " << pair << "\n";
        name = strsep(&pair, ":");
        value = pair;

        string str_name(str_trim(name));
        string str_value(str_trim(value));
        //std::cout << "N: " << str_name << "\n" << "V: " << str_value << "\n";
        // map.insert (std::pair<string, string>(str_name, str_value));
        map[str_name] = str_value;
    }

    free(original_ptr);
}

char *str_trim(char *str)
{
    char *end;

    while (isspace(*str)) str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    *(end + 1) = 0;

    return str;
}

int str_starts_with(const char *str, const char *substr)
{
    if ((str == NULL) || (substr == NULL)) return 0;
    return strncmp(str, substr, strlen(substr)) == 0;
}

static bool is_quote(char c)
{
    return (c == '"' || c == '\'');
}

char *str_strip_quotes(char *str)
{
    char *end;

    while (is_quote(*str)) str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && is_quote(*end)) end--;

    *(end + 1) = 0;

    return str;
}

string convert_method(int m)
{
    switch (m)
    {
        return "DELETE";
    case 1:
        return "GET";
    case 2:
        return "HEAD";
    case 3:
        return "POST";
    case 4:
        return "PUT";
    case 5:
        return "CONNECT";
    case 6:
        return "OPTIONS";
    case 7:
        return "TRACE";
    case 24:
        return "PATCH";
    case 25:
        return "PURGE";
    default:
        return "Undefined";
    }
}

void send_to_socket(int sock, const char *msg, int len_msg)
{
    int nmsg = write(sock, msg, len_msg);
    if (nmsg < 0) error("ERROR writing to socket");
}

void safe_copy(char *&des, const char *src, const int &len_src, int &size_des, int &len_des)
{
    if (len_des + len_src < size_des)
    {
        memcpy(des + len_des, src, len_src);
        len_des += len_src;
    }
    else
    {
        size_des = (len_des + len_src) * 2;
        char *temp = new char[size_des]();
        memcpy(temp, des, len_des);
        memcpy(temp + len_des, src, len_src);

        delete[] des;
        des = temp;
        std::cout << "temp: " << des << "\n";
        len_des += len_src;
    }
}

string doubletostr(const double &t)
{
    //std::cout << t << "\n";
    std::ostringstream os;
    os << t;
    return os.str();
}

char *str_concat(const char *str1, const char *str2) {
  if (str1 == NULL) {
    return strdup(str2);
  }

  if (str2 == NULL) {
    return strdup(str1);
  }

  int len1 = strlen(str1), len2 = strlen(str2);
  char *result = new char[strlen(str1) + strlen(str2) + 1]();

  memcpy(result, str1, len1);
  memcpy(result + len1, str2, len2 + 1);

  return result;
}
