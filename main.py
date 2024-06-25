import socket


def main():
    server_socket = socket.create_server(("localhost", 4221), reuse_port=True)
    #server_socket.accept()[0].sendall(b"HTTP/1.1 200 OK\r\n\r\n")
    #server_socket.accept()[0].sendall(b"GET /index.html HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\"")
    print("Ready to receive")
    conn, addr = server_socket.accept()
    msg = conn.recv(2048).decode("utf-8")
    split_msg = msg.split()
    path = split_msg[1]
    reply=b"HTTP/1.1 404 Not Found\r\n\r\n"
    if path == "/":
        reply = b"HTTP/1.1 200 OK\r\n\r\n"

    elif path[:5] == "/echo":
        reply = b"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + str.encode(str(len(path) - 6)) + b"\r\n\r\n" + str.encode(path[6:])
    elif path == "/user-agent":
        header_found = False
        for part in split_msg:
            if header_found:
                agent_name_len = len(part)
                reply = b"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + str.encode(str(len(part))) + b"\r\n\r\n" + str.encode(part)
                break
            if part == "User-Agent:":
                header_found = True

    print(reply)
    conn.send(reply)

if __name__ == "__main__":
    main()