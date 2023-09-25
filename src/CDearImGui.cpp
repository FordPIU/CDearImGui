#include "ProcessManager.h"

int main()
{
    ProcessManager proccess = ProcessManager::ProcessManager();
    proccess.launch(1, 1, "Hello World!", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
    /*
    // Init GLFW to setup OpenGL
    InitGlfw();

    // Init the Proccess Window itself
    GLFWwindow *proccessWindow = glfwCreateWindow(WindowWidth, WindowHeight, "NON-EXSISTENT SERVER", nullptr, nullptr);
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
    bool isWindowAOpen = true;
    bool isWindowBOpen = true;
    int counter = 0;
    int counter2 = 0;

    ServerList::LaunchServerListThread(proccessWindow);

    while (!glfwWindowShouldClose(proccessWindow))
    {
        // Have OpenGL pull input events, so you can actually close and move shit
        glfwPollEvents();

        // Initalize ImGui for this Frame
        InitImGui_Frame();

        ServerList::FrameCall();

        // WindowA
        if (isWindowAOpen)
        {
            ImGui::Begin("Window A", &isWindowAOpen);
            ImGui::Text("Hello, world!");
            if (ImGui::Button("Count!"))
                counter++;
            ImGui::Text("counter = %d", counter);
            ImGui::End();
        }

        // WindowB
        if (isWindowBOpen)
        {
            ImGui::Begin("Window B", &isWindowBOpen);
            ImGui::Text("Hello, world 2!");
            if (ImGui::Button("Count!"))
                counter2 += 5;
            ImGui::Text("counter = %d", counter2);
            ImGui::End();
        }

        // Check if both windows are closed
        if (!isWindowAOpen && !isWindowBOpen)
        {
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

    return 0;
    */
}