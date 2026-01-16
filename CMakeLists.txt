cmake_minimum_required(VERSION 3.1)

project(QtHelloWorld VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic handling of Qt specific files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Option to specify Qt version (4, 5, or 6)
# Can be set via -DQT_VERSION=X on command line
if(NOT DEFINED QT_VERSION)
    # Try to find Qt6 first, then Qt5, then Qt4
    find_package(Qt6 QUIET COMPONENTS Widgets)
    if(Qt6_FOUND)
        set(QT_VERSION 6)
    else()
        find_package(Qt5 QUIET COMPONENTS Widgets)
        if(Qt5_FOUND)
            set(QT_VERSION 5)
        else()
            find_package(Qt4 QUIET COMPONENTS QtGui)
            if(Qt4_FOUND)
                set(QT_VERSION 4)
            else()
                message(FATAL_ERROR "No Qt installation found. Please install Qt4, Qt5, or Qt6.")
            endif()
        endif()
    endif()
endif()

message(STATUS "Using Qt version: ${QT_VERSION}")

# Find and configure Qt based on version
if(QT_VERSION EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(QT_LIBRARIES Qt6::Widgets)
    add_compile_definitions(QT_VERSION_MAJOR=6)
elseif(QT_VERSION EQUAL 5)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_LIBRARIES Qt5::Widgets)
    add_compile_definitions(QT_VERSION_MAJOR=5)
elseif(QT_VERSION EQUAL 4)
    find_package(Qt4 REQUIRED COMPONENTS QtGui)
    include(${QT_USE_FILE})
    set(QT_LIBRARIES ${QT_LIBRARIES})
    add_compile_definitions(QT_VERSION_MAJOR=4)
else()
    message(FATAL_ERROR "Unsupported Qt version: ${QT_VERSION}. Supported versions are 4, 5, and 6.")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

set(HEADERS
    src/mainwindow.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Qt Version: ${QT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
