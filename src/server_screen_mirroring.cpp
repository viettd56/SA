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
#include <string>

int main(int argc, char *argv[])
{
    if (parse_options(argc, argv))
    {
        return 0;
    }

    int         sockfd, newsockfd, pid;
    socklen_t   clilen;
    struct      sockaddr_in cli_addr;
    log("------------server_mirror-------------");

    clilen      =   sizeof(cli_addr);
    sockfd      =   net_bind_socket_screen_mirror();
    while (1)
    {
        newsockfd   =   accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)
        {
            close(sockfd);
            http_parse_from_socket(newsockfd);
            exit(0);
        }
        else close(newsockfd);
    } /* end of while */
    close(sockfd);
    return 0; /* we never get here */
}

