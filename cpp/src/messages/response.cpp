#include "messages.hpp"

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
    return output;
}

Response::Response() {
}