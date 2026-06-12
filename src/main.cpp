#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "os_window.hpp"
#include "window/desktop_background.hpp"
#include "window/taskbar.hpp"
#include <GLFW/glfw3.h>
#include <ctime>
#include <string>

void setupFont();

int main() {
	// ImGuiIO &io = ImGui::GetIO();
	//(void)io;

	OSWindow window;

	Taskbar taskbar(
		[&window]() { glfwSetWindowShouldClose(window.window, GLFW_TRUE); });

	DesktopBackground desktop_background;

	setupFont();

	while (!glfwWindowShouldClose(window.window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImDrawList *bg_draw_list = ImGui::GetBackgroundDrawList();

		ImVec2 screen_pos = ImGui::GetMainViewport()->Pos;
		ImVec2 screen_size = ImGui::GetMainViewport()->Size;

		// ImU32 color_top_left = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f,
		// 0.2f, 0.3f, 1.0f));  // Dark Blue ImU32 color_bot_right =
		// ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.1f, 0.2f, 1.0f)); //
		// Dark Purple

		// bg_draw_list->AddRectFilledMultiColor(
		//     screen_pos,
		//     ImVec2(screen_pos.x + screen_size.x, screen_pos.y +
		//     screen_size.y), color_top_left, color_top_left, color_bot_right,
		//     color_bot_right);

		desktop_background.render();
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

void setupFont() {
	ImGuiIO &io = ImGui::GetIO();
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	static const ImWchar ranges[] = {
		0x0020,	 0x00FF,  // Basic Latin + Latin Supplement (normal text)
		0xE000,	 0xF8FF,  // BMP Private Use Area (Powerline, FA, Devicons...)
		0xF0000, 0xFFFFD, // Plane 15 PUA (Material Design icons)
		0,
	};
	io.Fonts->AddFontFromFileTTF("assets/MonacoNerdFont-Regular.ttf", 16.0f,
								 nullptr, ranges);
}