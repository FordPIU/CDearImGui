#pragma once
#ifndef SERVER_BRIDGE_H
#define SERVER_BRIDGE_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#endif

namespace ServerBridge
{
    int ConnectToServer(const char *ip)
    {
        int port = 5165;

        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return 1;
        }

        // Create a socket
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket." << std::endl;
            WSACleanup();
            return 1;
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(ip); // inet_addr is depreciated, but for now it works.
        serverAddress.sin_port = htons(port);

        // Connect to the server
        if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
        {
            std::cerr << "Connection failed." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // Connection successful, need to add logic

        // Cleanup
        closesocket(clientSocket);
        WSACleanup();

        return 0;
    }
}