#pragma once
#include "window.hpp"

class Mail : public Window {
public:
    Mail();

    void draw() override;
    ImVec2 position() override;
    ImVec2 size() override;

    bool isOpen() const { return m_open; }
    void open() { m_open = true; }
    void close() { m_open = false; }

private:
    bool m_open = false;

    int m_selectedFolder = 0;  // Inbox
    int m_selectedMessage = 0; // Welcome message
    int m_searchField = 0;     // Subject
    char m_searchBuf[128] = "";

    void drawMenuBar();
    void drawToolbar();
    void drawFolderPane();
    void drawMessageList();
    void drawPreviewPane();
    void drawStatusBar();
};
