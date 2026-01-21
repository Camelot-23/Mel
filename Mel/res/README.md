# Mel 资源文件使用说明

## 资源文件结构

```
Mel/
├── Mel.qrc              # Qt 资源文件
└── res/
    └── wallpaper/       # 壁纸资源
        ├── 1k/          # 1K 分辨率
        ├── 2k/          # 2K 分辨率
        └── 4k/          # 4K 分辨率
```

## 资源前缀

所有资源使用 `/Mel` 前缀访问。

## 使用方法

### 1. 加载图片资源

```cpp
#include <QPixmap>

// 加载 1K 壁纸
QPixmap pix1k(":/Mel/res/wallpaper/1k/supernova.jpg");

// 加载 2K 壁纸
QPixmap pix2k(":/Mel/res/wallpaper/2k/mountain&lake.png");
QPixmap pix2k2(":/Mel/res/wallpaper/2k/Sunshine&Maple_Forest.jpg");

// 加载 4K 壁纸
QPixmap pix4k(":/Mel/res/wallpaper/4k/room&window.png");
```

### 2. 设置为窗口背景

```cpp
#include <Mel/Mel.h>

// 在 ElaWindow 中设置背景
ElaWindow* window = new ElaWindow();

// 设置亮色主题壁纸
QPixmap lightWallpaper(":/Mel/res/wallpaper/2k/Sunshine&Maple_Forest.jpg");
window->setWindowPixmap(ElaThemeType::Light, lightWallpaper);

// 设置暗色主题壁纸
QPixmap darkWallpaper(":/Mel/res/wallpaper/4k/room&window.png");
window->setWindowPixmap(ElaThemeType::Dark, darkWallpaper);

// 设置绘制模式为图片
window->setWindowPaintMode(ElaWindowType::PaintMode::Pixmap);
```

### 3. 在 QLabel 中显示

```cpp
#include <QLabel>

QLabel* label = new QLabel();
QPixmap wallpaper(":/Mel/res/wallpaper/4k/doomsday&power_station.jpg");
label->setPixmap(wallpaper.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
```

### 4. 检查资源是否存在

```cpp
#include <QFile>

if (QFile::exists(":/Mel/res/wallpaper/1k/supernova.jpg")) {
    qDebug() << "资源文件存在";
} else {
    qDebug() << "资源文件不存在";
}
```

## 可用壁纸列表

### 1K 分辨率
- `:/Mel/res/wallpaper/1k/supernova.jpg`

### 2K 分辨率
- `:/Mel/res/wallpaper/2k/mountain&lake.png`
- `:/Mel/res/wallpaper/2k/Sunshine&Maple_Forest.jpg`

### 4K 分辨率
- `:/Mel/res/wallpaper/4k/doomsday&power_station.jpg`
- `:/Mel/res/wallpaper/4k/road&sakuar&inverted_image.png`
- `:/Mel/res/wallpaper/4k/room&window.png`

## 注意事项

1. **资源路径前缀**：必须使用 `:/Mel/` 前缀
2. **文件名中的 `&`**：在 .qrc 文件中已转义为 `&amp;`，使用时仍用 `&`
3. **大小写敏感**：资源路径区分大小写
4. **自动编译**：CMake 配置了 AUTORCC，修改 .qrc 文件后会自动重新编译

## 添加新资源

1. 将文件放入 `res/` 目录
2. 在 `Mel.qrc` 中添加文件路径
3. 重新编译项目

```xml
<file>res/your_folder/your_file.png</file>
```
