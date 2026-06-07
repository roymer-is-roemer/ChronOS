#include "os_window.hpp"

#include <cstdlib>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

OSWindow::OSWindow() {
    if (!glfwInit()) {
		std::cerr << "Unable to initialize GLFW! Ensure that your system has a GUI system supported by GLFW." << std::endl;
		exit(-1);
	}
	
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "OS Mockup", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
		std::cerr << "Unable to create GLFW window!" << std::endl;
		exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
}

OSWindow::~OSWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
