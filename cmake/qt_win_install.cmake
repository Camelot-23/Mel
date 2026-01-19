# Qt Windows 安装辅助函数
# 用于在 install 阶段拷贝 Qt 依赖库到安装目录

# 确定 Debug 后缀
if(NOT DEFINED QT_INSTALL_DEBUG_SUFFIX)
    set(QT_INSTALL_DEBUG_SUFFIX)
    if(MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(QT_INSTALL_DEBUG_SUFFIX "d")
    endif()
endif()

# 查找 Qt 安装目录
if(NOT DEFINED QT_INSTALL_PATH)
    set(QT_INSTALL_PATH_TMP "${QT_DIR}")
    set(MAX_ITER 4)
    set(iter 1)
    
    while(NOT EXISTS "${QT_INSTALL_PATH_TMP}/bin" AND NOT iter GREATER MAX_ITER)
        get_filename_component(QT_INSTALL_PATH_TMP "${QT_INSTALL_PATH_TMP}" DIRECTORY)
        math(EXPR iter "${iter} + 1")
    endwhile()
    
    if(iter GREATER MAX_ITER)
        message(FATAL_ERROR "未找到 Qt 安装目录")
    endif()
    
    set(QT_INSTALL_PATH "${QT_INSTALL_PATH_TMP}")
    message(STATUS "QT_INSTALL_PATH (for install): ${QT_INSTALL_PATH}")
endif()

# 安装Qt核心库DLL到安装目录
# 
# 参数:
#   ARGN - 一个或多个Qt模块名称（不带Qt5/Qt6前缀）
# 
# 示例:
#   install_qt_libs(Core Gui Widgets)
function(install_qt_libs)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    if(NOT DEFINED QT_VERSION_MAJOR)
        message(FATAL_ERROR "QT_VERSION_MAJOR 未定义，请先调用 init_qt()")
    endif()
    
    set(QT_DLL_FILES)
    foreach(QT_LIB IN LISTS ARGN)
        set(QT_DLL_NAME "Qt${QT_VERSION_MAJOR}${QT_LIB}${QT_INSTALL_DEBUG_SUFFIX}.dll")
        set(QT_DLL_PATH "${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}")
        if(EXISTS "${QT_DLL_PATH}")
            list(APPEND QT_DLL_FILES "${QT_DLL_PATH}")
        else()
            message(WARNING "Qt库文件不存在: ${QT_DLL_PATH}")
        endif()
    endforeach()
    
    if(QT_DLL_FILES)
        install(FILES ${QT_DLL_FILES} DESTINATION bin)
        message(STATUS "将安装 ${CMAKE_CURRENT_LIST_LENGTH} 个Qt库文件到 bin 目录")
    endif()
endfunction()

# 安装Qt路径库DLL到安装目录（如MinGW运行时库）
# 
# 参数:
#   ARGN - 一个或多个库名称
# 
# 示例:
#   install_qt_path_libs(libwinpthread-1 libstdc++-6 libgcc_s_seh-1)
function(install_qt_path_libs)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    if(NOT DEFINED QT_VERSION_MAJOR)
        message(FATAL_ERROR "QT_VERSION_MAJOR 未定义，请先调用 init_qt()")
    endif()
    
    set(QT_PATH_DLL_FILES)
    foreach(QT_PATH_LIB IN LISTS ARGN)
        set(QT_DLL_NAME "${QT_PATH_LIB}${QT_INSTALL_DEBUG_SUFFIX}.dll")
        set(QT_DLL_PATH "${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}")
        if(EXISTS "${QT_DLL_PATH}")
            list(APPEND QT_PATH_DLL_FILES "${QT_DLL_PATH}")
        else()
            message(WARNING "Qt路径库文件不存在: ${QT_DLL_PATH}")
        endif()
    endforeach()
    
    if(QT_PATH_DLL_FILES)
        install(FILES ${QT_PATH_DLL_FILES} DESTINATION bin)
        message(STATUS "将安装 ${CMAKE_CURRENT_LIST_LENGTH} 个Qt路径库文件到 bin 目录")
    endif()
endfunction()

# 安装Qt插件库到安装目录
# 
# 参数:
#   ARGN - 一个或多个Qt插件路径（格式: 类型/插件名）
# 
# 示例:
#   install_qt_plugins(
#       platforms/qwindows
#       styles/qwindowsvistastyle
#       imageformats/qjpeg
#   )
function(install_qt_plugins)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    foreach(QT_PLUGIN_LIB IN LISTS ARGN)
        string(FIND "${QT_PLUGIN_LIB}" "/" SLASH_POSITION)
        string(SUBSTRING "${QT_PLUGIN_LIB}" 0 ${SLASH_POSITION} LEFT_PART)
        
        set(LIB_PATH "${QT_INSTALL_PATH}/plugins/${QT_PLUGIN_LIB}${QT_INSTALL_DEBUG_SUFFIX}.dll")
        
        if(EXISTS ${LIB_PATH})
            install(FILES ${LIB_PATH} DESTINATION bin/plugins/${LEFT_PART})
        else()
            message(WARNING "Qt插件文件不存在: ${LIB_PATH}")
        endif()
    endforeach()
endfunction()
