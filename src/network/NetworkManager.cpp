#include "network/NetworkManager.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

NetworkManager::NetworkManager() : socket_(-1) {}

NetworkManager::~NetworkManager() { close(); }

bool NetworkManager::startServer(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return false;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        ::close(server_fd);
        return false;
    }

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        perror("bind");
        ::close(server_fd);
        return false;
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        ::close(server_fd);
        return false;
    }

    std::cout << "Waiting for opponent to connect..." << std::endl;
    sockaddr_in client_addr{};
    socklen_t addrlen = sizeof(client_addr);
    socket_ = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
    ::close(server_fd);
    if (socket_ < 0) {
        perror("accept");
        return false;
    }

    std::cout << "Opponent connected." << std::endl;
    return true;
}

bool NetworkManager::connectToServer(const std::string& host, int port) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        perror("socket");
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        ::close(socket_);
        return false;
    }

    if (connect(socket_, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        perror("connect");
        ::close(socket_);
        return false;
    }

    return true;
}

bool NetworkManager::sendMessage(const std::string& msg) {
    std::string data = msg + "\n";
    const char* buf = data.c_str();
    size_t total = 0;
    size_t len = data.size();
    while (total < len) {
        ssize_t sent = send(socket_, buf + total, len - total, 0);
        if (sent <= 0) {
            return false;
        }
        total += static_cast<size_t>(sent);
    }
    return true;
}

bool NetworkManager::receiveMessage(std::string& msg) {
    msg.clear();
    char ch;
    while (true) {
        ssize_t received = recv(socket_, &ch, 1, 0);
        if (received <= 0) {
            return false;
        }
        if (ch == '\n') {
            break;
        }
        msg.push_back(ch);
    }
    return true;
}

void NetworkManager::close() {
    if (socket_ >= 0) {
        ::close(socket_);
        socket_ = -1;
    }
}

