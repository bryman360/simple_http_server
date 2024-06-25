import socket
import threading


def main():
    server_socket = socket.create_server(("localhost", 4221), reuse_port=True)
    print("Ready to receive")
    while True:
        conn, addr = server_socket.accept()
        threading.Thread(target=send_response, args=(conn, addr)).start()
        

def send_response(conn, addr):
    msg = conn.recv(1024).decode("utf-8")
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
                reply = b"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + str.encode(str(len(part))) + b"\r\n\r\n" + str.encode(part)
                break
            if part == "User-Agent:":
                header_found = True

    conn.sendall(reply)

if __name__ == "__main__":
    main()