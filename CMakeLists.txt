cmake_minimum_required(VERSION 3.12)

project(QtHelloWorld VERSION 1.0.0 LANGUAGES CXX)

## -DCMAKE_PREFIX_PATH:PATH=C:\Qt\5.15.2\mingw81_64

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set Qt
include(cmake/qt_set.cmake)
init_qt()

# 自动扫描源文件
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui
)

# 可选：排除特定文件
# file(GLOB des CONFIGURE_DEPENDS
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/exclude_*.cpp
# )
# foreach(item ${des})
#     list(REMOVE_ITEM srcs ${item})
# endforeach()

# 在 IDE 中按目录结构组织文件
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${srcs})

# WIN32: Windows平台不显示控制台窗口
add_executable(${PROJECT_NAME} WIN32 ${srcs})

set(QT_LIBS Core Gui Widgets)
set(QT_PLUGINS
        styles/qwindowsvistastyle
        platforms/qwindows
        imageformats/qjpeg
)

# 链接qt
set_qt_libs(${PROJECT_NAME} ${QT_LIBS})


# 链接ElaWidgetTools
include(cmake/ela.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE ElaWidgetTools)

if (WIN32)
    include(cmake/win_run.cmake)
    copy_target(${PROJECT_NAME} ElaWidgetTools)

    include(cmake/qt_win_run.cmake)
    copy_qt_libs(${PROJECT_NAME} ${QT_LIBS})
    copy_qt_plugins(${PROJECT_NAME} ${QT_PLUGINS})
endif ()

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
