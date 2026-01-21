/**
 * @file mainwindow.h
 * @brief 主窗口 - 使用 Qt 原生控件实现壁纸切换
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onWallpaperChanged(int index);
    void onScaleModeChanged(int index);
    void onClearBackground();
    void onSelectBackgroundColor();
    void onShowAbout();

private:
    void initUi();
    void initWallpapers();
    void setCurrentWallpaper(const QString& resourcePath);
    
    Ui::MainWindow *ui;
    
    // 壁纸数据
    struct WallpaperInfo {
        QString path;        // 资源路径
        QString displayName; // 显示名称
        QString resolution;  // 分辨率
    };
    
    QList<WallpaperInfo> _wallpapers;
    int _currentWallpaperIndex;
};

#endif // MAINWINDOW_H
