#pragma once
#include "window.hpp"

class Taskbar : public Window {
private:
public:
	void draw() override;
	virtual ImVec2 position() override;
	virtual ImVec2 size() override;

	Taskbar();
};
