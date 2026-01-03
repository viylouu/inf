cmake_minimum_required(VERSION 3.16)
project(inf LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(inf STATIC)

target_sources(inf
    PRIVATE
        src/core/platform.c
        src/core/renderer.c

        src/plat/wl/wl.c
        src/plat/xlib/xlib.c

        src/rend/vk/vk.c
)

#target_include_directories(inf
#    PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/include
#)

if(UNIX AND NOT APPLE)
    target_link_libraries(inf
        PRIVATE
            X11
            wayland-client
            dl
            m
            vulkan
    )
endif()

if(WIN32)
    target_link_libraries(inf
        PRIVATE
            user32
            gdi32
            winmm
            vulkan1 # ? untested
    )
endif()

option(CORE_BUILD_TESTS "build core tests" ON)

if(CORE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
