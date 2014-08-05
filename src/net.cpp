#include "net.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "util.hpp"

const int port_airplay = 7000;
const int port_screen_mirror = 7100;

int net_bind_socket(int port){
	int sockfd, newsockfd, pid;
    struct sockaddr_in serv_addr;

    /* server */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket server control");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);

    return sockfd;
}

int net_bind_socket_airplay(){
    return net_bind_socket(port_airplay);
}
int net_bind_socket_screen_mirror(){
    return net_bind_socket(port_screen_mirror);
}