#include "request_handlers.hpp"
#include "../utils.hpp"


void GET_handler(Request request, Response &response) {
    std::string path = request.get_path();
    std::vector<std::string> split_path = split_string(path, "/", 2);
    if (request.bad_request) {
        response.set_status_code("400");
        response.set_status("Bad Request");
        return;
    }

    response.set_status_code("200");
    response.set_status("OK");
    if (split_path[1] == "") {
    }
    else if (split_path[1] == "echo") {
        response.add_header_content("Content-Type", "text/plain");
        response.add_header_content("Content-Length", std::to_string(split_path[2].length()) );
        response.set_body(split_path[2]);
    }
    else if (split_path[1] == "user-agent") {
        std::string user_agent = request.get_header_content("User-Agent")[0];
        response.add_header_content("Content-Type", "text/plain");
        response.add_header_content("Content-Length", std::to_string(user_agent.length()) );
        response.set_body(user_agent);
    }
    else {
        response.set_status_code("404");
        response.set_status("Not Found");
    };
    return;
}


void POST_handler(Request request, Response &response) {
    return;
}