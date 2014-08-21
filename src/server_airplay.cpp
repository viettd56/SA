#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "net.hpp"
#include "util.hpp"

int main(int argc, char *argv[])
{
    if(parse_options(argc, argv))
    {
        return 0;
    }

    system("avahi-publish -s 'Apple TV' _airplay._tcp 7000  deviceid=70:1a:04:4c:eb:a2 features=0x39f7 model=AppleTV2,1 srcvers=130.14 &");

    log("------------server_airplay-------------");

    int         sockfd, newsockfd;
    socklen_t   clilen;
    struct      sockaddr_in cli_addr;

    clilen      =   sizeof(cli_addr);
    sockfd      =   net_bind_socket_airplay();
    newsockfd   =   accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
        error("ERROR on accept");

    http_parse_from_socket(newsockfd);

    close(newsockfd);
    close(sockfd);
    return 0; /* we never get here */
}

