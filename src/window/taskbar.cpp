#include "taskbar.hpp"
#include "window.hpp"
#include <ctime>

Taskbar::Taskbar()
	: Window("Taskbar",
			 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
				 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				 ImGuiWindowFlags_NoSavedSettings |
				 ImGuiWindowFlags_NoScrollbar) {}

std::string GetCurrentTimeString() {
	std::time_t t = std::time(nullptr);

	struct tm *tm_info = std::localtime(&t);

	char buffer[16];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S %p", tm_info);

	return std::string(buffer);
}

void Taskbar::draw() {
	if (ImGui::Button("Start", ImVec2(60, 24))) {
	}
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(10.0f, 0.0f));
	ImGui::SameLine();

	if (ImGui::Button("Button 1", ImVec2(36, 36))) {
	}
	ImGui::SameLine();

	if (ImGui::Button("Button 2", ImVec2(36, 36))) {
	}
	ImGui::SameLine();

	if (ImGui::Button("Button 3", ImVec2(36, 36))) {
	}

	std::string timeStr = GetCurrentTimeString();
	float text_width = ImGui::CalcTextSize(timeStr.c_str()).x;
	float window_width = ImGui::GetWindowSize().x;

	ImGui::SameLine();
	ImGui::SetCursorPosX(window_width - text_width - 15.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
	ImGui::Text("%s", timeStr.c_str());
}

ImVec2 Taskbar::position() {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	float taskbar_height = 40.0f;
	return ImVec2(viewport->WorkPos.x, viewport->WorkSize.y - taskbar_height);
}

ImVec2 Taskbar::size() {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	float taskbar_height = 40.0f;
	return ImVec2(viewport->WorkSize.x, taskbar_height);
}
