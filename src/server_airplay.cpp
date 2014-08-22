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

void run();

int main(int argc, char *argv[])
{
    int pid;

    if (parse_options(argc, argv))
    {
        return 0;
    }

    system("avahi-publish -s 'Apple TV' _airplay._tcp 7000  deviceid=70:1a:04:4c:eb:a2 features=0x39f7 model=AppleTV2,1 srcvers=130.14 &");

    std::cout << "Airplay Service is running\n";

    pid = fork();
    if (pid < 0)
        error("ERROR on fork");
    if (pid == 0)
    {
        run_server_mirror();
    }
    if (pid > 0)
    {
        run();
        return 0; /* we never get here */
    }
}

void run()
{
    log("------------server_airplay-------------");

    int         sockfd, newsockfd, pid;
    socklen_t   clilen;
    struct      sockaddr_in cli_addr;

    clilen      =   sizeof(cli_addr);
    sockfd      =   net_bind_socket_airplay();

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
}