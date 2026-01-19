set(DEBUG_SUFFIX)
if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DEBUG_SUFFIX "d")
    message(STATUS "DEBUG_SUFFIX: ${DEBUG_SUFFIX}")
endif ()

# 查找qt安装目录
set(QT_INSTALL_PATH_TMP "${QT_DIR}")
# 最大向上迭代次数
set(MAX_ITER 4)
# 初始化迭代次数
set(iter 1)
while(NOT EXISTS "${QT_INSTALL_PATH_TMP}/bin" AND NOT iter GREATER MAX_ITER)
    # message(STATUS "在 ${QT_INSTALL_PATH_TMP} 中未找到 bin 目录，向上查找...")
    # 向上查找一层目录
    get_filename_component(QT_INSTALL_PATH_TMP "${QT_INSTALL_PATH_TMP}" DIRECTORY)
    math(EXPR iter "${iter} + 1")
endwhile()
if (iter GREATER MAX_ITER)
    message(FATAL_ERROR "未找到 Qt 安装目录")
endif ()
set(QT_INSTALL_PATH "${QT_INSTALL_PATH_TMP}")
message(STATUS "QT_INSTALL_PATH: ${QT_INSTALL_PATH}")

# 拷贝Qt核心库DLL到目标二进制目录
# 
# 参数:
#   target - 目标名称
#   ARGN   - 一个或多个Qt模块名称（不带Qt5/Qt6前缀）
# 
# 注意:
#   - 自动检测 Qt 版本（Qt5 或 Qt6）
#   - 需要在调用前设置 QT_VERSION_MAJOR 变量
# 
# 示例:
#   copy_qt_libs(${PROJECT_NAME}
#       Core
#       Gui
#       Widgets
#   )
#   # 结果: Qt5 拷贝 Qt5Core.dll，Qt6 拷贝 Qt6Core.dll
function(copy_qt_libs target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    if(NOT DEFINED QT_VERSION_MAJOR)
        message(FATAL_ERROR "QT_VERSION_MAJOR 未定义，请先调用 init_qt()")
    endif()
    
    add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "TARGET:${target}, FILE:$<TARGET_FILE:${target}>"
    )
    foreach(QT_LIB IN LISTS ARGN)
        set(QT_DLL_NAME "Qt${QT_VERSION_MAJOR}${QT_LIB}${DEBUG_SUFFIX}.dll")
        add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "COPY QT LIB: ${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}"
                "$<TARGET_FILE_DIR:${target}>"
        )
    endforeach()
endfunction()

function(copy_qt_path_libs target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()

    if(NOT DEFINED QT_VERSION_MAJOR)
        message(FATAL_ERROR "QT_VERSION_MAJOR 未定义，请先调用 init_qt()")
    endif()

    add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "TARGET:${target}, FILE:$<TARGET_FILE:${target}>"
    )
    foreach(QT_PATH_LIB IN LISTS ARGN)
        set(QT_DLL_NAME "${QT_PATH_LIB}${DEBUG_SUFFIX}.dll")
        add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "COPY QT PATH LIB: ${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/${QT_DLL_NAME}"
                "$<TARGET_FILE_DIR:${target}>"
        )
    endforeach()
endfunction()

# 拷贝Qt插件库到目标二进制输出目录
# 
# 参数:
#   target - 目标名称
#   ARGN   - 一个或多个Qt插件路径（格式: 类型/插件名）
# 
# 示例:
#   copy_qt_plugins(${PROJECT_NAME}
#       platforms/qwindows
#       styles/qwindowsvistastyle
#       imageformats/qjpeg
#   )
#   # 结果: 
#   #   plugins/platforms/qwindows.dll
#   #   plugins/styles/qwindowsvistastyle.dll
#   #   plugins/imageformats/qjpeg.dll
function(copy_qt_plugins target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    foreach(QT_PLUGIN_LIB IN LISTS ARGN)
        string(FIND "${QT_PLUGIN_LIB}" "/" SLASH_POSITION)
        string(SUBSTRING "${QT_PLUGIN_LIB}" 0 ${SLASH_POSITION} LEFT_PART)

        set(LIB_PATH "${QT_INSTALL_PATH}/plugins/${QT_PLUGIN_LIB}${DEBUG_SUFFIX}.dll")
        set(LIB_OUT_PATH "$<TARGET_FILE_DIR:${target}>/plugins/${LEFT_PART}/")

        if (EXISTS ${LIB_PATH})
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${LIB_OUT_PATH})
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "COPY QT PLUGIN LIB: ${LIB_PATH}"
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${LIB_PATH}
                    ${LIB_OUT_PATH})
        endif ()
    endforeach (QT_PLUGIN_LIB)
endfunction()

