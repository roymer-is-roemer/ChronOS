#include "window.hpp"

Window::Window(std::string name, ImGuiWindowFlags flags) : name{name}, window_flags{flags} {}

void Window::render() {
	ImVec2 size = this->size();
	ImVec2 position = this->position();
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(position);
	ImGui::Begin(name.c_str(), nullptr, window_flags);
	this->draw();
	ImGui::End();
}
