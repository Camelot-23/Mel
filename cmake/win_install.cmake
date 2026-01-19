# Windows 安装辅助函数
# 用于在 install 阶段拷贝库文件到安装目录

# 安装外部库文件到安装目录
# 
# 参数:
#   ARGN - 一个或多个库文件的绝对路径
# 
# 使用场景:
#   - 安装第三方库的DLL文件
#   - 安装不由CMake管理的外部依赖
#   - 需要明确指定文件路径
# 
# 示例:
#   install_lib(
#       "C:/ThirdParty/opencv/bin/opencv_world455.dll"
#       "D:/libs/ffmpeg/bin/avcodec-59.dll"
#   )
function(install_lib)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    set(LIB_FILES)
    foreach(LIB_PATH IN LISTS ARGN)
        if(EXISTS "${LIB_PATH}")
            list(APPEND LIB_FILES "${LIB_PATH}")
        else()
            message(WARNING "库文件不存在: ${LIB_PATH}")
        endif()
    endforeach()
    
    if(LIB_FILES)
        install(FILES ${LIB_FILES} DESTINATION bin)
    endif()
endfunction()

# 安装CMake目标到安装目录
# 
# 参数:
#   ARGN - 一个或多个CMake目标名称
# 
# 使用场景:
#   - 安装项目内构建的动态库
#   - 安装通过FetchContent引入的库
#   - 自动处理Debug/Release配置差异
# 
# 注意:
#   - 使用生成器表达式 $<TARGET_FILE:target> 自动获取目标文件
#   - 支持不同构建类型（Debug/Release）自动选择正确的库文件
# 
# 示例:
#   install_target(
#       ElaWidgetTools
#       MyCustomLibrary
#   )
function(install_target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    
    foreach(TARGET IN LISTS ARGN)
        if(TARGET ${TARGET})
            # 获取目标类型
            get_target_property(TARGET_TYPE ${TARGET} TYPE)
            
            # 只处理动态库和可执行文件
            if(TARGET_TYPE STREQUAL "SHARED_LIBRARY" OR TARGET_TYPE STREQUAL "MODULE_LIBRARY")
                install(FILES $<TARGET_FILE:${TARGET}> DESTINATION bin)
            elseif(TARGET_TYPE STREQUAL "EXECUTABLE")
                # 可执行文件通常使用 install(TARGETS ...) 更合适
                # 但这里也支持，以保持接口一致
                install(FILES $<TARGET_FILE:${TARGET}> DESTINATION bin)
            else()
                message(WARNING "目标 ${TARGET} 的类型 ${TARGET_TYPE} 不支持安装")
            endif()
        else()
            message(WARNING "CMake目标不存在: ${TARGET}")
        endif()
    endforeach()
endfunction()
