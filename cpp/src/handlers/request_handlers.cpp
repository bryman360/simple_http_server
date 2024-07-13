#include "handlers.hpp"


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
    else if (split_path[1] == "files") {
        std::ifstream file (os_path_join(config::directory, split_path[2]));
        if (file.is_open()) {
            std::stringstream sstr;
            sstr << file.rdbuf();
            response.add_header_content("Content-Type", "application/octet-stream");
            response.set_body(sstr.str());
            response.add_header_content("Content-Length", std::to_string(sstr.str().length()));
        }
        else {
            response.set_status_code("404");
            response.set_status("Not Found");
            return;
        }
        
    }
    else {
        response.set_status_code("404");
        response.set_status("Not Found");
        return;
    };

    handle_universal_request_headers(request, response);
    return;
}


void POST_handler(Request request, Response &response) {
    std::string path = request.get_path();
    std::vector<std::string> split_path = split_string(path, "/", 2);;;
    std::string file_path = os_path_join(config::directory, split_path[2]);
    if (request.bad_request) {
        response.set_status_code("400");
        response.set_status("Bad Request");
        return;
    }

    response.set_status_code("201");
    response.set_status("Created");
    if (split_path[1] == "files") {
        std::ofstream file;
        file.open(file_path);
        if (file.is_open()){
            file << request.get_body();
            file.flush();
            file.close();
        }
        else {
            response.set_status_code("404");
            response.set_status("Not Found");
        }
    }
    else {
        response.set_status_code("404");
        response.set_status("Not Found");
    }
    return;
}


void handle_universal_request_headers(Request request, Response &response) {
    if (request.is_header_present("Accept-Encoding")) {
        std::vector<std::string> encoding_types = request.get_header_content("Accept-Encoding");
        for (std::string const encoding_type : encoding_types) {
            if (encoding_type == "gzip") {
                response.add_header_content("Content-Encoding", "gzip");
            }
        }
    }
}