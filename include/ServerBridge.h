#pragma once
#ifndef SERVER_BRIDGE_H
#define SERVER_BRIDGE_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <winsock2.h>

#endif

namespace ServerBridge
{
    bool DoesBridgeExist(std::string ip)
    {
        int port = 80;

        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return false;
        }

        // Create a socket
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket." << std::endl;
            WSACleanup();
            return false;
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
        serverAddress.sin_port = htons(port);

        // Connect to the server
        if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
        {
            // Connection failed
            std::cerr << "Connection failed." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return false;
        }

        // Connection successful
        std::cout << "Server at " << ip << " is alive." << std::endl;

        // Cleanup
        closesocket(clientSocket);
        WSACleanup();

        return true;
    }
}