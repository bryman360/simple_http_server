#include "handlers.hpp"


#define READ_BUFF_SIZE 65536


void connection_handler(int client_fd, in_addr client_addr, in_port_t client_port) {
    std::cout << "Client connected via " << inet_ntoa(client_addr) << " on port " << ntohs(client_port) << std::endl;
    // std::cout << "Threading" << std::endl;
    // return;

    std::string read_buffer(READ_BUFF_SIZE, '\0');
    int bytesReceived = read(client_fd, &read_buffer[0], READ_BUFF_SIZE);
    if (bytesReceived < 0) {
        std::cout << "recv() failed";
    }
    else {
        Request request(read_buffer);
        Response response;
        if (request.bad_request) {}
        else if (request.get_request_type() == "GET") {GET_handler(request, response);}
        else if (request.get_request_type() == "POST") {POST_handler(request, response);}

        std::string response_message = response.message();
        send(client_fd, &response_message[0], response_message.length(), 0);
    }
}