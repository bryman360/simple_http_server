class Request:
    def __init__(self, message: str) -> None:
        self.request_type = None
        self.http_opener = "HTTP/1.1"
        self.path = ""
        self.headers = {}
        self.body = ""
        self._parse_message(message)
        self.bad_request = False

    def _parse_message(self, message: str):
        try:
            # Split the Request line and the headers from the body
            rn_parse = message.split('\r\n\r\n', 1)
            request_line_and_headers = rn_parse[0].split('\r\n', 1)
            self.body = rn_parse[1]

            # Parse out the request line
            request_line = request_line_and_headers[0].split()
            self.request_type = request_line[0]
            self.path = request_line[1]
            self.http_opener = request_line[2]

            # Parse out the headers
            headers_and_values = request_line_and_headers[1].split('\r\n')
            for header_and_value in headers_and_values:
                header_and_value_split = header_and_value.split()
                self.headers[header_and_value_split[0][:-1]] = header_and_value_split[1]
        except:
            self.bad_request = True

    def __str__(self) -> str:
        string = f"{self.request_type} {self.path} {self.http_opener}\r\n"
        for header in self.headers:
            string += f"{header}: {self.headers[header]}\r\n"
        string += f"\r\n"
        string += self.body
        return string
    

class Response:
    def __init__(self) -> None:
        self.http_opener = "HTTP/1.1"
        self.status_code = "404"
        self.status = "Not Found"
        self.headers = {}
        self.body = ""

    def __str__(self) -> str:
        string = f"{self.http_opener} {self.status_code} {self.status}\r\n"
        for header in self.headers:
            string += f"{header}: {self.headers[header]}\r\n"
        string += f"\r\n"
        string += self.body
        return string
