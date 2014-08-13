#include "net.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "util.hpp"
#include <iostream>
#include <map>
#include <netdb.h>
#include <cstdlib>

using std::string;

const int port_airplay          =   7000;
const int port_screen_mirror    =   7100;

int net_bind_socket(int port)
{
    int sockfd, newsockfd, pid;
    struct sockaddr_in serv_addr;

    /* server */

    sockfd                      =   socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket server control");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family        =   AF_INET;
    serv_addr.sin_addr.s_addr   =   INADDR_ANY;
    serv_addr.sin_port          =   htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);

    return sockfd;
}

int net_bind_socket_airplay()
{
    return net_bind_socket(port_airplay);
}
int net_bind_socket_screen_mirror()
{
    return net_bind_socket(port_screen_mirror);
}

int net_search_socket_remote_control()
{
    std::map<string, string> map;
    string cmd, remote_searched;

    cmd = "avahi-browse _airplay._tcp -r -t | grep address";
    remote_searched = exec(cmd.c_str());
    attrs_brackets_map_str_parse(map, remote_searched.c_str());

    cmd = "avahi-browse _airplay._tcp -r -t | grep port";
    remote_searched = exec(cmd.c_str());
    attrs_brackets_map_str_parse(map, remote_searched.c_str());

    std::cout << map["address"] << "\n" << map["port"] << "\n";
    if ((map["address"] != "") && (map["port"] != ""))
    {
        int     sockfd, portno, n;
        struct  sockaddr_in serv_addr;
        struct  hostent *server;

        portno = atoi(map["port"].c_str());
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            std::cout << "ERROR opening socket remote control\n";
        server = gethostbyname(map["address"].c_str());
        if (server == NULL)
        {
            std::cout << "ERROR, no such host remote control\n";
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            std::cout << "ERROR connecting remote control\n";

        return sockfd;
    }
    else
    {
        return -1;
    }
}