import argparse
import os
import socket
import threading
from typing import List


file_storage_path = '/'


class ResponseContent:
    def __init__(self) -> None:
        self.http_opener = "HTTP/1.1"
        self.status_code = "404"
        self.status = "Not Found"
        self.headers = {}
        self.body = ""

    def __str__(self) -> str:
        string = f"{{HTTP Opener: {self.http_opener}, Status Code: {self.status_code}, Status: {self.status}\n"
        string += f"Headers: {self.headers}\n"
        string += f"Body: '{self.body}' }}"
        return string


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--directory', default='.')
    args = parser.parse_args()
    directory_arg = args.directory.split('/')
    if directory_arg[0] == '':
        directory_arg[0] = '/'
    global file_storage_path
    for folder in directory_arg:
        if folder == '' or folder == '\n':
            continue
        file_storage_path = os.path.join(file_storage_path, folder)
    server_addr = "localhost"
    server_port = 4221
    server_socket = socket.create_server(("localhost", 4221), reuse_port=True)
    print(f"Starting HTTP server up at {server_addr} on port {server_port}...")
    while True:
        print("Listening...")
        conn, addr = server_socket.accept()
        threading.Thread(target=handle_connection, args=(conn, addr)).start()
        

def handle_connection(conn: socket, addr):
    print("Established connection with", addr)
    response_content = parse_message(conn)
    send_response(conn, response_content)


def parse_message(conn: socket):
    msg = conn.recv(1024).decode("utf-8")
    split_msg = msg.split()
    request_type = split_msg[0]
    path = split_msg[1]
    received_msg_content = split_msg[2:]

    if request_type == "GET":
        return gather_get_response_content(path, received_msg_content)
    else:
        conn.send(b"HTTP/1.1 400 Bad Request (Only GET supported)\r\n\r\n")


def gather_get_response_content(path: List[str], received_msg_content: List[str]):
    response_content = ResponseContent()
    if len(path) == 1:
        response_content.status_code = "200"
        response_content.status = "OK"
        return response_content
    
    split_path = path[1:].split('/')
    if split_path[0] == "echo":
        response_content.status_code = "200"
        response_content.status = "OK"
        response_content.headers['Content-Type'] = 'text/plain'
        response_content.headers['Content-Length'] = str(len(path[6:]))
        response_content.body = path[6:]
    elif split_path[0] == "user-agent":
        response_content.status_code = "200"
        response_content.status = "OK"
        response_content.headers['Content-Type'] = 'text/plain'
        for i, content in enumerate(received_msg_content):
            if content == "User-Agent:":
                response_content.headers["Content-Length"] = str(len(received_msg_content[i + 1]))
                response_content.body = received_msg_content[i + 1]
                break
    elif split_path[0] == "files":
        for path_part in split_path[1:]:
            file_path = os.path.join(file_storage_path, path_part)
        try:
            with open(file_path) as file:
                file_content = file.read()
                response_content.status_code = "200"
                response_content.status = "OK"
                response_content.headers['Content-Type'] = 'application/octet-stream'
                response_content.headers['Content-Length'] = str(len(file_content))
                response_content.body = file_content
        except:
            pass
        
    return response_content


def send_response(conn: socket, response_content: ResponseContent)->bool:
    reply = response_content.http_opener + " " + response_content.status_code + " " + response_content.status + "\r\n"
    for header in response_content.headers:
        reply += header + ": " + response_content.headers[header] + "\r\n"
    reply += "\r\n" + response_content.body
    conn.sendall(reply.encode())


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print()
        print("Shutting down HTTP server...")
