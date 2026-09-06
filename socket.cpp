#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int startTestServer(const std::string& host, int port) {
    // 1. Create socket (IPv4, TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error: Socket creation failed" << std::endl;
        return -1;
    }

    // Allow immediate reuse of the port after stopping the server
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Configure address structures
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // Convert host string (e.g., "127.0.0.1") to binary
    if (host == "localhost" || host == "0.0.0.0") {
        address.sin_addr.s_addr = INADDR_ANY;
    } else {
        address.sin_addr.s_addr = inet_addr(host.c_str());
    }

    // 3. Bind socket to the host and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Error: Bind failed on " << host << ":" << port << std::endl;
        close(server_fd);
        return -1;
    }

    // 4. Start listening for incoming connections (backlog queue of 10)
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Error: Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    std::cout << "Server listening on " << host << ":" << port << "...\n";
    return server_fd;
}

void runServerLoop(int server_fd) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Accept a single client connection (blocking)
    int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        std::cerr << "Error: Accept failed" << std::endl;
        return;
    }

    char buffer[4096];
    std::memset(buffer, 0, sizeof(buffer));

    // Read the raw HTTP request sent by curl or a browser
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        std::cout << "--- Received HTTP Request ---\n" << buffer << std::endl;
    }

    // Send a mock HTTP response back
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nConfig Parser Test: Success!\n";
    write(client_socket, response.c_str(), response.length());

    // Close the connection
    close(client_socket);
}