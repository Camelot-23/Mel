#include "BackgroundWidgetExample.h"

#include "ElaComboBox.h"
#include "ui_BackgroundWidgetExample.h"

#include <QColorDialog>
#include <QPushButton>

BackgroundWidgetExample::BackgroundWidgetExample(QWidget *parent) :
    QWidget(parent), ui(new Ui::BackgroundWidgetExample) {
    ui->setupUi(this);
    initUi();
}

BackgroundWidgetExample::~BackgroundWidgetExample() {
    delete ui;
}

void BackgroundWidgetExample::initUi() {
    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    backgroundWidget = new Mel::BackgroundWidget(this);
    backgroundWidget->setScaleMode(Mel::ScaleMode_Fill);
    backgroundWidget->setTransitionDuration(500);
    mainLayout->addWidget(backgroundWidget);


    widgetPanel = new QWidget(this);
    widgetPanel->setFixedWidth(200);
    layout = new QVBoxLayout(widgetPanel);
    comboBoxResourcePath = new QComboBox(widgetPanel);
    layout->addWidget(comboBoxResourcePath);

    _wallpapers = {
            // 1K 壁纸
            {":/Mel/res/wallpaper/1k/supernova.jpg", "超新星 (Supernova)", "1K"},

            // 2K 壁纸
            {":/Mel/res/wallpaper/2k/mountain&lake.png", "山与湖 (Mountain & Lake)", "2K"}, {":/Mel/res/wallpaper/2k/Sunshine&Maple_Forest.jpg", "阳光枫林 (Sunshine & Maple Forest)", "2K"},

            // 4K 壁纸
            {":/Mel/res/wallpaper/4k/doomsday&power_station.jpg", "末日发电站 (Doomsday & Power Station)", "4K"}, {":/Mel/res/wallpaper/4k/road&sakuar&inverted_image.png", "樱花之路 (Road & Sakura)", "4K"},
            {":/Mel/res/wallpaper/4k/room&window.png", "房间窗景 (Room & Window)", "4K"}};

    for (const auto &wallpaper: _wallpapers) {
        QString itemText = QString("%1 [%2]").arg(wallpaper.displayName, wallpaper.resolution);
        comboBoxResourcePath->addItem(itemText);
    }

    connect(comboBoxResourcePath, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BackgroundWidgetExample::onWallpaperChanged);

    mainLayout->addWidget(widgetPanel);

    comboBoxMode = new QComboBox(this);
    comboBoxMode->addItem("填满窗口 剪裁");
    comboBoxMode->setItemData(0, Mel::ScaleMode_Fill);
    comboBoxMode->addItem("适应窗口 留空");
    comboBoxMode->setItemData(1, Mel::ScaleMode_Fit);
    comboBoxMode->addItem("拉伸填充 变形");
    comboBoxMode->setItemData(2, Mel::ScaleMode_Stretch);
    connect(comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BackgroundWidgetExample::onModelChange);
    layout->addWidget(comboBoxMode);

    auto colorButton = new QPushButton("选择背景色", this);
    layout->addWidget(colorButton);
    connect(colorButton, &QPushButton::clicked, this, &BackgroundWidgetExample::onSelectBackgroundColor);
    resize(800, 600);
}

void BackgroundWidgetExample::setCurrentWallpaper(const QString &resourcePath) const {
    backgroundWidget->setBackgroundImage(resourcePath);
}

void BackgroundWidgetExample::onWallpaperChanged(const int index) {
    if (index >= 0 && index < _wallpapers.size()) {
        setCurrentWallpaper(_wallpapers[index].path);
    }
}

void BackgroundWidgetExample::onModelChange(const int index) const {
    const Mel::BackgroundScaleMode mode = static_cast<Mel::BackgroundScaleMode>(comboBoxMode->itemData(index).toInt());
    backgroundWidget->setScaleMode(mode);
}

void BackgroundWidgetExample::onSelectBackgroundColor() {
    // 获取当前背景色
    QColor currentColor = backgroundWidget->getBackgroundColor();
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
        backgroundWidget->setBackgroundColor(color);
    } else {
        // 用户点击了取消，清除背景色
        backgroundWidget->setBackgroundColor(QColor());
    }
}
