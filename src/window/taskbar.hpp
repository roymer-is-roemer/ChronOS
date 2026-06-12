#pragma once

#include "window.hpp"
#include <functional>
#include "task_manager.hpp"

class Taskbar : public Window {
private:
    std::function<void()> m_onPowerClick;
    bool m_showPowerConfirm = false;
    std::unique_ptr<TaskManager> m_taskManager;
public:
	void draw() override;
	ImVec2 position() override;
	ImVec2 size() override;

	Taskbar(std::function<void()> onPowerClick);
};
