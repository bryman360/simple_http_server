#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "messages/messages.hpp"
#define BUFF_SIZE 65536

int main(int argc, char **argv) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
      std::cerr << "Failed to create server socket\n";
      return 1;
    }

    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
      std::cerr << "setsockopt failed\n";
      return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
      std::cerr << "Failed to bind to port 4221\n";
      return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
      std::cerr << "listen failed\n";
      return 1;
    }
    Request("Hey");
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    std::cout << "Waiting for a client to connect...\n";

    std::string read_buffer(65536, '\0');
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    std::cout << "Client connected via " << inet_ntoa(client_addr.sin_addr) << " on port " << ntohs(server_addr.sin_port) << "\n";
    int bytesReceived = read(client_fd, &read_buffer[0], 65536);
    if (bytesReceived < 0) {
      std::cout << "recv() failed";
    }
    else {
      // std::cout << read_buffer;
      Request request(read_buffer);
      Response def_response;
      std::cout << "Request message: ";
      std::cout << request.message() << std::endl;
      std::cout << "Default Response message: ";
      std::cout << def_response.message() << std::endl;

      const std::string response{"HTTP/1.1 200 OK\r\n\r\n"};
      send(client_fd, &response[0], response.length(), 0);
    }

    close(server_fd);

    return 0;
}
