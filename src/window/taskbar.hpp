#pragma once

#include "window.hpp"

class Taskbar : public Window {
private:
public:
	void draw() override;
	ImVec2 position() override;
	ImVec2 size() override;

	Taskbar();
};
