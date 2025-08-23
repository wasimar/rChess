#pragma once

#include <string>

// Simple network manager using TCP sockets for two-player communication
// Provides basic server and client setup along with message send/receive
class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Start as server and wait for a single client connection
    bool startServer(int port);

    // Connect to remote server as client
    bool connectToServer(const std::string& host, int port);

    // Send a newline-terminated message
    bool sendMessage(const std::string& msg);

    // Receive a message (newline-terminated)
    // Returns true on success, false on connection error
    bool receiveMessage(std::string& msg);

    // Close socket
    void close();

private:
    int socket_;
};

