#pragma once
#include <arpa/inet.h>
#include <netdb.h>
#include "../messages/messages.hpp"


void connection_handler(int client_fd, in_addr client_addr, in_port_t client_port);


void GET_handler(Request request, Response &response);


void POST_handler(Request request, Response &response);