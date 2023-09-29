#pragma once

#ifndef PROCCESS_MANAGER_H
#define PROCCESS_MANAGER_H
#include <stdexcept>

#include "ColorVectors.h"
#include "Timer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif // PROCCESS_MANAGER_H

typedef bool (*FunctionPointer)(int);

class ProcessManager
{
public:
    ProcessManager(FunctionPointer frameFunction)
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
        proccessWindow = glfwCreateWindow(winWidth, winHeight, winName, nullptr, nullptr);

        if (proccessWindow == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("GLFW Window Creation Failed.");
        }

        // Set Context
        glfwMakeContextCurrent(proccessWindow);

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

        ImGui_ImplGlfw_InitForOpenGL(proccessWindow, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Last frames duration / completion time
        int last_frame_duration = 0;

        while (!glfwWindowShouldClose(proccessWindow))
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
                glfwSetWindowShouldClose(proccessWindow, GLFW_TRUE);
            }

            // // Handle per-frame de-initalization
            // Update the viewport for resizing to take effect
            int display_w, display_h;
            glfwGetFramebufferSize(proccessWindow, &display_w, &display_h);
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

            glfwSwapBuffers(proccessWindow);

            // End the timer, and set frame duration
            last_frame_duration = timer.endTimer();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(proccessWindow);
        glfwTerminate();
    }

private:
    GLFWwindow *proccessWindow;
    FunctionPointer frameCallFunc;
};