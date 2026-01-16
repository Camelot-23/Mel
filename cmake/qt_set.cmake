# 初始化 Qt 环境
# 启用 Qt 自动工具并查找 Qt
# 
# 使用场景:
#   - 在项目开始时调用，设置 Qt 全局环境
#   - 必须在任何 Qt 目标创建之前调用
# 
# 示例:
#   init_qt()
macro(init_qt)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
    
    message(STATUS "========== Qt 配置信息 ==========")
    message(STATUS "版本        : ${Qt${QT_VERSION_MAJOR}_VERSION}")
    message(STATUS "Qt目录      : ${QT_DIR}")
    message(STATUS "==================================")
endmacro()

function(set_qt_libs target)
    foreach (MODULE ${ARGN})
        list(APPEND QT_LIBS ${MODULE})
    endforeach ()
    list(APPEND QT_LIBS Core)

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS})
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS})

    foreach (MODULE ${ARGN})
        list(APPEND QT_VERSION_LIBS Qt${QT_VERSION_MAJOR}::${MODULE})
    endforeach ()

    target_link_libraries(${target} PUBLIC ${QT_VERSION_LIBS})
endfunction()
