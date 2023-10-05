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
    bool IsValidIPAddress(const char *ip)
    {
        std::vector<std::string> components;
        std::stringstream ss(ip);
        std::string component;

        // Split the IP address into components using '.' as the delimiter
        while (std::getline(ss, component, '.'))
        {
            components.push_back(component);
        }

        // Check if there are 4 components
        if (components.size() != 4)
        {
            return false;
        }

        // Check if each component is a valid number between 0 and 255
        for (const auto &comp : components)
        {
            try
            {
                int num = std::stoi(comp);
                if (num < 0 || num > 255)
                {
                    return false;
                }
            }
            catch (const std::invalid_argument &e)
            {
                return false; // Conversion to integer failed
            }
            catch (const std::out_of_range &e)
            {
                return false; // Integer is out of range
            }
        }

        return true;
    }

    int ConnectToServer(const char *ip)
    {
        int port = 5165;

        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return 2;
        }

        if (!IsValidIPAddress(ip))
        {
            std::cerr << "Invalid IP address!" << std::endl;
            WSACleanup();
            return 1;
        }

        // Create a socket
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket." << std::endl;
            WSACleanup();
            return 3;
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
            return 5;
        }

        // Connection successful, need to add logic

        // Cleanup
        closesocket(clientSocket);
        WSACleanup();

        return 0;
    }
}