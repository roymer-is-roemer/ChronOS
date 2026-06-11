#pragma once

#include "window.hpp"
#include <functional>

class Taskbar : public Window {
private:
    std::function<void()> m_onPowerClick;
    bool m_showPowerConfirm = false;
public:
	void draw() override;
	ImVec2 position() override;
	ImVec2 size() override;

	Taskbar(std::function<void()> onPowerClick);
};
