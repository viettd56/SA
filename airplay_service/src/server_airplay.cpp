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

void run();

int main(int argc, char *argv[])
{
    if (parse_options(argc, argv))
    {
        return 0;
    }

    int pid;

    string macaddr_eth0     =   mac_eth0();
    string macaddr_wlan0    =   mac_wlan0();

    if (macaddr_eth0 != "")
    {
        string cmd_avahi        =   "avahi-publish -s 'Apple TV LAN' _airplay._tcp 7000 deviceid=";
        cmd_avahi               +=  macaddr_eth0;
        cmd_avahi               +=  " features=0x37f5 model=AppleTV2,1 srcvers=130.14 &";

        system(cmd_avahi.c_str());
    }

    if (macaddr_wlan0 != "")
    {
        string cmd_avahi        =   "avahi-publish -s 'Apple TV WIFI' _airplay._tcp 7000 deviceid=";
        cmd_avahi               +=  macaddr_wlan0;
        cmd_avahi               +=  " features=0x37f5 model=AppleTV2,1 srcvers=130.14 &";

        system(cmd_avahi.c_str());
    }

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
