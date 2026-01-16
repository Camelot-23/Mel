# Qt Hello World

一个基于 CMake 的跨 Qt 版本（Qt 4/5/6）Hello World 示例项目。

## 特性

- 支持 Qt 4、Qt 5 和 Qt 6
- 使用 CMake 构建系统
- 自动检测已安装的 Qt 版本
- 简洁的现代 UI 界面

## 项目结构

```
.
├── CMakeLists.txt      # CMake 构建配置
├── README.md           # 项目说明文档
├── LICENSE             # 许可证
└── src/
    ├── main.cpp        # 程序入口
    ├── mainwindow.h    # 主窗口头文件
    └── mainwindow.cpp  # 主窗口实现
```

## 依赖要求

- CMake 3.1 或更高版本
- Qt 4.x、Qt 5.x 或 Qt 6.x（任选其一）
- C++11 兼容的编译器

## 构建说明

### 自动检测 Qt 版本

如果您的系统中安装了 Qt，CMake 会自动检测并使用可用的 Qt 版本（优先级：Qt6 > Qt5 > Qt4）。

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目（自动检测 Qt 版本）
cmake ..

# 构建项目
cmake --build .

# 运行程序
./QtHelloWorld
```

### 指定 Qt 版本

您可以通过 `-DQT_VERSION` 参数指定要使用的 Qt 版本：

```bash
# 使用 Qt6
cmake -DQT_VERSION=6 ..

# 使用 Qt5
cmake -DQT_VERSION=5 ..

# 使用 Qt4
cmake -DQT_VERSION=4 ..
```

### 指定 Qt 安装路径

如果 Qt 不在标准路径中，可以通过 `CMAKE_PREFIX_PATH` 指定：

```bash
# Qt6 示例
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 ..

# Qt5 示例
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/5.x.x/gcc_64 ..
```

## 跨平台构建

### Linux

```bash
mkdir build && cd build
cmake ..
make
./QtHelloWorld
```

### macOS

```bash
mkdir build && cd build
cmake ..
make
./QtHelloWorld
# 或者打开生成的 .app bundle
open QtHelloWorld.app
```

### Windows (Visual Studio)

```batch
mkdir build && cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
Release\QtHelloWorld.exe
```

### Windows (MinGW)

```batch
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
QtHelloWorld.exe
```

## Qt 版本兼容性说明

本项目使用了以下技术来确保跨 Qt 版本兼容性：

1. **预处理器宏**：使用 `QT_VERSION` 和 `QT_VERSION_CHECK` 处理 API 差异
2. **信号槽连接**：使用 `SIGNAL()` 和 `SLOT()` 宏以兼容旧版本
3. **CMake 配置**：根据检测到的 Qt 版本动态设置编译选项

### Qt4 特定注意事项

- Qt4 使用 `QtGui` 模块而不是 `Widgets` 模块
- 某些布局方法有所不同（如 `setMargin` vs `setContentsMargins`）

### Qt5/Qt6 差异

- Qt6 中某些 API 已被移除或更改
- 使用 `setContentsMargins` 替代已废弃的 `setMargin`

## 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。
