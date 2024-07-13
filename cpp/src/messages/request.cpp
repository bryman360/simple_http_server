#include "messages.hpp"

std::string Request::message() {
    if (bad_request) {
        return "";
    }
    std::string output = "";
    output += request_type + " " + path + " " + http_opener + "\r\n";
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

Request::Request(std::string request_message) {
    request_type = "";
    http_opener = "HTTP/1.1";
    path = "";
    body = "";
    try {
        std::string clrf_delimiter = "\r\n";
        std::string double_clrf_delimiter = "\r\n\r\n";
        std::string header_delimiter = ": ";
        std::string message_no_filler = request_message.substr(0, request_message.find('\0'));
        std::vector<std::string> opener_and_rest_of_message_split = split_string(message_no_filler, clrf_delimiter, 1);
        std::vector<std::string> opener_split = split_string(opener_and_rest_of_message_split[0], " ", 2);
        std::vector<std::string> headers_with_content_and_body_split = split_string(opener_and_rest_of_message_split[1], double_clrf_delimiter, 1);
        std::vector<std::string> headers_with_content_separated = split_string(headers_with_content_and_body_split[0], clrf_delimiter);
        
        request_type = opener_split[0];
        path = opener_split[1];
        http_opener = opener_split[2];
        body = headers_with_content_and_body_split.size() > 1 ? headers_with_content_and_body_split[1] : "";

        if (headers_with_content_and_body_split[0] == clrf_delimiter) {
            return;
        }
        for (int i=0; i < headers_with_content_separated.size(); i++) {
            std::vector<std::string> header_and_content_split = split_string(headers_with_content_separated[i], header_delimiter, 1);
            std::string header = header_and_content_split[0];
            std::string header_content = header_and_content_split[1];
            std::vector<std::string> header_content_split = split_string(header_content);

            if (headers.find(header) == headers.end()) headers[header] = std::vector<std::string>();
            for (const std::string value : header_content_split) {
                if (value[value.length() - 1] == ',') headers[header].push_back(value.substr(0, value.length()-1));
                else headers[header].push_back(value);
            }

        }
        return;
    } catch (...) {
        bad_request = true;
    }

};
