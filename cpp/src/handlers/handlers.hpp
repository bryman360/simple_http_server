#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "../config.hpp"
#include "../messages/messages.hpp"
#include "../utils/utils.hpp"




void connection_handler(int client_fd, in_addr client_addr, in_port_t client_port);


void GET_handler(Request request, Response &response);


void POST_handler(Request request, Response &response);