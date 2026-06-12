#pragma once
#include "window.hpp"
#include <vector>

struct Message {
    const char *subject;
    const char *from;     // display name shown in the list
    const char *date;     // short date shown in the list
    const char *size;
    bool unread;
    const char *fromAddr; // full From: header in the preview
    const char *to;       // To: header in the preview
    const char *dateFull; // Date: header in the preview
    const char *body;     // message body shown in the preview
};

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

    std::vector<Message> m_messages;

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
