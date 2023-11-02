#include "Process.h"
#include "Timer.h"
#include "ServerBridge.h"
#include "StorageManager.h"

#include <windows.h>
#include <vector>
#include <cmath>

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

    // Draw out every bridge's section
    for (const string &bridgeIp : ipAddresss)
    {
        ImGui::NewLine();
        ImGui::BeginGroup();
        ImGui::Text("%s", bridgeIp.c_str());
        ImGui::EndGroup();
    }
}

bool perFrame(int lastFrameTime)
{
    if (MainWindow_IsOpen)
    {
        // Begin ImGui Instance
        ImGui::Begin("CDearImGui", &MainWindow_IsOpen, ImGuiWindowFlags_MenuBar);

        // Begin the Menu Bar
        menuBar();

        // Server List
        serverList();

        ImGui::End();
    }

    if (!MainWindow_IsOpen)
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////

int main()
{
    Process proccess = Process(perFrame);
    proccess.launch(1, 1, "CDearImGui", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
}

////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();

    return 0;
}