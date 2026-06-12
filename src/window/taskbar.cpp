#include "taskbar.hpp"
#include "window.hpp"
#include <ctime>

namespace {
std::string getCurrentTimeString() {
	std::time_t t = std::time(nullptr);

	struct tm *tm_info = std::localtime(&t);

	char buffer[std::size("2026-12-31 12:34:56 PM")];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S %p", tm_info);

	return std::string(buffer);
}

} // namespace

Taskbar::Taskbar(std::function<void()> onPowerClick)
	: Window("Taskbar",
			 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
				 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				 ImGuiWindowFlags_NoSavedSettings |
				 ImGuiWindowFlags_NoScrollbar),
	  m_onPowerClick(std::move(onPowerClick)) {}

void Taskbar::draw() {
	constexpr float buttonHeight = 24.0f;
	constexpr float buttonGap = 8.0f;
	constexpr float edgePadding = 15.0f;
	constexpr float powerButtonWidth = 26.0f;

	const float windowWidth = ImGui::GetWindowSize().x;
	const float windowHeight = ImGui::GetWindowSize().y;

	// Vertical center for taskbar items
	const float rowTop = (windowHeight - buttonHeight) * 0.5f;

	// Vertical center for text in button
	const float textTop =
		rowTop + (buttonHeight - ImGui::GetTextLineHeight()) * 0.5f;

	ImGui::SetCursorPosY(rowTop);

	if (ImGui::Button(" Start", ImVec2(60, buttonHeight))) {
	}
	ImGui::SameLine(0.0f, buttonGap);

	if (ImGui::Button(" Mail", ImVec2(0, buttonHeight))) { // 0 = fit width
	}
	ImGui::SameLine(0.0f, buttonGap);

	if (ImGui::Button("󱊣 Power Manager", ImVec2(0, buttonHeight))) {
		if (!m_powerManager) {
			m_powerManager = std::make_unique<PowerManager>();
		}
		m_powerManager->open();
	}
	ImGui::SameLine(0.0f, buttonGap);

	if (ImGui::Button(" Task Manager", ImVec2(0, buttonHeight))) {
		if (!m_taskManager) {
			m_taskManager = std::make_unique<TaskManager>();
		}
		m_taskManager->open();
	}

	// Power button pinned to the right with clock to its left
	const float powerButtonX = windowWidth - powerButtonWidth - edgePadding;

	std::string timeStr = getCurrentTimeString();
	float textWidth = ImGui::CalcTextSize(timeStr.c_str()).x;

	ImGui::SameLine();
	ImGui::SetCursorPosX(powerButtonX - textWidth - edgePadding);
	ImGui::SetCursorPosY(textTop);
	ImGui::Text("%s", timeStr.c_str());

	ImGui::SameLine();
	ImGui::SetCursorPosX(powerButtonX);
	ImGui::SetCursorPosY(rowTop);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
						  ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
						  ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
	if (ImGui::Button("󰐥", ImVec2(26.0f, 24))) {
		m_showPowerConfirm = true;
		ImGui::OpenPopup("Confirm Shutdown");
	}
	ImGui::PopStyleColor(3);

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirm Shutdown", &m_showPowerConfirm,
							   ImGuiWindowFlags_AlwaysAutoResize |
								   ImGuiWindowFlags_NoMove)) {
		ImGui::Text("Are you sure you want to shut down?");
		ImGui::Spacing();

		float buttonWidth = 80.0f;
		float spacing = 10.0f;
		float totalWidth = buttonWidth * 2 + spacing;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

		if (ImGui::Button("Shut Down", ImVec2(buttonWidth, 0))) {
			ImGui::CloseCurrentPopup();
			m_showPowerConfirm = false;
			if (m_onPowerClick)
				m_onPowerClick();
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

	if (m_powerManager) {
		m_powerManager->draw();
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
