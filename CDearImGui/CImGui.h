#pragma once

#ifndef CIMGUI_H_INCLUDED
#define CIMGUI_H_INCLUDED
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

class COpenGL {
public:
	COpenGL() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void newFrame() {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

class CWindow {
public:
	CWindow(const char* title, int width, int height) {
		this->window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (this->window == NULL)
		{
			glfwTerminate();
			throw std::runtime_error("GLFW Window Creation Failed.");
		}

		glfwMakeContextCurrent(this->window);
		gladLoadGL();

		glViewport(0, 0, 800, 800);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(this->window);
	}

	bool shouldClose() {
		return glfwWindowShouldClose(this->window);
	}

	void endFrame() {
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	void endProccess() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	GLFWwindow* getGLFWwindow() {
		return this->window;
	}

protected:
	GLFWwindow* window;
};

class CImGui {
public:
	CImGui(CWindow pWindow, const char* pGuiTitle) {
		this->window = pWindow.getGLFWwindow();
		this->guiTitle = pGuiTitle;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(this->window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void newFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(this->guiTitle);
	}

	void endFrame() {
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void endProccess() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void text(const char* pText) {
		ImGui::Text(pText);
	}

private:
	GLFWwindow* window;
	const char* guiTitle;
};