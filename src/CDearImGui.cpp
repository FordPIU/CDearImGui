#include "Process.h"
#include "Timer.h"
#include "ServerBridge.h"
#include "StorageManager.h"

#include <windows.h>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <unordered_map>

////////////////////////////////////////////////////////////

int GetUnixEpochTime()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    // Convert duration to seconds and return as int
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
}

int GetUnixEpochTimeMillis()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    // Convert duration to milliseconds and return as int
    return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}

////////////////////////////////////////////////////////////

bool MainWindow_IsOpen = true;
bool popup_newBridgeConnection = false;
char inbuf_serverIp[16] = "";

void menuBar()
{
    if (ImGui::BeginMenuBar())
    {
        // CONNECTION
        if (ImGui::BeginMenu("Bridge"))
        {
            ImGui::MenuItem("Add New Bridge", NULL, &popup_newBridgeConnection);
            ImGui::EndMenu();
        }

        // ABOUT
        if (ImGui::BeginMenu("About"))
        {
            // GITHUB
            if (ImGui::Button("Project Github"))
            {
                string url = "https://github.com/FordPIU/CDearImGui";

                std::wstring wideUrl(url.begin(), url.end());
                ShellExecuteW(NULL, L"open", wideUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
            }

            // CREATORS
            if (ImGui::BeginMenu("Creator(s)"))
            {
                ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.8f, 1.f), "Caleb Brodock");
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Popups
    if (popup_newBridgeConnection)
    {
        ImGui::OpenPopup("Server IP");
        if (ImGui::BeginPopupModal("Server IP"))
        {
            ImGui::InputText("##InputField", inbuf_serverIp, IM_ARRAYSIZE(inbuf_serverIp));

            if (ImGui::Button("OK"))
            {
                StorageManager::SaveIPAddress(inbuf_serverIp);

                strcpy_s(inbuf_serverIp, "");
                popup_newBridgeConnection = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                popup_newBridgeConnection = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

std::unordered_map<std::string, bool> serverConnectionStatus;
void serverList()
{
    vector<string> ipAddresss = StorageManager::GetIPAddressList();

    // If no bridge connections have been added, we can simply return.
    if (ipAddresss.empty())
    {
        ImVec2 textSize = ImGui::CalcTextSize("No Bridges have been added.");
        float windowWidth = ImGui::GetWindowWidth();
        float centerX = (windowWidth - textSize.x) * 0.5f;
        ImGui::SetCursorPosX(centerX);
        ImGui::TextColored(ImVec4(1.f, 0.5f, 0.5f, 1.f), "No Bridges have been added.");
        return;
    }

    // Set up some style options for a specific section
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f)); // Increase spacing between items
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f)); // Increase padding within buttons

    // Draw out every bridge's section
    for (const string &bridgeIp : ipAddresss)
    {
        // IM GUI RENDERING
        ImGui::BeginGroup();

        // SERVER IP HEADER
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Server IP: %s", bridgeIp.c_str());
        ImGui::SameLine();

        // SERVER STATUS DOT
        bool serverConnected = serverConnectionStatus[bridgeIp] || false;
        ImVec4 dotColor = (serverConnected) ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, dotColor);
        ImGui::Button("##ServerStatusDot", ImVec2(12.5f, 12.5f));
        ImGui::PopStyleColor();

        // CONNECT BUTTON
        if (serverConnected == true)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.5f, 0.f, 1.0f));
            if (ImGui::Button("Connect", ImVec2(80.0f, 0.0f)))
            {
                // LaunchBridge(bridgeIp);
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }

        // REMOVE BUTTON
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.f, 0.f, 1.0f));
        std::string buttonLabel = "Remove " + bridgeIp;
        if (ImGui::Button(buttonLabel.c_str()))
        {
            StorageManager::RemoveIPAddress(bridgeIp);
        }
        ImGui::PopStyleColor();

        ImGui::EndGroup();
        ImGui::Separator();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

bool perFrame(int lastFrameTime)
{
    if (MainWindow_IsOpen)
    {
        // int uEpochStart = GetUnixEpochTimeMillis();

        // Begin ImGui Instance
        ImGui::Begin("CDearImGui", &MainWindow_IsOpen, ImGuiWindowFlags_MenuBar);

        // Begin the Menu Bar
        menuBar();

        // Server List
        serverList();

        ImGui::End();

        // int uEpochEnd = GetUnixEpochTimeMillis();
        // std::cout << "Time to render: " << (uEpochEnd - uEpochStart) << " ms." << std::endl;
    }

    if (!MainWindow_IsOpen)
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////

void serverStatusFunct()
{
    while (true)
    {
        vector<string> ipAddresss = StorageManager::GetIPAddressList();

        for (const string &bridgeIp : ipAddresss)
        {
            bool isConnected = ServerBridge::DoesBridgeExist(bridgeIp);
            serverConnectionStatus[bridgeIp] = isConnected;
            std::cout << "Server: " << bridgeIp << " Status: " << isConnected << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::jthread serverStatusThread(serverStatusFunct);
    Process proccess = Process(perFrame);
    proccess.launch(1, 1, "CDearImGui", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
}

////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();

    return 0;
}