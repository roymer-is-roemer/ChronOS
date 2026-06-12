#pragma once
#include "window.hpp"
#include <vector>
#include <string>

struct ProcessInfo {
    std::string name;
    int pid;
    double cpu;
    double memoryMB;
    double diskMBs;
    double networkMbps;
};

class TaskManager : public Window {
public:
    TaskManager();

    void draw() override;
    ImVec2 position() override;
    ImVec2 size() override;

    bool isOpen() const { return m_open; }
    void open() { m_open = true; }
    void close() { m_open = false; }

private:
    bool m_open = false;
    std::vector<ProcessInfo> m_processes;
    double m_lastUpdate = 0.0;

    void updateStats();
};