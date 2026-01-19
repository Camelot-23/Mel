cmake_minimum_required(VERSION 3.12)

project(Mel_)

# -DCMAKE_PREFIX_PATH:PATH=C:\Qt\5.15.2\mingw81_64
# -DCMAKE_PREFIX_PATH:PATH=C:\Qt\6.6.3\mingw_64

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

# 构建选项
option(BUILD_EXAMPLES "构建示例应用" ON)

add_subdirectory(Mel)

# ========== 可选：添加 Example 示例应用 ==========
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

