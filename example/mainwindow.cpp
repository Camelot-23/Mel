/**
 * @file mainwindow.cpp
 * @brief 主窗口实现 - 使用 BackgroundWidget 封装背景功能
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Mel.h"
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _currentWallpaperIndex(0)
{
    ui->setupUi(this);
    
    initWallpapers();
    initUi();
    
    // 设置初始壁纸
    if (!_wallpapers.isEmpty()) {
        setCurrentWallpaper(_wallpapers[0].path);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::initUi()
{
    // ========== 配置 BackgroundWidget ==========
    ui->backgroundWidget->setScaleMode(Mel::ScaleMode_Fill);  // 默认填满模式
    ui->backgroundWidget->setTransitionDuration(500);          // 设置背景切换动画时长为500毫秒
    
    // ========== 更新描述信息（动态版本信息）==========
    const QString melVersion = Mel::MelLib::getVersion();
    const QString descText = QString(
        "Mel 库版本: %1\n"
        "主版本: %2  次版本: %3  补丁: %4\n"
        "选择下方的壁纸以切换窗口背景"
    ).arg(melVersion)
     .arg(Mel::MelLib::getVersionMajor())
     .arg(Mel::MelLib::getVersionMinor())
     .arg(Mel::MelLib::getVersionPatch());
    
    ui->descLabel->setText(descText);
    
    // ========== 填充壁纸下拉框 ==========
    for (const auto& wallpaper : _wallpapers) {
        QString itemText = QString("%1 [%2]")
            .arg(wallpaper.displayName, wallpaper.resolution);
        ui->wallpaperCombo->addItem(itemText);
    }
    
    // ========== 设置缩放模式下拉框的数据 ==========
    ui->scaleModeCombo->setItemData(0, Mel::ScaleMode_Fill);
    ui->scaleModeCombo->setItemData(1, Mel::ScaleMode_Fit);
    ui->scaleModeCombo->setItemData(2, Mel::ScaleMode_Stretch);
    
    // ========== 连接信号槽 ==========
    connect(ui->wallpaperCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onWallpaperChanged);
    
    connect(ui->scaleModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onScaleModeChanged);
    
    connect(ui->colorButton, &QPushButton::clicked, 
            this, &MainWindow::onSelectBackgroundColor);
    
    connect(ui->clearButton, &QPushButton::clicked, 
            this, &MainWindow::onClearBackground);
    
    connect(ui->infoButton, &QPushButton::clicked, 
            this, &MainWindow::onShowAbout);
}

void MainWindow::setCurrentWallpaper(const QString& resourcePath)
{
    // 使用 BackgroundWidget 设置背景图片
    if (!ui->backgroundWidget->setBackgroundImage(resourcePath)) {
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
    const Mel::BackgroundScaleMode mode = static_cast<Mel::BackgroundScaleMode>(
        ui->scaleModeCombo->itemData(index).toInt()
    );
    
    ui->backgroundWidget->setScaleMode(mode);
    
    qDebug() << "MainWindow: 缩放模式已切换";
}

void MainWindow::onClearBackground()
{
    // 清除背景图片
    ui->backgroundWidget->clearBackground();
    
    // 重置壁纸选择框为无选择状态
    ui->wallpaperCombo->setCurrentIndex(-1);
    
    qDebug() << "MainWindow: 背景图片已清除";
}

void MainWindow::onSelectBackgroundColor()
{
    // 获取当前背景色
    QColor currentColor = ui->backgroundWidget->getBackgroundColor();
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
        ui->backgroundWidget->setBackgroundColor(color);
        qDebug() << "MainWindow: 背景色已设置为:" << color.name() << "透明度:" << color.alpha();
    } else {
        // 用户点击了取消，清除背景色
        ui->backgroundWidget->setBackgroundColor(QColor());
        qDebug() << "MainWindow: 已清除背景色";
    }
}

void MainWindow::onShowAbout()
{
    QMessageBox::information(this, 
        "关于 Mel 壁纸切换",
        QString(
            "Mel 壁纸切换演示\n\n"
            "版本: %1\n"
            "使用 Qt 原生控件实现\n\n"
            "功能:\n"
            "• 6 张精美壁纸 (1K/2K/4K)\n"
            "• 平滑的淡入淡出切换动画\n"
            "• 背景色选择功能\n"
            "• 实时背景更新\n\n"
            "资源已嵌入程序，无需外部文件"
        ).arg(Mel::MelLib::getVersion())
    );
}
