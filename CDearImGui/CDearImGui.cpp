#include "CImGui.h"

#include <iostream>

using namespace std;

const int WindowHeight = 1;
const int WindowWidth = 1;

int main() {
	// Init GLFW to setup OpenGL
	InitGlfw();

	// Init the Proccess Window itself
	GLFWwindow* proccessWindow = glfwCreateWindow(WindowWidth, WindowHeight, "NON-EXSISTENT SERVER", nullptr, nullptr);
	WindowExists(proccessWindow);

	// Set the window to be active its at the top of the screen
	glfwMakeContextCurrent(proccessWindow);

	// Load OpenGL
	gladLoadGL();

	// Set the Viewport
	glViewport(0, 0, WindowWidth, WindowHeight);

	// Set the Init Background Color of the Proccess Window
	SetWindowBkg(0.f, 0.f, 50.f, 150.f);
	
	// Init ImGUI
	InitImGui(proccessWindow);

	// Generate Server Status Thread
	vector<string> servers = loadSavedServers();
	thread statusUpdateThread([&proccessWindow]() {
		while (!glfwWindowShouldClose(proccessWindow)) {
			for (const string& server : loadSavedServers()) {
				updateServerStatus(server);
			}
			this_thread::sleep_for(chrono::seconds(5));
		}
		});

	bool isServerListOpen = true;
	char newServerInputBuffer[256] = "";
	bool isWindowAOpen = true;
	bool isWindowBOpen = true;
	int counter = 0;
	int counter2 = 0;

	while (!glfwWindowShouldClose(proccessWindow))
	{
		// Have OpenGL pull input events, so you can actually close and move shit
		glfwPollEvents();

		// Initalize ImGui for this Frame
		InitImGui_Frame();

		// Main Menu
		if (isServerListOpen) {
			servers = loadSavedServers();

			ImGui::Begin("Server List", &isServerListOpen);

			ImGui::InputText("Server IP & Port", newServerInputBuffer, 256);
			ImGui::SameLine();
			if (ImGui::Button("Add")) {
				if (isServerInputValid(newServerInputBuffer)) {
					saveNewServer(newServerInputBuffer);
				}
				strcpy_s(newServerInputBuffer, "");
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete")) {
				if (isServerInputValid(newServerInputBuffer)) {
					removeServer(newServerInputBuffer);
				}
				strcpy_s(newServerInputBuffer, "");
			}
			
			int i = 0;
			for (const string& server : servers) {
				if (!newServerInputBuffer[0] || strstr(server.c_str(), newServerInputBuffer)) {
					// Column Organizer
					if (i > 0) {
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

					if (ImGui::Button(stringToCharPointer(status + " " + server))) {
						strcpy_s(newServerInputBuffer, sizeof(newServerInputBuffer), server.c_str());
					}

					// Column Tracker Set
					i = (i + 1) % 2;
				}
			}


			ImGui::End();
		}

		// WindowA
		if (isWindowAOpen) {
			ImGui::Begin("Window A", &isWindowAOpen);
			ImGui::Text("Hello, world!");
			if (ImGui::Button("Count!"))
				counter++;
			ImGui::Text("counter = %d", counter);
			ImGui::End();
		}

		// WindowB
		if (isWindowBOpen) {
			ImGui::Begin("Window B", &isWindowBOpen);
			ImGui::Text("Hello, world 2!");
			if (ImGui::Button("Count!"))
				counter2 += 5;
			ImGui::Text("counter = %d", counter2);
			ImGui::End();
		}

		// Check if both windows are closed
		if (!isWindowAOpen && !isWindowBOpen) {
			glfwSetWindowShouldClose(proccessWindow, GLFW_TRUE);
		}

		// Update the Window Viewport for resizing to take effect
		UpdateViewport(proccessWindow);

		// Set the Window Background again, or else it flickers for whatever reason
		SetWindowBkg(0.f, 0.f, 50.f, 150.f);

		// End Calls for ImGui
		EFImGui();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		// Unsure why, but if you don't do this, window just goes white with no content.
		glfwSwapBuffers(proccessWindow);
	}

	// Cleanup
	Cleanup(proccessWindow);
	statusUpdateThread.join();

	return 0;
}