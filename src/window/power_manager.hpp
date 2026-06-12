#pragma once
#include "window.hpp"

class PowerManager : public Window {
public:
    PowerManager();

    void draw() override;
    ImVec2 position() override;
    ImVec2 size() override;

    bool isOpen() const { return m_open; }
    void open() { m_open = true; }
    void close() { m_open = false; }

private:
    bool m_open = false;

    // Plugged in (true) vs on battery (false), shared by System & Display tabs.
    bool m_pluggedIn = true;

    // --- General ---
    int m_powerButtonAction = 0;
    int m_sleepButtonAction = 0;
    int m_hibernateButtonAction = 0;
    int m_batteryButtonAction = 0;
    bool m_handleBrightnessKeys = true;
    int m_brightnessStepCount = 10;
    bool m_brightnessExponential = false;
    bool m_statusNotifications = true;
    bool m_systemTrayIcon = false;

    // --- System ---
    int m_systemSleepMode = 0;       // Suspend
    int m_inactiveFor = 0;           // 0 == Never
    int m_lidAction = 0;
    int m_powerProfile = 1;          // performance
    int m_criticalLevel = 10;
    int m_criticalAction = 0;        // Notify
    bool m_lockOnSleep = true;

    // --- Display (per power source) ---
    bool m_displayPowerManagement = true;
    int m_sleepAfter = 5;            // minutes
    int m_switchOffAfter = 10;       // minutes
    int m_reduceBrightnessTo = 80;   // percent
    int m_reduceAfter = 150;         // seconds

    void drawGeneralTab();
    void drawSystemTab();
    void drawDisplayTab();
    void drawDevicesTab();
    void drawPowerSourceSelector();

    // Devices tab selection.
    int m_selectedDevice = 0;
};
