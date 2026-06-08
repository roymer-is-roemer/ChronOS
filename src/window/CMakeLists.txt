target_sources(OSMockup PRIVATE
    window.cpp
    taskbar.cpp
    desktop_background.cpp
)

target_include_directories(OSMockup PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
