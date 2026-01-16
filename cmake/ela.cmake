set(lib_name "ElaWidgetTools")

set(${lib_name}_GIT_TAG  main)
set(${lib_name}_GIT_URL  https://github.com/Liniyous/ElaWidgetTools.git)

include(FetchContent)

FetchContent_Declare(
        ${lib_name}
        GIT_REPOSITORY    ${${lib_name}_GIT_URL}
        GIT_TAG           ${${lib_name}_GIT_TAG}
)


# SET(QT_SDK_DIR C:/Qt/5.15.2/mingw81_64 CACHE PATH "QT SDK DIR" FORCE)
FetchContent_MakeAvailable(${lib_name})

# set_target_properties(${lib_name} PROPERTIES AUTORCC ON AUTOMOC ON AUTOUIC ON)

# MinGW编译支持
if(WIN32 AND MINGW AND TARGET ${lib_name})
    target_compile_definitions(${lib_name} PRIVATE
        SM_CXPADDEDBORDER=92
        USER_DEFAULT_SCREEN_DPI=96
    )
endif()
