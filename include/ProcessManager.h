#pragma once

#ifndef PROCCESS_MANAGER
#define PROCCESS_MANAGER
#include <stdexcept>

#include "ColorVectors.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

class ProcessManager
{
public:
    ProcessManager()
    {
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
    }

private:
    GLFWwindow *proccessWindow;
};