#pragma once
#include <GLFW/glfw3.h>

class OSWindow {
private:
public:
    GLFWwindow* window;

    OSWindow();
    ~OSWindow();

    OSWindow(const OSWindow&) = delete;
    OSWindow& operator=(const OSWindow&) = delete;
};