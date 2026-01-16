#include <QApplication>
#include "mainwindow.h"

#include <ElaApplication.h>
#include <ElaWindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    eApp->init();
    
    // Set application information
    QCoreApplication::setApplicationName("Qt Hello World");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("QtHelloWorld");
    
    // MainWindow mainWindow;
    // mainWindow.show();

    ElaWindow window;
    window.show();

    return QApplication::exec();
}
