#pragma once
#ifndef SERVER_BRIDGE_H
#define SERVER_BRIDGE_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <winsock2.h>

std::string sendHttpRequest(const std::string &server, const std::string &path)
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return "";
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return "";
    }

    // Resolve the server address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(80); // HTTP default port
    hostent *hostInfo = gethostbyname(server.c_str());
    if (hostInfo == nullptr)
    {
        std::cerr << "Error resolving server address." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "";
    }
    serverAddr.sin_addr = *((in_addr *)hostInfo->h_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "";
    }

    // Prepare the HTTP GET request
    std::string httpRequest = "GET " + path + " HTTP/1.1\r\n"
                                              "Host: " +
                              server + "\r\n"
                                       "Connection: close\r\n\r\n";

    // Send the HTTP request
    if (send(clientSocket, httpRequest.c_str(), httpRequest.length(), 0) == SOCKET_ERROR)
    {
        std::cerr << "Error sending HTTP request." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "";
    }

    // Receive and print the HTTP response
    std::string httpResponse;
    char buffer[4096];
    int bytesReceived;
    do
    {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0)
        {
            httpResponse.append(buffer, bytesReceived);
        }
    } while (bytesReceived > 0);

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return httpResponse;
}

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

    bool ConnectToBridge(std::string ip)
    {
        std::string returnFromServer = sendHttpRequest(ip, "/bridgeAlive");

        if (returnFromServer.contains("HTTP/1.1 200 OK"))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}