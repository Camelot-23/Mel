/**
 * @file mainwindow.cpp
 * @brief 主窗口实现 - 使用 BackgroundWidget 封装背景功能
 */

#include "mainwindow.h"
#include "Mel.h"
#include <QMessageBox>
#include <QDebug>
#include <QFont>
#include <QStackedLayout>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _backgroundWidget(nullptr)
    , _contentWidget(nullptr)
    , _mainLayout(nullptr)
    , _titleLabel(nullptr)
    , _descLabel(nullptr)
    , _wallpaperCombo(nullptr)
    , _scaleModeCombo(nullptr)
    , _themeButton(nullptr)
    , _colorButton(nullptr)
    , _clearButton(nullptr)
    , _infoButton(nullptr)
    , _currentWallpaperIndex(0)
    , _isDarkTheme(false)
{
    initWallpapers();
    setupUi();
    
    // 设置初始壁纸
    if (!_wallpapers.isEmpty()) {
        setCurrentWallpaper(_wallpapers[0].path);
    }
}

MainWindow::~MainWindow()
= default;

void MainWindow::initWallpapers()
{
    // 添加所有壁纸资源
    _wallpapers = {
        // 1K 壁纸
        {":/Mel/res/wallpaper/1k/supernova.jpg", 
         "超新星 (Supernova)", "1K"},
        
        // 2K 壁纸
        {":/Mel/res/wallpaper/2k/mountain&lake.png", 
         "山与湖 (Mountain & Lake)", "2K"},
        {":/Mel/res/wallpaper/2k/Sunshine&Maple_Forest.jpg", 
         "阳光枫林 (Sunshine & Maple Forest)", "2K"},
        
        // 4K 壁纸
        {":/Mel/res/wallpaper/4k/doomsday&power_station.jpg", 
         "末日发电站 (Doomsday & Power Station)", "4K"},
        {":/Mel/res/wallpaper/4k/road&sakuar&inverted_image.png", 
         "樱花之路 (Road & Sakura)", "4K"},
        {":/Mel/res/wallpaper/4k/room&window.png", 
         "房间窗景 (Room & Window)", "4K"}
    };
}

void MainWindow::setupUi()
{
    // 设置窗口属性
    setWindowTitle("Mel - 壁纸切换演示 (Qt Native + BackgroundWidget)");
    resize(1200, 800);
    // setMinimumSize(800, 600);
    
    // ========== 创建中心Widget和主布局 ==========
    auto* centralWidget = new QWidget(this);
    auto* mainHBoxLayout = new QHBoxLayout(centralWidget);
    mainHBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainHBoxLayout->setSpacing(0);
    
    // ========== 左侧：背景预览区域 ==========
    _backgroundWidget = new BackgroundWidget(centralWidget);
    _backgroundWidget->setScaleMode(ScaleMode_Fill);  // 默认填满模式
    _backgroundWidget->setTransitionDuration(500);     // 设置背景切换动画时长为500毫秒
    _backgroundWidget->setMinimumSize(600, 400);       // 设置最小尺寸
    
    // ========== 右侧：控制面板 ==========
    _contentWidget = new QWidget(centralWidget);
    _contentWidget->setMinimumWidth(400);
    _contentWidget->setMaximumWidth(500);
    _contentWidget->setStyleSheet(
        "QWidget {"
        "   background-color: #f5f5f5;"
        "}"
    );
    
    // 添加到主布局
    mainHBoxLayout->addWidget(_backgroundWidget, 3);  // 左侧占3份
    mainHBoxLayout->addWidget(_contentWidget, 1);     // 右侧占1份
    
    setCentralWidget(centralWidget);
    
    // ========== 控制面板布局 ==========
    _mainLayout = new QVBoxLayout(_contentWidget);
    _mainLayout->setContentsMargins(30, 30, 30, 30);
    _mainLayout->setSpacing(20);
    
    // ========== 标题 ==========
    _titleLabel = new QLabel("🎨 Mel 壁纸切换", _contentWidget);
    _titleLabel->setAlignment(Qt::AlignCenter);
    
    QFont titleFont = _titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    _titleLabel->setFont(titleFont);
    
    // 设置标题样式
    _titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #333333;"
        "   background-color: white;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   border: 2px solid #e0e0e0;"
        "}"
    );
    
    _mainLayout->addWidget(_titleLabel);
    
    // ========== 描述信息 ==========
    const QString melVersion = Mel::MelLib::getVersion();
    const QString descText = QString(
        "Mel 库版本: %1\n"
        "主版本: %2  次版本: %3  补丁: %4\n"
        "选择下方的壁纸以切换窗口背景"
    ).arg(melVersion)
     .arg(Mel::MelLib::getVersionMajor())
     .arg(Mel::MelLib::getVersionMinor())
     .arg(Mel::MelLib::getVersionPatch());
    
    _descLabel = new QLabel(descText, _contentWidget);
    _descLabel->setAlignment(Qt::AlignCenter);
    _descLabel->setWordWrap(true);
    
    QFont descFont = _descLabel->font();
    descFont.setPointSize(10);
    _descLabel->setFont(descFont);
    
    _descLabel->setStyleSheet(
        "QLabel {"
        "   color: #666666;"
        "   background-color: white;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   border: 1px solid #e0e0e0;"
        "}"
    );
    
    _mainLayout->addWidget(_descLabel);
    
    _mainLayout->addSpacing(10);
    
    // ========== 壁纸选择区域 ==========
    auto* wallpaperLabel = new QLabel("选择壁纸：", _contentWidget);
    wallpaperLabel->setStyleSheet("QLabel { color: #333333; font-weight: bold; font-size: 12px; }");
    _mainLayout->addWidget(wallpaperLabel);
    
    _wallpaperCombo = new QComboBox(_contentWidget);
    _wallpaperCombo->setMinimumHeight(40);
    
    // 设置下拉框样式
    _wallpaperCombo->setStyleSheet(
        "QComboBox {"
        "   background-color: white;"
        "   border: 2px solid #4a90e2;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-size: 13px;"
        "   color: #333333;"
        "}"
        "QComboBox:hover {"
        "   border: 2px solid #357abd;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "   image: none;"
        "   border-left: 5px solid transparent;"
        "   border-right: 5px solid transparent;"
        "   border-top: 8px solid #666666;"
        "   margin-right: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: white;"
        "   border: 2px solid #4a90e2;"
        "   selection-background-color: #e3f2fd;"
        "   selection-color: #333333;"
        "   outline: none;"
        "}"
    );
    
    // 填充下拉框选项
    for (const auto& wallpaper : _wallpapers) {
        QString itemText = QString("%1 [%2]")
            .arg(wallpaper.displayName)
            .arg(wallpaper.resolution);
        _wallpaperCombo->addItem(itemText);
    }
    
    // 连接壁纸信号
    connect(_wallpaperCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onWallpaperChanged);
    
    _mainLayout->addWidget(_wallpaperCombo);
    
    _mainLayout->addSpacing(15);
    
    // ========== 缩放模式选择区域 ==========
    auto* scaleModeLabel = new QLabel("缩放模式：", _contentWidget);
    scaleModeLabel->setStyleSheet("QLabel { color: #333333; font-weight: bold; font-size: 12px; }");
    _mainLayout->addWidget(scaleModeLabel);
    
    _scaleModeCombo = new QComboBox(_contentWidget);
    _scaleModeCombo->setMinimumHeight(40);
    _scaleModeCombo->addItem("填满窗口（裁剪）", ScaleMode_Fill);
    _scaleModeCombo->addItem("适应窗口（留空）", ScaleMode_Fit);
    _scaleModeCombo->addItem("拉伸填充（变形）", ScaleMode_Stretch);
    _scaleModeCombo->setCurrentIndex(0);
    
    // 设置缩放模式下拉框样式
    _scaleModeCombo->setStyleSheet(_wallpaperCombo->styleSheet());
    
    // 连接缩放模式信号
    connect(_scaleModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onScaleModeChanged);
    
    _mainLayout->addWidget(_scaleModeCombo);
    
    _mainLayout->addStretch();
    
    // ========== 按钮区域 ==========
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #4a90e2;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2a5f8f;"
        "}";
    
    // 主题切换按钮
    _themeButton = new QPushButton("🌙 深色主题", _contentWidget);
    _themeButton->setMinimumHeight(45);
    _themeButton->setStyleSheet(buttonStyle);
    connect(_themeButton, &QPushButton::clicked, this, &MainWindow::onThemeToggled);
    _mainLayout->addWidget(_themeButton);
    
    // 背景色选择按钮
    _colorButton = new QPushButton("🎨 选择背景色", _contentWidget);
    _colorButton->setMinimumHeight(45);
    _colorButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #9b59b6;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #8e44ad;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #7d3c98;"
        "}"
    );
    connect(_colorButton, &QPushButton::clicked, this, &MainWindow::onSelectBackgroundColor);
    _mainLayout->addWidget(_colorButton);
    
    // 清除背景按钮
    _clearButton = new QPushButton("🗑️ 清除背景", _contentWidget);
    _clearButton->setMinimumHeight(45);
    _clearButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c0392b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #a93226;"
        "}"
    );
    connect(_clearButton, &QPushButton::clicked, this, &MainWindow::onClearBackground);
    _mainLayout->addWidget(_clearButton);
    
    // 信息按钮
    _infoButton = new QPushButton("ℹ️ 关于", _contentWidget);
    _infoButton->setMinimumHeight(45);
    _infoButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1e8449;"
        "}"
    );
    connect(_infoButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, 
            "关于 Mel 壁纸切换",
            QString(
                "Mel 壁纸切换演示\n\n"
                "版本: %1\n"
                "使用 Qt 原生控件实现\n\n"
                "功能:\n"
                "• 6 张精美壁纸 (1K/2K/4K)\n"
                "• 平滑的淡入淡出切换动画\n"
                "• 主题切换支持\n"
                "• 实时背景更新\n\n"
                "资源已嵌入程序，无需外部文件"
            ).arg(Mel::MelLib::getVersion())
        );
    });
    _mainLayout->addWidget(_infoButton);
}

void MainWindow::setCurrentWallpaper(const QString& resourcePath)
{
    // 使用 BackgroundWidget 设置背景图片
    if (!_backgroundWidget->setBackgroundImage(resourcePath)) {
        qWarning() << "MainWindow: 无法设置壁纸:" << resourcePath;
        return;
    }
    
    qDebug() << "MainWindow: 壁纸设置成功:" << resourcePath;
}

void MainWindow::onWallpaperChanged(int index)
{
    if (index >= 0 && index < _wallpapers.size()) {
        _currentWallpaperIndex = index;
        setCurrentWallpaper(_wallpapers[index].path);
        
        qDebug() << "MainWindow: 切换到壁纸:" << _wallpapers[index].displayName;
    }
}

void MainWindow::onScaleModeChanged(int index)
{
    const BackgroundScaleMode mode = static_cast<BackgroundScaleMode>(
        _scaleModeCombo->itemData(index).toInt()
    );
    
    _backgroundWidget->setScaleMode(mode);
    
    qDebug() << "MainWindow: 缩放模式已切换";
}

void MainWindow::onThemeToggled()
{
    _isDarkTheme = !_isDarkTheme;
    
    if (_isDarkTheme) {
        _themeButton->setText("☀️ 浅色主题");
        
        // 深色主题：添加半透明黑色遮罩
        _backgroundWidget->setOverlayColor(QColor(0, 0, 0, 80));
        
        qDebug() << "MainWindow: 切换到深色主题";
    } else {
        _themeButton->setText("🌙 深色主题");
        
        // 浅色主题：清除遮罩
        _backgroundWidget->clearOverlay();
        
        qDebug() << "MainWindow: 切换到浅色主题";
    }
}

void MainWindow::onClearBackground()
{
    // 清除背景图片
    _backgroundWidget->clearBackground();
    
    // 重置壁纸选择框为无选择状态
    _wallpaperCombo->setCurrentIndex(-1);
    
    qDebug() << "MainWindow: 背景图片已清除";
}

void MainWindow::onSelectBackgroundColor()
{
    // 获取当前背景色
    QColor currentColor = _backgroundWidget->getBackgroundColor();
    if (!currentColor.isValid()) {
        currentColor = Qt::white;  // 默认白色
    }
    
    // 打开颜色选择对话框
    QColor color = QColorDialog::getColor(
        currentColor,
        this,
        "选择背景色",
        QColorDialog::ShowAlphaChannel  // 支持透明度
    );
    
    // 如果用户选择了颜色
    if (color.isValid()) {
        _backgroundWidget->setBackgroundColor(color);
        qDebug() << "MainWindow: 背景色已设置为:" << color.name() << "透明度:" << color.alpha();
    }
}
