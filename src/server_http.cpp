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
#include "http_request.hpp"

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;

    if (argc < 2)
    {
        error("ERROR, no port provided\n");
    }

    clilen = sizeof(cli_addr);

    sockfd = net_bind_socket(atoi(argv[1]));

    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    Http_request request(newsockfd);
    request.http_request_excute();
    
    close(newsockfd);
    close(sockfd);
    return 0; /* we never get here */
}

