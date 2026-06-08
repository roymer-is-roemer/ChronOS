#include "desktop_background.hpp"

#include <cstdint>
#include <ctime>
#include <iostream>

#include "imgui.h"

DesktopBackground::DesktopBackground() : image("/Users/rek/Coding/ChronOS/test_output.oif") {
	std::cout << "Image texture ID: " << image.gl_texture_id << std::endl;
}

void DesktopBackground::render() {
	auto viewport = ImGui::GetMainViewport();
	ImVec2 p_min = viewport->Pos; 
	ImVec2 p_max = ImVec2(p_min.x + viewport->Size.x, p_min.y + viewport->Size.y);

	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)(intptr_t)image.gl_texture_id, p_min, p_max, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
}
