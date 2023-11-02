#pragma once

#ifndef PROCESS_H
#define PROCESS_H
#include "ColorVectors.h"
#include "Timer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif // PROCESS_H

typedef bool (*FunctionPointer)(int);

class Process
{
public:
    Process(FunctionPointer frameFunction)
    {
        frameCallFunc = frameFunction;
    }

    void launch(int winWidth, int winHeight, const char *winName, int winPosX, int winPosY, rgbaVec winBkgRGBA)
    {
        // Initalize GLFW Context
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

        // Create Window
        processWindow = glfwCreateWindow(winWidth, winHeight, winName, nullptr, nullptr);

        if (processWindow == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("GLFW Window Creation Failed.");
        }

        // Set Context
        glfwMakeContextCurrent(processWindow);

        // Initalize OpenGL
        gladLoadGL();
        glViewport(winPosX, winPosY, winWidth, winHeight);
        glClearColor(winBkgRGBA.r, winBkgRGBA.g, winBkgRGBA.b, winBkgRGBA.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Initalize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplGlfw_InitForOpenGL(processWindow, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Last frames duration / completion time
        int last_frame_duration = 0;

        while (!glfwWindowShouldClose(processWindow) && !windowTerminate)
        {
            // Start a timer
            Timer timer = Timer();

            // Handle per-frame initialization
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Call the per frame function
            bool shouldClose = frameCallFunc(last_frame_duration);

            // Check if the per frame function
            //   requested to close the window
            if (shouldClose)
            {
                glfwSetWindowShouldClose(processWindow, GLFW_TRUE);
            }

            // // Handle per-frame de-initalization
            // Update the viewport for resizing to take effect
            int display_w, display_h;
            glfwGetFramebufferSize(processWindow, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            // Set the window background, or else flickering occurs
            glClearColor(winBkgRGBA.r, winBkgRGBA.g, winBkgRGBA.b, winBkgRGBA.a);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            glfwSwapBuffers(processWindow);

            // End the timer, and set frame duration
            last_frame_duration = timer.endTimer();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(processWindow);
        glfwTerminate();
    }

    void terminate()
    {
        windowTerminate = true;
    }

private:
    GLFWwindow *processWindow;
    FunctionPointer frameCallFunc;

    bool windowTerminate = false;
};