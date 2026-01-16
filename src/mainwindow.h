/**
 * @file mainwindow.h
 * @brief Main Window Header File
 * 
 * Compatible with Qt 4, 5, and 6.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();

private:
    void setupUi();
    QString getQtVersionString() const;

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *helloLabel;
    QLabel *versionLabel;
    QPushButton *closeButton;
};

#endif // MAINWINDOW_H
