#include <QApplication>
#include <QDebug>
#include "Mel.h"
#include "mainwindow.h"
#include "BackgroundWidgetExample.h"
#include "widgets/ElMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QCoreApplication::setApplicationName("Mel Example");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("Mel");

    // 打印 Mel 库版本信息
    qDebug() << "=================================";
    qDebug() << "Mel 库版本:" << Mel::MelLib::getVersion();
    qDebug() << "主版本号:" << Mel::MelLib::getVersionMajor();
    qDebug() << "次版本号:" << Mel::MelLib::getVersionMinor();
    qDebug() << "补丁版本号:" << Mel::MelLib::getVersionPatch();
    qDebug() << "=================================";

    // 创建并显示主窗口
    // MainWindow w;
    // BackgroundWidgetExample w;
    Mel::ElMainWindow w;
    w.setWindowTitle("Mel ElMainWindow Demo");
    w.resize(800, 600);
    w.show();

    return QApplication::exec();
}
