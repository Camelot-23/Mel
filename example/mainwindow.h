/**
 * @file mainwindow.h
 * @brief 主窗口 - 使用 Qt 原生控件实现壁纸切换
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>

#include "backgroundWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onWallpaperChanged(int index);
    void onScaleModeChanged(int index);
    void onThemeToggled();
    void onClearBackground();
    void onSelectBackgroundColor();

private:
    void setupUi();
    void initWallpapers();
    void setCurrentWallpaper(const QString& resourcePath);

    // UI 组件
    BackgroundWidget* _backgroundWidget;  // 背景控件（左侧）
    QWidget* _contentWidget;              // 控制面板（右侧）
    QVBoxLayout* _mainLayout;             // 控制面板的主布局
    
    QLabel* _titleLabel;
    QLabel* _descLabel;
    QComboBox* _wallpaperCombo;
    QComboBox* _scaleModeCombo;
    QPushButton* _themeButton;
    QPushButton* _colorButton;
    QPushButton* _clearButton;
    QPushButton* _infoButton;
    
    // 壁纸数据
    struct WallpaperInfo {
        QString path;        // 资源路径
        QString displayName; // 显示名称
        QString resolution;  // 分辨率
    };
    
    QList<WallpaperInfo> _wallpapers;
    int _currentWallpaperIndex;
    
    // 主题
    bool _isDarkTheme;
};

#endif // MAINWINDOW_H
