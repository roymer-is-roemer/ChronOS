#include "desktop_background.hpp"

#include <cstdint>
#include <ctime>

#include "imgui.h"

// Image sourced from
// https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fwallpapercave.com%2Fwp%2Fwp14251246.jpg&f=1&nofb=1&ipt=e38bb99cdfe42e260f345958434ecfca1e0456f048da271576744d98089d978c
// and transformed into a binary format via the scripts/convertToImage.py
// script.
DesktopBackground::DesktopBackground() : image("wallpaper.oif") {}

void DesktopBackground::render() {
	auto viewport = ImGui::GetMainViewport();
	ImVec2 p_min = viewport->Pos;
	ImVec2 p_max =
		ImVec2(p_min.x + viewport->Size.x, p_min.y + viewport->Size.y);

	ImGui::GetBackgroundDrawList()->AddImage(
		(ImTextureID)(intptr_t)image.gl_texture_id, p_min, p_max,
		ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
}
