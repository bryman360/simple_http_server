# simple_http_server

This is a simple HTTP/Web Server that handles some basic GET/POST requests.

This server requires Python3 to run. Development was done on Python 3.10.12

To run this server, just run `main.py`

Files can be saved via POST requests. To specify where you want these files saved, use the `--directory` argument followed by the path to the folder you want to save to.
By default, files are saved to `/tmp/`

**GET Requests:**
/ will return a 200 response.
/echo/<string> will yield a response with <string> as the body.
/user-agent will have the User-Agent from the request in the body.


**POST Requests:**
/files/<path_of_file_to_write_to> will write the body of the request into a file specified by the <path_of_file_to_write_to> and relative to the `--directory` folder.


This server also allows compression of the body of the responses it returns.
Currently, the only compression format supported is `gzip`

To have the response body be encoded, include an `Accept-Encoding: gzip` header in the request.

Example:
To test this server, you can use `curl` to send/receive messages.
`curl -v http://localhost:4221/echo/abc -H 'Accept-Encoding: gzip'`
