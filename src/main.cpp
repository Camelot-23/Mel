/**
 * @file main.cpp
 * @brief Qt Hello World Application Entry Point
 * 
 * This application demonstrates a simple Qt Hello World program
 * that is compatible with Qt 4, 5, and 6.
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    QCoreApplication::setApplicationName("Qt Hello World");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("QtHelloWorld");
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
