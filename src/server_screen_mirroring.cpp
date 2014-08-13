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
#include "http_request_parse.hpp"
#include <string>

int main(int argc, char *argv[])
{
    int         sockfd, newsockfd;
    socklen_t   clilen;
    struct      sockaddr_in cli_addr;

    clilen      =   sizeof(cli_addr);
    sockfd      =   net_bind_socket_screen_mirror();
    newsockfd   =   accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    Http_request_parse request(newsockfd);
    request.http_request_parse_excute();

    close(newsockfd);
    close(sockfd);
    return 0; /* we never get here */
}

