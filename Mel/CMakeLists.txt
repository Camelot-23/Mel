project(Mel VERSION 1.0.1)

# 将项目名称转换为大写，用于宏定义
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

# 构建选项
option(${PROJECT_NAME_UPPER}_BUILD_SHARED "Build ${PROJECT_NAME} as a shared library" ON)

# 当使用此库为静态库时定义 ${PROJECT_NAME_UPPER}_STATIC_DEFINE
# target_compile_definitions(MyApp PRIVATE ${PROJECT_NAME_UPPER}_STATIC_DEFINE)

# 生成版本头文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_version.h
    @ONLY
)

# 扫描 ${PROJECT_NAME} 库源文件
file(GLOB_RECURSE ${PROJECT_NAME}_srcs CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${${PROJECT_NAME}_srcs})

# 创建 ${PROJECT_NAME} 库
if(${PROJECT_NAME_UPPER}_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_srcs})
    message(STATUS "${PROJECT_NAME} 将编译为动态库")
else()
    add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_srcs})
    message(STATUS "${PROJECT_NAME} 将编译为静态库")
endif()

# 设置符号可见性（默认隐藏，只导出标记的符号）
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_VISIBILITY_PRESET hidden           # 默认隐藏所有符号
    VISIBILITY_INLINES_HIDDEN ON           # 隐藏内联函数
    C_VISIBILITY_PRESET hidden             # C 符号也隐藏
)

# 生成导出宏头文件
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    BASE_NAME ${PROJECT_NAME_UPPER}
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h
)

# 设置 ${PROJECT_NAME} 库的包含目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}  # 包含生成的导出头文件
)

set(QT_LIBS Widgets)
set_qt_libs(${PROJECT_NAME} ${QT_LIBS})

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
elseif(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装 ${PROJECT_NAME} 库
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin    # Windows DLL
    LIBRARY DESTINATION lib    # Linux .so
    ARCHIVE DESTINATION lib    # Windows .lib / Linux .a
)

# 安装 ${PROJECT_NAME} 库的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

# 安装生成的导出头文件和版本头文件
install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_version.h
    DESTINATION include/${PROJECT_NAME}
)

message(STATUS "${PROJECT_NAME} 库配置完成")
