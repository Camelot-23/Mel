#include <QApplication>
#include <QDebug>


#include <ElaApplication.h>
#include <ElaWindow.h>

#include "Mel.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    eApp->init();
    
    // Set application information
    QCoreApplication::setApplicationVersion("1.0.0");

    // MainWindow mainWindow;
    // mainWindow.show();

    Mel::MelLib melLib;
    qDebug() << "mel version:" << melLib.getVersion();

    ElaWindow window;
    window.show();

    return QApplication::exec();
}
