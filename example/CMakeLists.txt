# Example 示例应用
project(Mel_Example VERSION 1.0.0)

# 扫描 example 源文件
file(GLOB_RECURSE example_srcs CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
)

# 在 IDE 中按目录结构组织文件
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${example_srcs})

# 创建可执行文件
add_executable(${PROJECT_NAME} ${example_srcs})

# Windows 平台不显示控制台窗口
if(WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
elseif(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 引入 ElaWidgetTools（必须在创建目标之前）
include(../cmake/ela.cmake)

# 链接 Mel ElaWidgetTools
target_link_libraries(${PROJECT_NAME} PRIVATE Mel ElaWidgetTools)

# Windows 平台特定配置
set(QT_LIBS Core Gui Widgets)
# MinGW 需要额外的运行时库
set(QT_PATH_LIBS libwinpthread-1 libstdc++-6 libgcc_s_seh-1)
set(QT_PLUGINS
        styles/qwindowsvistastyle
        platforms/qwindows
        imageformats/qjpeg
)

if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/win_run.cmake)
    copy_target(${PROJECT_NAME} Mel ElaWidgetTools)

    include(${CMAKE_SOURCE_DIR}/cmake/qt_win_run.cmake)
    copy_qt_libs(${PROJECT_NAME} ${QT_LIBS})
    if (MINGW)
        copy_qt_path_libs(${PROJECT_NAME} ${QT_PATH_LIBS})
    endif()
    copy_qt_plugins(${PROJECT_NAME} ${QT_PLUGINS})
endif()

# 安装示例可执行文件
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
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
