# Example 示例应用
project(Mel_Example VERSION 1.0.0)

# 调试：输出 Qt 自动工具变量的值
message(STATUS "========== Example Qt 自动工具状态 ==========")
message(STATUS "CMAKE_AUTOMOC: ${CMAKE_AUTOMOC}")
message(STATUS "CMAKE_AUTOUIC: ${CMAKE_AUTOUIC}")
message(STATUS "CMAKE_AUTORCC: ${CMAKE_AUTORCC}")
message(STATUS "=============================================")

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

# 注意：AUTOMOC/AUTOUIC/AUTORCC 已在顶层 CMakeLists.txt 中通过 init_qt() 全局设置
# 理论上不需要在这里重复设置，但某些情况下显式设置更可靠
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     AUTOMOC ON
#     AUTOUIC ON
#     AUTORCC ON
# )

# Windows 平台不显示控制台窗口
if(WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# 设置编译选项
if(MSVC)
    # 移除默认的 /W3，避免与 /W4 冲突
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /W4                          # 警告级别 4
        /utf-8                       # 设置源和执行字符集为 UTF-8
        /wd4819                      # 禁用代码页警告
    )
elseif(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 链接 Qt 库
set(QT_LIBS Widgets)
set_qt_libs(${PROJECT_NAME} ${QT_LIBS})

# 引入 ElaWidgetTools
include(../cmake/ela.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE ElaWidgetTools)

# 链接 Mel 库
target_link_libraries(${PROJECT_NAME} PRIVATE Mel)

# 目前看来generate_export_header会自动处理静态/动态库的宏定义,不用额外定义 MEL_STATIC_DEFINE
# target_compile_definitions(${PROJECT_NAME} PRIVATE MEL_STATIC_DEFINE)

# Windows 平台特定配置（用于拷贝运行时文件）
set(QT_RUNTIME_LIBS Core Gui Widgets)
# MinGW 需要额外的运行时库
set(QT_PATH_LIBS libwinpthread-1 libstdc++-6 libgcc_s_seh-1)
set(QT_PLUGINS
        styles/qwindowsvistastyle
        platforms/qwindows
        imageformats/qjpeg
)

if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/win_run.cmake)
    # 只拷贝 Mel 库（不需要 ElaWidgetTools）
    copy_target(${PROJECT_NAME} Mel)

    include(${CMAKE_SOURCE_DIR}/cmake/qt_win_run.cmake)
    copy_qt_libs(${PROJECT_NAME} ${QT_RUNTIME_LIBS})
    if (MINGW)
        copy_qt_path_libs(${PROJECT_NAME} ${QT_PATH_LIBS})
    endif()
    copy_qt_plugins(${PROJECT_NAME} ${QT_PLUGINS})
    
    # 引入安装函数
    include(${CMAKE_SOURCE_DIR}/cmake/win_install.cmake)
    include(${CMAKE_SOURCE_DIR}/cmake/qt_win_install.cmake)
endif()

# 安装示例可执行文件
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

#if(WIN32)
#    install_target(Mel ElaWidgetTools)
#    install_qt_libs(${QT_LIBS})
#    if(MINGW)
#        install_qt_path_libs(${QT_PATH_LIBS})
#    endif()
#    install_qt_plugins(${QT_PLUGINS})
#endif()

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
