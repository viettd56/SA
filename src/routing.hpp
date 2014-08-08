#ifndef _ROUTING_HPP_
#define _ROUTING_HPP_

#include "request.hpp"

//For each request rq, excute callback
//sock: socket client
void routing_excute(const int &sock, const Request &rq);

#endif