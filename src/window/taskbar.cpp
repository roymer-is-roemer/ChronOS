#include "taskbar.hpp"
#include "window.hpp"
#include <ctime>

Taskbar::Taskbar(std::function<void()> onPowerClick)
    : Window("Taskbar",
             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoScrollbar),
      m_onPowerClick(std::move(onPowerClick)) {}

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

    if (ImGui::Button("TskMngr", ImVec2(36, 36))) {
        if (!m_taskManager) {
            m_taskManager = std::make_unique<TaskManager>();
        }
        m_taskManager->open();
    }
    ImGui::SameLine();

	std::string timeStr = GetCurrentTimeString();
	float text_width = ImGui::CalcTextSize(timeStr.c_str()).x;
	float window_width = ImGui::GetWindowSize().x;

	ImGui::SameLine();
	ImGui::SetCursorPosX(window_width - text_width - 60.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
	ImGui::Text("%s", timeStr.c_str());

	ImGui::SameLine();
	ImGui::SetCursorPosX(window_width - 26.0f - 15.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    if (ImGui::Button("(|)", ImVec2(26.0f, 24))){
        m_showPowerConfirm = true;
        ImGui::OpenPopup("Confirm Shutdown");
    }
    ImGui::PopStyleColor(3);

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Confirm Shutdown", &m_showPowerConfirm,
                                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("Are you sure you want to shut down?");
        ImGui::Spacing();

        float buttonWidth = 80.0f;
        float spacing = 10.0f;
        float totalWidth = buttonWidth * 2 + spacing;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

        if (ImGui::Button("Shut Down", ImVec2(buttonWidth, 0))) {
            ImGui::CloseCurrentPopup();
            m_showPowerConfirm = false;
            if (m_onPowerClick) m_onPowerClick();
        }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
            ImGui::CloseCurrentPopup();
            m_showPowerConfirm = false;
        }

        ImGui::EndPopup();
    }

    if (m_taskManager) {
    m_taskManager->draw();
}
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
