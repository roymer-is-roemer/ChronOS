target_sources(OSMockup PRIVATE
    window.cpp
    taskbar.cpp
    desktop_background.cpp
    task_manager.cpp
    power_manager.cpp
)

target_include_directories(OSMockup PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(imgui PUBLIC IMGUI_USE_WCHAR32)