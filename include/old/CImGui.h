#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <stdexcept>

#include <thread>
#include <mutex>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

void InitGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
}

void WindowExists(GLFWwindow *window)
{
    if (window == NULL)
    {
        glfwTerminate();
        throw runtime_error("GLFW Window Creation Failed.");
    }
}

void SetWindowBkg(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void InitImGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void InitImGui_Frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Cleanup(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void UpdateViewport(GLFWwindow *window)
{
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
}

void EFImGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool isServerInputValid(const string &input)
{
    // Regular expression pattern for IP:Port format
    regex pattern(R"(^(?:[0-9]{1,3}\.){3}[0-9]{1,3}:[0-9]{1,5}$)");

    return regex_match(input, pattern);
}

vector<string> loadSavedServers()
{
    vector<string> servers;
    ifstream file("servers.txt");
    if (!file.is_open())
    {
        cout << "Error opening servers file" << endl;
        return servers;
    }

    string line;
    while (getline(file, line))
    {
        servers.push_back(line);
    }

    file.close();
    return servers;
}

void saveNewServer(const string &newServer)
{
    ofstream file("servers.txt", ios::app);
    if (!file.is_open())
    {
        cout << "Error opening servers file" << endl;
        return;
    }

    file << newServer << endl;
    file.close();
}

void removeServer(const string &serverToRemove)
{
    ifstream inFile("servers.txt");
    if (!inFile.is_open())
    {
        cout << "Error opening servers file" << endl;
        return;
    }

    vector<string> servers;
    string line;
    while (getline(inFile, line))
    {
        servers.push_back(line);
    }
    inFile.close();

    ofstream outFile("servers.txt");
    if (!outFile.is_open())
    {
        cout << "Error opening servers file" << endl;
        return;
    }

    for (const string &server : servers)
    {
        if (server != serverToRemove)
        {
            outFile << server << endl;
        }
    }
    outFile.close();
}

char *stringToCharPointer(const string &str)
{
    char *charPtr = new char[str.length() + 1];
    strcpy_s(charPtr, str.length() + 1, str.c_str());
    return charPtr;
}

bool isServerAlive(const string &serverAddress)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed" << endl;
        return false;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Failed to create socket" << endl;
        WSACleanup();
        return false;
    }

    string serverIP;
    string serverPortStr;
    size_t colonPos = serverAddress.find(":");
    if (colonPos != string::npos)
    {
        serverIP = serverAddress.substr(0, colonPos);
        serverPortStr = serverAddress.substr(colonPos + 1);
    }
    else
    {
        cerr << "Invalid server address format" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddressInfo{};
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_port = htons(stoi(serverPortStr));

    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddressInfo.sin_addr) <= 0)
    {
        cerr << "Invalid IP address" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddressInfo), sizeof(serverAddressInfo)) == SOCKET_ERROR)
    {
        cerr << "Failed to connect to server" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    closesocket(clientSocket);
    WSACleanup();
    return true;
}

mutex serverStatusMutex;
map<string, bool> serverStatusMap;

void updateServerStatus(const string &serverAddress)
{
    bool status = isServerAlive(serverAddress);

    lock_guard<mutex> lock(serverStatusMutex);
    serverStatusMap[serverAddress] = status;
}

int getServerStatus(const string &serverAddress)
{
    lock_guard<mutex> lock(serverStatusMutex);
    auto it = serverStatusMap.find(serverAddress);
    if (it != serverStatusMap.end())
    {
        if (it->second)
        {
            return 100; // Alive
        }
        else
        {
            return 101; // Not Alive
        }
    }
    else
    {
        return 102; // Pending
    }
}