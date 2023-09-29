#pragma once

#ifndef SERVER_LIST_H
#define SERVER_LIST_H
#include "CImGui.h"
#endif

bool isServerListOpen = true;
char newServerInputBuffer[256] = "";

namespace ServerList
{
    void LaunchServerListThread(GLFWwindow *&proccessWindow)
    {
        vector<string> servers = loadSavedServers();
        thread statusUpdateThread([&proccessWindow]()
                                  {
			while (!glfwWindowShouldClose(proccessWindow)) {
				for (const string& server : loadSavedServers()) {
					updateServerStatus(server);
				}
				this_thread::sleep_for(chrono::seconds(15));
			} });

        statusUpdateThread.detach();
    }

    void FrameCall()
    {
        if (isServerListOpen)
        {
            vector<string> servers = loadSavedServers();

            ImGui::Begin("Server List", &isServerListOpen);

            ImGui::InputText("Server IP & Port", newServerInputBuffer, 256);
            ImGui::SameLine();
            if (ImGui::Button("Add"))
            {
                if (isServerInputValid(newServerInputBuffer))
                {
                    saveNewServer(newServerInputBuffer);
                }
                strcpy_s(newServerInputBuffer, "");
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete"))
            {
                if (isServerInputValid(newServerInputBuffer))
                {
                    removeServer(newServerInputBuffer);
                }
                strcpy_s(newServerInputBuffer, "");
            }

            int i = 0;
            for (const string &server : servers)
            {
                if (!newServerInputBuffer[0] || strstr(server.c_str(), newServerInputBuffer))
                {
                    // Column Organizer
                    if (i > 0)
                    {
                        ImGui::SameLine();
                    }

                    // Ping the server
                    string status = "";
                    int serverStatus = getServerStatus(server);
                    if (serverStatus == 100)
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "S");
                    else if (serverStatus == 101)
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "S");
                    else if (serverStatus == 102)
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "S");
                    else
                        ImGui::TextColored(ImVec4(0.5f, 0, 0.5f, 1), "S");

                    ImGui::SameLine();

                    if (ImGui::Button(stringToCharPointer(status + " " + server)))
                    {
                        strcpy_s(newServerInputBuffer, sizeof(newServerInputBuffer), server.c_str());
                    }

                    // Column Tracker Set
                    i = (i + 1) % 2;
                }
            }

            ImGui::End();
        }
    }
}