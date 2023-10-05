#include "Process.h"
#include "Timer.h"
#include "ServerBridge.h"

#include <windows.h>
#include <vector>
#include <cmath>

////////////////////////////////////////////////////////////

bool MainWindow_IsOpen = true;
int status_ServerConnection = 0;
bool popup_newServerConnection = false;
char inbuf_newServerConnection[16] = "";

void connectToServer(char *serverIp)
{
    status_ServerConnection = 1;

    int connectionStatus = ServerBridge::ConnectToServer(serverIp);

    switch (connectionStatus)
    {
    case 0:
        std::cout << "Connection Successful!" << std::endl;

    case 1:
        status_ServerConnection = 2;
    case 2:
        status_ServerConnection = 3;
    case 3:
        status_ServerConnection = 3;
    case 4:
        status_ServerConnection = 3;
    case 5:
        status_ServerConnection = 4;
    }
}

void menuBar()
{
    if (ImGui::BeginMenuBar())
    {
        // CONNECTION
        if (ImGui::BeginMenu("Connect"))
        {
            ImGui::MenuItem("Connect to New Server", NULL, &popup_newServerConnection);
            if (ImGui::BeginMenu("Connection History"))
            {
                // Inject all previously connected servers
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        // ABOUT
        if (ImGui::BeginMenu("About"))
        {
            // GITHUB
            if (ImGui::Button("Project Github"))
            {
                std::string url = "https://github.com/FordPIU/CDearImGui";
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
    if (popup_newServerConnection)
    {
        ImGui::OpenPopup("Server IP");
        if (ImGui::BeginPopupModal("Server IP"))
        {
            ImGui::InputText("##InputField", inbuf_newServerConnection, IM_ARRAYSIZE(inbuf_newServerConnection));

            if (ImGui::Button("OK"))
            {
                strcpy_s(inbuf_newServerConnection, "");
                popup_newServerConnection = false;
                ImGui::CloseCurrentPopup();

                connectToServer(inbuf_newServerConnection);
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                popup_newServerConnection = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
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

        // Center Connect Status
        ImVec2 textSize = ImGui::CalcTextSize("Not connected to a server...");
        float windowWidth = ImGui::GetWindowWidth();
        float centerX = (windowWidth - textSize.x) * 0.5f;
        ImGui::SetCursorPosX(centerX);

        switch (status_ServerConnection)
        {
        case 0:
            ImGui::TextColored(ImVec4(1.f, 0.5f, 0.5f, 1.f), "Not connected to a server.");
            break;
        case 1:
            ImGui::TextColored(ImVec4(0.5f, 1.f, 1.f, 1.f), "Connecting...");
            break;
        case 2:
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.f), "Invalid IP Address Entered!");
            break;
        case 3:
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "An error occured whilst connecting.");
            break;
        case 4:
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.f), "Server is not responding...");
            break;
        }

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
    proccess.launch(1, 1, "Hello World!", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
}

////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();

    return 0;
}