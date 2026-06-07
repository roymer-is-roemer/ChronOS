target_sources(OSMockup PRIVATE
    os_window.cpp
    image.cpp
)

target_include_directories(OSMockup PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(window)
