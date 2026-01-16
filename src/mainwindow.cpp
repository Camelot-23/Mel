/**
 * @file mainwindow.cpp
 * @brief Main Window Implementation
 * 
 * Compatible with Qt 4, 5, and 6.
 */

#include "mainwindow.h"
#include <QMessageBox>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , layout(nullptr)
    , helloLabel(nullptr)
    , versionLabel(nullptr)
    , closeButton(nullptr)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    // Qt's parent-child relationship handles cleanup
}

void MainWindow::setupUi()
{
    // Set window properties
    setWindowTitle(tr("Qt Hello World"));
    setMinimumSize(400, 300);
    resize(500, 350);

    // Create central widget and layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(20);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    layout->setContentsMargins(40, 40, 40, 40);
#else
    layout->setMargin(40);
#endif

    // Create and configure hello label
    helloLabel = new QLabel(tr("Hello, World!"), centralWidget);
    helloLabel->setAlignment(Qt::AlignCenter);
    
    // Set font for hello label
    QFont helloFont = helloLabel->font();
    helloFont.setPointSize(24);
    helloFont.setBold(true);
    helloLabel->setFont(helloFont);
    
    // Create version info label
    versionLabel = new QLabel(getQtVersionString(), centralWidget);
    versionLabel->setAlignment(Qt::AlignCenter);
    
    QFont versionFont = versionLabel->font();
    versionFont.setPointSize(12);
    versionLabel->setFont(versionFont);
    
    // Create close button
    closeButton = new QPushButton(tr("Close"), centralWidget);
    closeButton->setMinimumHeight(40);
    
    // Connect button signal to slot
    connect(closeButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    // Add widgets to layout
    layout->addStretch();
    layout->addWidget(helloLabel);
    layout->addWidget(versionLabel);
    layout->addStretch();
    layout->addWidget(closeButton);
}

QString MainWindow::getQtVersionString() const
{
    QString version = QString("Running on Qt %1.%2.%3")
        .arg(QT_VERSION_MAJOR)
        .arg(QT_VERSION_MINOR)
        .arg(QT_VERSION_PATCH);
    
#if QT_VERSION_MAJOR == 6
    version += " (Qt6)";
#elif QT_VERSION_MAJOR == 5
    version += " (Qt5)";
#elif QT_VERSION_MAJOR == 4
    version += " (Qt4)";
#endif

    return version;
}

void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, 
        tr("Goodbye"), 
        tr("Thank you for using Qt Hello World!\n\nThis application supports Qt 4, 5, and 6."));
    close();
}
