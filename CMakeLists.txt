cmake_minimum_required(VERSION 3.12)

project(QtHelloWorld 
    VERSION 1.0.0 
    DESCRIPTION "A Qt-based application with ElaWidgetTools"
    LANGUAGES CXX
)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 初始化 Qt 环境
include(cmake/qt_set.cmake)
init_qt()

# Qt 模块和插件配置
set(QT_LIBS Core Gui Widgets)
set(QT_PLUGINS
    styles/qwindowsvistastyle
    platforms/qwindows
    imageformats/qjpeg
)

# 引入 ElaWidgetTools（必须在创建目标之前）
include(cmake/ela.cmake)

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

# 创建可执行文件
add_executable(${PROJECT_NAME} ${srcs})

# Windows 平台不显示控制台窗口
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
elseif(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 链接 Qt 库
set_qt_libs(${PROJECT_NAME} ${QT_LIBS})

# 链接 ElaWidgetTools
target_link_libraries(${PROJECT_NAME} PRIVATE ElaWidgetTools)

# Windows 平台特定配置
if(WIN32)
    include(cmake/win_run.cmake)
    copy_target(${PROJECT_NAME} ElaWidgetTools)

    include(cmake/qt_win_run.cmake)
    copy_qt_libs(${PROJECT_NAME} ${QT_LIBS})
    copy_qt_plugins(${PROJECT_NAME} ${QT_PLUGINS})
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 配置摘要
message(STATUS "")
message(STATUS "========== 配置摘要 ==========")
message(STATUS "项目名称    : ${PROJECT_NAME}")
message(STATUS "项目版本    : ${PROJECT_VERSION}")
message(STATUS "C++ 标准    : ${CMAKE_CXX_STANDARD}")
message(STATUS "构建类型    : ${CMAKE_BUILD_TYPE}")
message(STATUS "输出目录    : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "================================")
message(STATUS "")
