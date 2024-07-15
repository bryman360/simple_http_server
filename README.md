# simple_http_server

This is a simple HTTP/Web Server that handles some basic GET/POST requests. This is for practice/demonstration purposes only.

Two versions of this server were built: First was in Python 3.10.12 and the second in C++11. Both were built on a Linux Platform.

To run the Python version, run the `main.py` file.
To build and run the C++ version, run the `local_server.sh` file. (Note: Due to pathing, this only works on a Linux file system)

Files can be saved via POST requests. To specify where you want these files saved, use the `--directory` argument followed by the path to the folder you want to save to.
For example, you can specify the directory for the C++ version by running `./local_server.sh --directory ./files_to_be_saved/tmp`
By default, files are saved to `/tmp/`

## **GET Requests:**

`/` will return a 200 response.

`/echo/<string>` will yield a response with <string> as the body.

`/user-agent` will have the User-Agent from the request in the body of the response.

`/files/<file_path/name>` will allow fetching of a file from the directory specified above. (Note: This has no security measures. A file outside of the specified directory can be fetched. Be careful.)

This server also allows compression of the body of the responses it returns.
Currently, the only compression format supported is `gzip`. To have the response body be encoded, include an `Accept-Encoding: gzip` header in the request.

## **POST Requests:**

`/files/<path_of_file_to_write_to>` will write the body of the request into a file specified by the <path_of_file_to_write_to> and relative to the `--directory` folder.



## Testing
To test this server, you can use `curl` to send/receive messages to/from the server.
`curl -v "http://localhost:4221/echo/abc" -H 'Accept-Encoding: gzip'`
