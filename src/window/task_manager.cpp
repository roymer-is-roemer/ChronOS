#include "task_manager.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

static const char* kProcessNames[] = {
    "System Idle Process", "explorer.exe", "chrome.exe", "svchost.exe",
    "Discord.exe", "Code.exe", "spotify.exe", "dwm.exe",
    "audiodg.exe", "RuntimeBroker.exe"
};

static constexpr double kUpdateInterval = 1.0;

TaskManager::TaskManager()
    : Window("Task Manager",
             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings) {

    std::srand((unsigned)std::time(nullptr));

    int pid = 1000;
    for (auto name : kProcessNames) {
        ProcessInfo p;
        p.name = name;
        p.pid = pid;
        pid += (std::rand() % 40) + 4;
        p.cpu = std::min((std::rand() % 1000) / 10.0, 99.9);
        p.memoryMB = (std::rand() % 80000) / 100.0 + 5.0;
        p.diskMBs = (std::rand() % 500) / 100.0;
        p.networkMbps = (std::rand() % 200) / 100.0;
        m_processes.push_back(p);
    }
    m_processes[0].cpu = (std::rand() % 30) / 10.0;
    m_processes[0].memoryMB = 0.1;
    m_processes[0].diskMBs = 0.0;
    m_processes[0].networkMbps = 0.0;
}

static double RandRange(double lo, double hi) {
    return lo + (std::rand() / (double)RAND_MAX) * (hi - lo);
}

void TaskManager::updateStats() {
    double now = ImGui::GetTime();
    if (now - m_lastUpdate < kUpdateInterval) return;
    m_lastUpdate = now;

    for (auto& p : m_processes) {
        p.cpu = std::clamp(p.cpu + RandRange(-5.0, 5.0), 0.0, 99.9);
        p.memoryMB = std::max(p.memoryMB + RandRange(-50.0, 50.0), 0.1);
        p.diskMBs = std::max(p.diskMBs + RandRange(-0.5, 0.5), 0.0);
        p.networkMbps = std::max(p.networkMbps + RandRange(-0.3, 0.3), 0.0);
    }
}

void TaskManager::draw() {
    if (!m_open) return;

    updateStats();

    bool open = m_open;
    if (!ImGui::Begin("Task Manager", &open,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::End();
        m_open = open;
        return;
    }

    if (ImGui::BeginTable("ProcessTable", 6,
                           ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                           ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY |
                           ImGuiTableFlags_Sortable)) {

        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableSetupColumn("CPU", ImGuiTableColumnFlags_WidthFixed, 70.0f);
        ImGui::TableSetupColumn("Memory", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Disk", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Network", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs()) {
            if (specs->SpecsDirty && specs->SpecsCount > 0) {
                const ImGuiTableColumnSortSpecs& sortSpec = specs->Specs[0];
                bool ascending = sortSpec.SortDirection == ImGuiSortDirection_Ascending;

                std::sort(m_processes.begin(), m_processes.end(),
                    [&](const ProcessInfo& a, const ProcessInfo& b) {
                        bool less;
                        switch (sortSpec.ColumnIndex) {
                            case 0: less = a.name < b.name; break;
                            case 1: less = a.pid < b.pid; break;
                            case 2: less = a.cpu < b.cpu; break;
                            case 3: less = a.memoryMB < b.memoryMB; break;
                            case 4: less = a.diskMBs < b.diskMBs; break;
                            case 5: less = a.networkMbps < b.networkMbps; break;
                            default: less = false; break;
                        }
                        return ascending ? less : (a.name != b.name || true) && !less;
                    });

                specs->SpecsDirty = false;
            }
        }

        for (auto& p : m_processes) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(p.name.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", p.pid);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f%%", p.cpu);

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f MB", p.memoryMB);

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.1f MB/s", p.diskMBs);

            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%.2f Mbps", p.networkMbps);
        }

        ImGui::EndTable();
    }

    ImGui::End();
    m_open = open;
}

ImVec2 TaskManager::position() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 sz = size();
    return ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x - sz.x) * 0.5f,
                   viewport->WorkPos.y + (viewport->WorkSize.y - sz.y) * 0.5f);
}

ImVec2 TaskManager::size() {
    return ImVec2(600.0f, 400.0f);
}