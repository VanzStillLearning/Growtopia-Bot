cmake_minimum_required(VERSION 3.16)

project(Growtopia_Bot 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Growtopia bot with Qt and ENet"
)

# Modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 first, fall back to Qt5 if not available
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Widgets)
    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets)
else()
    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets)
endif()

# Platform detection
if(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux/Android/Termux")
    set(IS_LINUX TRUE)
elseif(APPLE)
    message(STATUS "Building for macOS")
    set(IS_MACOS TRUE)
elseif(WIN32)
    message(STATUS "Building for Windows")
    set(IS_WINDOWS TRUE)
endif()

# ENet library
add_library(enet STATIC
    Growtopia_Bot/enet2/callbacks.c
    Growtopia_Bot/enet2/compress.c
    Growtopia_Bot/enet2/host.c
    Growtopia_Bot/enet2/list.c
    Growtopia_Bot/enet2/packet.c
    Growtopia_Bot/enet2/peer.c
    Growtopia_Bot/enet2/protocol.c
    Growtopia_Bot/enet2/unix.c
    Growtopia_Bot/enet2/win32.c
)

target_include_directories(enet PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Growtopia_Bot/enet2/include
)

# Platform-specific ENet definitions
if(IS_WINDOWS)
    target_compile_definitions(enet PRIVATE HAS_FCNTL=1 HAS_POLL=1)
    target_link_libraries(enet PRIVATE ws2_32 winmm)
elseif(IS_LINUX OR IS_MACOS)
    target_compile_definitions(enet PRIVATE HAS_FCNTL=1 HAS_POLL=1 HAS_GETHOSTBYNAME_R=1 HAS_INET_PTON=1 HAS_INET_NTOP=1)
    if(IS_MACOS)
        target_compile_definitions(enet PRIVATE HAS_MSGHDR_FLAGS=1)
    endif()
endif()

# Main executable
add_executable(growtopia_bot
    Growtopia_Bot/main.cpp
    Growtopia_Bot/mainwindow.cpp
    Growtopia_Bot/mainwindow.h
    Growtopia_Bot/mainwindow.ui
    Growtopia_Bot/corefunc.h
    Growtopia_Bot/userfunc.h
    Growtopia_Bot/utilsfunc.h
    Growtopia_Bot/addbots.h
    Growtopia_Bot/addbots.cpp
)

target_include_directories(growtopia_bot PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Growtopia_Bot
    ${CMAKE_CURRENT_SOURCE_DIR}/Growtopia_Bot/include
)

target_link_libraries(growtopia_bot PRIVATE
    ${QT_LIBRARIES}
    enet
)

# Platform-specific linking
if(IS_WINDOWS)
    target_link_libraries(growtopia_bot PRIVATE ws2_32 winmm)
elseif(IS_LINUX OR IS_MACOS)
    # No special linking needed for Unix-like systems
endif()

# Compiler optimizations
if(MSVC)
    target_compile_options(growtopia_bot PRIVATE /W4 /O2)
else()
    target_compile_options(growtopia_bot PRIVATE -Wall -Wextra -O2)
endif()

# Installation
install(TARGETS growtopia_bot DESTINATION bin)

# Print build configuration
message(STATUS "=== Build Configuration ===")
message(STATUS "Qt Version: ${Qt${QT_VERSION_MAJOR}_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
