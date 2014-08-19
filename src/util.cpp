#include "util.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

using std::cout;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


string str_nconcat_char(const string &des, const char *src, const int &n)
{
    if (src == NULL)
    {
        return des;
    }

    if (n == 0)
    {
        return des;
    }

    char src_2[n + 1];
    memcpy (&src_2, src, n);
    src_2[n] = 0;

    return des + src_2;
}

void attrs_quotes_map_str_parse(std::map<string, string> &map, const char *str)
{
    char *pair, *name, *value, *header_str, *original_ptr;
    header_str = strdup(str);
    original_ptr = header_str;

    while (isspace(*header_str)) header_str++;

    while ((pair = strsep(&header_str, ";")) && pair != NULL)
    {
        name    =   strsep(&pair, "=");
        value   =   strsep(&pair, "=");

        string str_name(str_trim(name));
        string str_value(str_trim(str_strip_quotes(value)));

        // map.insert (std::pair<string, string>(str_name, str_value));
        map[str_name] = str_value;
    }

    free(original_ptr);
}

void attrs_brackets_map_str_parse(std::map<string, string> &map, const char *str)
{
    char *pair, *name, *value, *header_str, *original_ptr;
    header_str = strdup(str);
    original_ptr = header_str;

    while (isspace(*header_str)) header_str++;

    while ((pair = strsep(&header_str, "\n")) && pair != NULL && strcmp(pair, ""))
    {
        name    =   strsep(&pair, "=");
        value   =   strsep(&pair, "=");

        string str_name(str_trim(name));
        string str_value(str_strip_brackets(str_trim(value)));

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
        name    =   strsep(&pair, ":");
        value   =   pair;

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

static bool is_bracket(char c)
{
    return (c == '[' || c == ']');
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

char *str_strip_brackets(char *str)
{
    char *end;
    while (is_bracket(*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && is_bracket(*end)) end--;

    *(end + 1) = 0;

    return str;
}


string convert_method(const int &m)
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

void send_to_socket(const int &sock, const char *msg, const int &len_msg)
{
    int nmsg = write(sock, msg, len_msg);
    if (nmsg < 0) error("ERROR writing to socket");
    // std::cout << "MSG: " << msg << "\n";
}

void send_res_to_socket(const int &sock, http_response_t *res)
{
    int datalen;
    const char *data_res = http_response_get_data(res, &datalen);
    cout << "data_res: ";
    nprintln(data_res, datalen);
    send_to_socket(sock, data_res, datalen);
}

void send_req_to_socket(const int &sock, http_request_t *req)
{
    int datalen;
    const char *data_req = http_request_get_data(req, &datalen);
    cout << "data_req: ";
    nprintln(data_req, datalen);
    send_to_socket(sock, data_req, datalen);
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
        log("delete des");
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

char *str_concat(const char *str1, const char *str2)
{
    if (str1 == NULL)
    {
        return strdup(str2);
    }

    if (str2 == NULL)
    {
        return strdup(str1);
    }

    int len1 = strlen(str1), len2 = strlen(str2);
    char *result = new char[strlen(str1) + strlen(str2) + 1]();

    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2 + 1);

    return result;
}

void write_to_file(FILE *fr, const char *buffer, const int &length)
{
    /* write data */
    fflush(fr);
    fwrite(buffer, sizeof(char), length, fr); /* write data to file */
    fclose(fr);
}

string exec(const char *cmd)
{
    FILE *pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

string read_from_socket(const int &sock)
{

    int     buffer_size     = 500;
    int     msg_size        = 500;
    int     msg_loaded      = 0;
    char    *buffer         = new char[buffer_size]();
    char    *msg            = new char[msg_size]();
    int     n               = 0;
    do
    {
        n = read(sock, buffer, buffer_size);
        cout << n << "\n";
        if (n < 0) error("ERROR reading response from socket");
        safe_copy(msg, buffer, n, msg_size, msg_loaded);
    }
    while (n > 0);
    string result(msg);
    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }
    log("delete buffer");
    if (msg != NULL)
    {
        delete[] msg;
        msg = NULL;
    }
    log("delete msg");
    return result;
}

void log(string msg)
{
    msg += "\n";
    FILE *fr = fopen("log.txt", "ab");
    write_to_file(fr, msg.c_str(), msg.length());
}

void nprintln(const char *chr, const int length)
{
    char temp[length + 1];
    memcpy(&temp, chr, length);
    cout << temp << "\n";
}