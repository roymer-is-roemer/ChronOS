#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <ctime>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

std::string GetCurrentTimeString()
{
    std::time_t t = std::time(nullptr);

    struct tm *tm_info = std::localtime(&t);

    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S %p", tm_info);

    return std::string(buffer);
}

int main()
{
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OS Mockup", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImDrawList *bg_draw_list = ImGui::GetBackgroundDrawList();

        ImVec2 screen_pos = ImGui::GetMainViewport()->Pos;
        ImVec2 screen_size = ImGui::GetMainViewport()->Size;

        ImU32 color_top_left = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.2f, 0.3f, 1.0f));  // Dark Blue
        ImU32 color_bot_right = ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.1f, 0.2f, 1.0f)); // Dark Purple

        bg_draw_list->AddRectFilledMultiColor(
            screen_pos,
            ImVec2(screen_pos.x + screen_size.x, screen_pos.y + screen_size.y),
            color_top_left, color_top_left, color_bot_right, color_bot_right);

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        float taskbar_height = 40.0f;

        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkSize.y - taskbar_height));
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, taskbar_height));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoScrollbar;
        // Taskbar
        ImGui::Begin("Taskbar", nullptr, window_flags);

        // Start Button
        if (ImGui::Button("Start", ImVec2(60, 24)))
        {
            /* logic */
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10.0f, 0.0f));
        ImGui::SameLine();


        if (ImGui::Button("Button 1", ImVec2(36, 36)))
        {
        }
        ImGui::SameLine();

        if (ImGui::Button("Button 2", ImVec2(36, 36)))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("Button 3", ImVec2(36, 36)))
        {
        }

        std::string timeStr = GetCurrentTimeString();
        float text_width = ImGui::CalcTextSize(timeStr.c_str()).x;
        float window_width = ImGui::GetWindowSize().x;

        ImGui::SameLine();
        ImGui::SetCursorPosX(window_width - text_width - 15.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
        ImGui::Text("%s", timeStr.c_str());

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}