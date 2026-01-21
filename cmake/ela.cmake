set(lib_name "ElaWidgetTools")

set(${lib_name}_GIT_TAG  main)
set(${lib_name}_GIT_URL  https://github.com/Liniyous/ElaWidgetTools.git)

include(FetchContent)

# 保存原始变量，避免被子项目覆盖
set(_SAVED_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(_SAVED_BUILD_EXAMPLES ${BUILD_EXAMPLES})

#set(BUILD_EXAMPLES OFF CACHE BOOL "Disable ElaWidgetTools examples" FORCE)

FetchContent_Declare(
        ${lib_name}
        GIT_REPOSITORY    ${${lib_name}_GIT_URL}
        GIT_TAG           ${${lib_name}_GIT_TAG}
)

FetchContent_MakeAvailable(${lib_name})

#set(CMAKE_INSTALL_PREFIX ${_SAVED_CMAKE_INSTALL_PREFIX} CACHE PATH "Installation path" FORCE)
#unset(_SAVED_CMAKE_INSTALL_PREFIX)

# MinGW编译支持
if(WIN32 AND MINGW AND TARGET ${lib_name})
    target_compile_definitions(${lib_name} PRIVATE
        SM_CXPADDEDBORDER=92
        USER_DEFAULT_SCREEN_DPI=96
    )
endif()
