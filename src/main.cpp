#include "imgui.h"
#include "os_window.hpp"
#include <ctime>
#include <string>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window/taskbar.hpp"
#include "image.hpp"

int main()
{
    //ImGuiIO &io = ImGui::GetIO();
    //(void)io;
	OSWindow window;

	Taskbar taskbar;

	Image image("/Users/rek/Coding/ChronOS/test_output.oif");

    while (!glfwWindowShouldClose(window.window))
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

		auto sz = ImVec2(251, 80);
		ImGui::Image((ImTextureID)(intptr_t)image.gl_texture_id, ImVec2(image.width, image.height));

        // Taskbar
		taskbar.render();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window.window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.window);
    }

    return 0;
}
