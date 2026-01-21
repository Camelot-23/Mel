#include <QApplication>
#include <QDebug>
#include "Mel.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QCoreApplication::setApplicationName("Mel Example");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("Mel");

    // 打印 Mel 库版本信息
    qDebug() << "=================================";
    qDebug() << "Mel 壁纸切换演示 (Qt Native)";
    qDebug() << "=================================";
    qDebug() << "Mel 库版本:" << Mel::MelLib::getVersion();
    qDebug() << "主版本号:" << Mel::MelLib::getVersionMajor();
    qDebug() << "次版本号:" << Mel::MelLib::getVersionMinor();
    qDebug() << "补丁版本号:" << Mel::MelLib::getVersionPatch();
    qDebug() << "=================================";
    qDebug() << "可用壁纸: 6 张 (1K/2K/4K)";
    qDebug() << "使用纯 Qt 原生控件实现";
    qDebug() << "=================================";

    // 创建并显示主窗口
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
