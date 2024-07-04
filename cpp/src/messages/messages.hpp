#include <string>
#include <map>
#include <vector>
#include <iterator>

class Response {
    private:
    std::string http_opener = "HTTP/1.1";
    std::string status_code = "404";
    std::string status = "Not Found";
    std::map <std::string, std::vector<std::string>> headers;
    std::string body = "";

    public:
    Response();

    std::string message();
};

class Request {
    public:
    std::string request_type = "";
    std::string http_opener = "HTTP/1.1";
    std::string path = "";
    std::map<std::string, std::vector<std::string>> headers;
    std::string body = "";
    bool bad_request = false;

    Request(std::string request_message);

    std::string message();
};