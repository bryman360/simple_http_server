#include "messages.hpp"


Response::Response() {
    http_opener = "HTTP/1.1";
    status_code = "404";
    status = "Not Found";
    body = "";
}


std::string Response::message() {
    std::string output = "";
    output += http_opener + " " + status_code + " " + status + "\r\n";
    for (auto const& header : headers) {
        output += header.first + ":";
        for (int i=0; i < header.second.size(); i++) {
            output += " " + header.second[i];
            if (i < header.second.size() - 1) output += ",";
        }
        output += "\r\n";
    }
    output += "\r\n";
    output += body;
    return output;
}


void Response::add_header_content(std::string header, std::string content) {
    if (headers.find(header) == headers.end()) headers[header] = std::vector<std::string>();
    headers[header].push_back(content);
}