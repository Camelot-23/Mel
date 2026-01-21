# Mel Example - 壁纸切换演示 (Qt Native)

## 功能介绍

这是一个基于 **Qt 原生控件** 和 **Mel** 库的示例应用程序，完全使用 Qt 标准组件实现，展示了以下功能：

### ✨ 主要特性

1. **动态背景切换**
   - 支持从下拉框中选择不同的壁纸
   - 平滑的淡入淡出切换动画（500毫秒）
   - 实时切换窗口背景图片
   - 包含 6 张精美壁纸（1K/2K/4K）

2. **主题支持**
   - 支持浅色/深色主题切换
   - 深色主题添加半透明黑色遮罩
   - 主题切换时保持当前选择的壁纸

3. **现代化 UI**
   - 使用 QMainWindow 作为窗口基类
   - 自定义 paintEvent 绘制背景
   - 半透明的控制面板
   - 美观的按钮和下拉框样式

## 📦 可用壁纸

### 1K 分辨率
- 🌌 超新星 (Supernova)

### 2K 分辨率
- 🏔️ 山与湖 (Mountain & Lake)
- 🍁 阳光枫林 (Sunshine & Maple Forest)

### 4K 分辨率
- 🏭 末日发电站 (Doomsday & Power Station)
- 🌸 樱花之路 (Road & Sakura)
- 🪟 房间窗景 (Room & Window)

## 🎮 使用方法

### 启动应用

```bash
# 配置项目
cmake -B build

# 编译
cmake --build build

# 运行（Windows）
./build/example/Debug/Mel_Example.exe

# 或从安装目录运行
./Install/bin/Mel_Example.exe
```

### 操作说明

1. **切换壁纸**
   - 点击窗口中央的下拉框
   - 选择你喜欢的壁纸
   - 背景会以平滑的淡入淡出效果切换

2. **切换主题**
   - 点击窗口右上角的主题按钮（月亮/太阳图标）
   - 享受流畅的主题切换动画
   - 壁纸会保持不变

3. **窗口操作**
   - 最小化、最大化、关闭按钮在右上角
   - 支持拖拽移动窗口
   - 支持调整窗口大小

## 🔧 代码结构

```
example/
├── main.cpp                    # 应用程序入口
├── mainwindow.h                # 主窗口类声明
├── mainwindow.cpp              # 主窗口实现
├── backgroundwidget.h          # 背景控件类声明（支持动画）
├── backgroundwidget.cpp        # 背景控件实现
├── BACKGROUNDWIDGET_USAGE.md   # BackgroundWidget 使用指南
├── CMakeLists.txt              # CMake 配置
└── README.md                   # 本文档
```

### 核心代码

#### MainWindow 类

```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;   // 绘制背景
    void resizeEvent(QResizeEvent *event) override; // 处理窗口大小变化

private slots:
    void onWallpaperChanged(int index);  // 壁纸切换槽函数
    void onThemeToggled();               // 主题切换槽函数

private:
    void setupUi();                      // 初始化 UI
    void initWallpapers();               // 初始化壁纸列表
    void setCurrentWallpaper(const QString&);  // 设置当前壁纸
    void updateScaledPixmap();           // 更新缩放后的图片
    
    // UI 组件（Qt 原生控件）
    QComboBox* _wallpaperCombo;          // 壁纸选择下拉框
    QLabel* _titleLabel;                 // 标题标签
    QLabel* _descLabel;                  // 描述标签
    QPushButton* _themeButton;           // 主题切换按钮
    QPushButton* _infoButton;            // 信息按钮
    
    // 壁纸数据
    QPixmap _currentWallpaper;           // 当前壁纸原图
    QPixmap _scaledWallpaper;            // 缩放后的壁纸
    QList<WallpaperInfo> _wallpapers;    // 壁纸列表
    bool _isDarkTheme;                   // 是否深色主题
};
```

#### 关键功能实现

**1. 绘制背景（核心）**

```cpp
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    if (!_scaledWallpaper.isNull()) {
        // 绘制背景图片（居中）
        int x = (width() - _scaledWallpaper.width()) / 2;
        int y = (height() - _scaledWallpaper.height()) / 2;
        painter.drawPixmap(x, y, _scaledWallpaper);
        
        // 深色主题添加半透明遮罩
        if (_isDarkTheme) {
            painter.fillRect(rect(), QColor(0, 0, 0, 80));
        }
    }
    
    QMainWindow::paintEvent(event);
}
```

**2. 设置壁纸**

```cpp
void MainWindow::setCurrentWallpaper(const QString& resourcePath)
{
    _currentWallpaper.load(resourcePath);
    
    // 缩放图片以适应窗口
    _scaledWallpaper = _currentWallpaper.scaled(
        size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
    );
    
    update(); // 触发重绘
}
```

**3. 切换壁纸**

```cpp
void MainWindow::onWallpaperChanged(int index)
{
    if (index >= 0 && index < _wallpapers.size()) {
        _currentWallpaperIndex = index;
        setCurrentWallpaper(_wallpapers[index].path);
    }
}
```

**4. 响应窗口大小变化**

```cpp
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    
    // 窗口大小改变时重新缩放壁纸
    updateScaledPixmap();
}
```

**5. 主题切换**

```cpp
void MainWindow::onThemeToggled()
{
    _isDarkTheme = !_isDarkTheme;
    
    if (_isDarkTheme) {
        _themeButton->setText("☀️ 切换到浅色主题");
    } else {
        _themeButton->setText("🌙 切换到深色主题");
    }
    
    update(); // 重绘应用主题遮罩
}
```

## 📚 依赖库

### Qt 原生组件
- **QMainWindow**: 主窗口基类
- **QComboBox**: 下拉框组件
- **QLabel**: 标签组件
- **QPushButton**: 按钮组件
- **QPixmap**: 图片加载和处理
- **QPainter**: 自定义绘制
- **QLayout**: 布局管理（QVBoxLayout, QHBoxLayout）

### Mel
- **Mel::MelLib**: 版本信息管理
- **资源系统**: 内嵌的壁纸资源（通过 Qt 资源系统）

**优势：**
- ✅ 无需额外第三方库
- ✅ 跨平台兼容性好
- ✅ 代码简洁易懂
- ✅ 完全可控的 UI 样式

## 🎨 自定义扩展

### 添加新壁纸

1. 将图片放入 `Mel/res/wallpaper/` 目录
2. 在 `Mel/Mel.qrc` 中注册资源
3. 在 `initWallpapers()` 中添加条目：

```cpp
void MainWindow::initWallpapers()
{
    _wallpapers = {
        // ... 现有壁纸 ...
        
        // 添加新壁纸
        {":/Mel/res/wallpaper/your_folder/your_image.jpg",
         "你的壁纸名称", "分辨率"},
    };
}
```

### 为不同主题设置不同壁纸

修改 `setCurrentWallpaper` 方法，根据主题加载不同的壁纸：

```cpp
void MainWindow::setCurrentWallpaper(int index)
{
    QString path = _isDarkTheme 
        ? _wallpapers[index].darkPath 
        : _wallpapers[index].lightPath;
    
    _currentWallpaper.load(path);
    updateScaledPixmap();
    update();
}
```

### 自定义 UI 样式

所有控件都使用 `setStyleSheet()` 设置样式：

```cpp
// 自定义按钮样式
QPushButton* myButton = new QPushButton("我的按钮", this);
myButton->setStyleSheet(
    "QPushButton {"
    "   background-color: rgba(255, 100, 100, 200);"
    "   color: white;"
    "   border-radius: 8px;"
    "   padding: 10px;"
    "}"
    "QPushButton:hover {"
    "   background-color: rgba(255, 100, 100, 255);"
    "}"
);
_mainLayout->addWidget(myButton);

// 自定义标签样式
QLabel* myLabel = new QLabel("我的标签", this);
myLabel->setStyleSheet(
    "QLabel {"
    "   background-color: rgba(255, 255, 255, 180);"
    "   border: 2px solid blue;"
    "   border-radius: 5px;"
    "   padding: 8px;"
    "}"
);
_mainLayout->addWidget(myLabel);
```

### 添加动画效果

使用 QPropertyAnimation 为控件添加动画：

```cpp
#include <QPropertyAnimation>

void MainWindow::animateButton()
{
    QPropertyAnimation* animation = new QPropertyAnimation(_themeButton, "geometry");
    animation->setDuration(500);
    animation->setStartValue(_themeButton->geometry());
    animation->setEndValue(QRect(100, 100, 200, 50));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
```

## 💡 技巧与提示

1. **壁纸资源都在编译时嵌入到程序中**，不需要额外的文件
2. **使用 `:/Mel/` 前缀访问资源**，这是 Qt 资源系统的标准用法
3. **使用 `Qt::KeepAspectRatioByExpanding` 缩放模式**，确保壁纸填满整个窗口
4. **在 `resizeEvent` 中更新缩放后的图片**，确保窗口大小变化时壁纸正确显示
5. **使用 `rgba()` 颜色实现半透明效果**，增强文字的可读性
6. **样式表支持 `:hover` 和 `:pressed` 伪状态**，创造更好的交互体验
7. **paintEvent 中先绘制背景，再调用父类方法**，确保控件正常显示

## 🐛 常见问题

### Q: 壁纸显示不出来？
A: 检查：
- 资源文件是否正确编译（查看生成的 `qrc_Mel.cpp`）
- 资源路径是否正确（必须以 `:/Mel/` 开头）
- 图片文件是否损坏
- `paintEvent` 是否被正确调用

### Q: 下拉框选择后没反应？
A: 检查：
- 信号槽连接是否正确
- `onWallpaperChanged` 槽函数是否被调用
- 控制台是否有错误输出
- `update()` 是否被调用以触发重绘

### Q: 窗口大小改变后壁纸拉伸变形？
A: 确保在 `resizeEvent` 中调用 `updateScaledPixmap()` 重新缩放图片

### Q: 控件无法显示或被背景覆盖？
A: 确保 `paintEvent` 最后调用 `QMainWindow::paintEvent(event)`，让子控件能够正常绘制

### Q: 样式表不生效？
A: 检查：
- CSS 语法是否正确
- 选择器是否匹配
- 是否有其他样式覆盖

## 📄 许可证

本示例代码遵循 Mel 项目的许可证。

---

**Enjoy coding with Mel! 🎉**
