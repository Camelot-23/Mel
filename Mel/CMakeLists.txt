# 扫描 Mel 库源文件
file(GLOB_RECURSE mel_srcs CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${mel_srcs})

# 创建 Mel 库
add_library(Mel STATIC ${mel_srcs})

# 设置 Mel 库的包含目录
target_include_directories(Mel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(QT_LIBS Core Gui Widgets)
set_qt_libs(Mel ${QT_LIBS})

# 设置编译选项
if(MSVC)
    target_compile_options(Mel PRIVATE /W4 /utf-8)
elseif(MINGW)
    target_compile_options(Mel PRIVATE -Wall -Wextra)
else()
    target_compile_options(Mel PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装 Mel 库
install(TARGETS Mel
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装 Mel 库的头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/Mel
    FILES_MATCHING PATTERN "*.h"
)

message(STATUS "Mel 库配置完成")
