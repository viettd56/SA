#ifndef _net_hpp_
#define _net_hpp_

//Create socket with port
int net_bind_socket(int port);
int net_bind_socket_airplay();
int net_bind_socket_screen_mirror();
int net_search_socket_remote_control(); //Return -1 if not found remote control

#endif