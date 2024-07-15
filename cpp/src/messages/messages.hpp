#pragma once
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <iostream>
#include "../utils/utils.hpp"

class Response {
    private:
    std::string http_opener;
    std::string status_code;
    std::string status;
    std::map <std::string, std::vector<std::string>> headers;
    std::string body;

    public:
    Response();

    std::string message();
    void set_status_code(std::string new_status_code) { status_code = new_status_code; }
    void set_status(std::string new_status_message) { status = new_status_message; }
    void set_body(std::string new_body) { body = new_body; }
    void add_header_content(std::string header, std::string content);
    std::string get_body() {return body;}
    void reset_response();
};

class Request {
    private:
    std::string request_type;
    std::string http_opener;
    std::string path;
    std::map<std::string, std::vector<std::string>> headers;
    std::string body;

    public:
    Request(std::string request_message);
    bool bad_request = false;

    std::string message();
    std::string get_request_type() { return request_type; }
    std::string get_path() { return path; }
    std::string get_body() { return body; }
    bool is_header_present(std::string header) {return (headers.find(header) != headers.end());}
    std::vector<std::string> get_header_content(std::string header) { return headers[header];}
};