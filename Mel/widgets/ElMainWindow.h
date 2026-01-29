#ifndef MEL_ELMAINWINDOW_H
#define MEL_ELMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "Mel_export.h"

class QHBoxLayout;

namespace Mel {

// 标题栏按钮
class TitleBarButton : public QPushButton {
    Q_OBJECT
public:
    TitleBarButton(const QString &text, const QColor &hoverBg, QWidget *parent = nullptr);
    void setHovered(bool hovered);
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    QColor _hoverBgColor;
    QColor _normalTextColor{200, 200, 200};
    bool _hovered = false;
};

/**
 * @brief 无边框主窗口（支持 Windows 原生动画和 Snap Layout）
 */
class MEL_EXPORT ElMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ElMainWindow(QWidget *parent = nullptr);
    ~ElMainWindow() override = default;

    void setTitleBarHeight(int height);
    [[nodiscard]] int titleBarHeight() const { return _titleBarHeight; }

    void setResizable(bool resizable);
    [[nodiscard]] bool isResizable() const { return _resizable; }

Q_SIGNALS:
    void closeButtonClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif
#endif

private Q_SLOTS:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();

private:
    void setupTitleBar();
    void updateMaximizeButton();
    bool containsCursorToItem(QWidget *item) const;

    QWidget        *_titleBar;
    TitleBarButton *_minimizeBtn;
    TitleBarButton *_maximizeBtn;
    TitleBarButton *_closeBtn;

    int  _titleBarHeight;
    bool _resizable;
    bool _isHoverMaxButton;
    
#ifdef Q_OS_WIN
    qint64 _currentWinID;
#endif
};

} // namespace Mel

#endif // MEL_ELMAINWINDOW_H
