import argparse
import os
import socket
import threading
from typing import List
from datetime import datetime

from Messages import Request, Response


file_storage_path = ''


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('--directory', default='.')
    args = parser.parse_args()
    directory_arg = args.directory.split('/')
    global file_storage_path
    if directory_arg[0] == '':
        directory_arg[0] = '/'
    elif directory_arg[0] != '~' and directory_arg[0] != '.':
        file_storage_path = '.'
    for folder in directory_arg:
        if folder == '' or folder == '\n':
            continue
        file_storage_path = os.path.join(file_storage_path, folder)
    server_addr = "localhost"
    server_port = 4221
    server_socket = socket.create_server(("localhost", 4221), reuse_port=True)
    print(f"{datetime.now()} Starting HTTP server up at {server_addr} on port {server_port}...")
    while True:
        conn, addr = server_socket.accept()
        threading.Thread(target=handle_connection, args=(conn, addr)).start()
        

def handle_connection(conn: socket, addr) -> None:
    print(datetime.now(), "Established connection with", addr, "and received a request")
    response = receive_message(conn)
    conn.sendall(str(response).encode())
    print(datetime.now(), "Sent response message to", addr)


def receive_message(conn: socket) -> Response:
    msg = conn.recv(1024).decode("utf-8")
    msg_content = Request(msg)
    if msg_content.request_type == "GET":
        return handle_get_request(msg_content)
    elif msg_content.request_type == "POST":
        return handle_post_request(msg_content)
    else:
        bad_response = Response()
        bad_response.status_code = "400"
        bad_response.status = "Bad Request"
        return bad_response


def handle_get_request(request: Request) -> Response:
    response = Response()
    if len(request.path) == 1:
        response.status_code = "200"
        response.status = "OK"
        return response
    
    split_path = request.path[1:].split('/')
    if split_path[0] == "echo":
        response.status_code = "200"
        response.status = "OK"
        response.headers['Content-Type'] = 'text/plain'
        response.headers['Content-Length'] = str(len(request.path[6:]))
        response.body = request.path[6:]
    elif split_path[0] == "user-agent":
        response.status_code = "200"
        response.status = "OK"
        response.headers['Content-Type'] = 'text/plain'
        response.headers['Content-Length'] = str(len(request.headers['User-Agent']))
        response.body = request.headers['User-Agent']
    elif split_path[0] == "files":
        for path_part in split_path[1:]:
            file_path = os.path.join(file_storage_path, path_part)
        try:
            with open(file_path) as file:
                file_content = file.read()
                response.status_code = "200"
                response.status = "OK"
                response.headers['Content-Type'] = 'application/octet-stream'
                response.headers['Content-Length'] = str(len(file_content))
                response.body = file_content
        except:
            pass
        
    return response


def handle_post_request(request: Request) -> Response:
    response = Response()
    return response


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print()
        print(datetime.now(), "Shutting down HTTP server...")
