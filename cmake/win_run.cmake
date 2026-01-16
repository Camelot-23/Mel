# 拷贝外部库文件到目标二进制目录
# 
# 参数:
#   target - 目标名称
#   ARGN   - 一个或多个库文件的绝对路径
# 
# 使用场景:
#   - 拷贝第三方库的DLL文件
#   - 拷贝不由CMake管理的外部依赖
#   - 需要明确指定文件路径
# 
# 示例:
#   copy_lib(${PROJECT_NAME}
#       "C:/ThirdParty/opencv/bin/opencv_world455.dll"
#       "D:/libs/ffmpeg/bin/avcodec-59.dll"
#   )
function(copy_lib target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    foreach(LIB_PATH IN LISTS ARGN)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "COPY LIB: ${LIB_PATH}"
                COMMAND ${CMAKE_COMMAND} -E copy "${LIB_PATH}" "$<TARGET_FILE_DIR:${target}>")
    endforeach (LIB_PATH)
endfunction()

# 拷贝CMake目标到目标二进制目录
# 
# 参数:
#   target - 目标名称
#   ARGN   - 一个或多个CMake目标名称
# 
# 使用场景:
#   - 拷贝项目内构建的动态库
#   - 拷贝通过FetchContent引入的库
#   - 自动处理Debug/Release配置差异
# 
# 示例:
#   copy_target(${PROJECT_NAME}
#       ElaWidgetTools
#       MyCustomLibrary
#   )
function(copy_target target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()
    foreach(TARGET IN LISTS ARGN)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "COPY TARGET LIB: $<TARGET_FILE:${TARGET}>"
                COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${TARGET}>" "$<TARGET_FILE_DIR:${target}>")
    endforeach (TARGET)
endfunction()
