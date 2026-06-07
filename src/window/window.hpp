#pragma once

#include "imgui.h"
#include <string>

class Window {
private:
	std::string name;
	ImGuiWindowFlags window_flags;
public:
	virtual ImVec2 position() { return ImVec2(); };
	virtual ImVec2 size() { return ImVec2(); };

	virtual void draw() {};
	void render();

	Window(std::string name, ImGuiWindowFlags flags);
};
