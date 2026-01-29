#include "ElMainWindow.h"
#include <QMouseEvent>
#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPalette>
#include <QScreen>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#endif

namespace Mel {

// TitleBarButton 实现
TitleBarButton::TitleBarButton(const QString &text, const QColor &hoverBg, QWidget *parent)
    : QPushButton(text, parent), _hoverBgColor(hoverBg) {
    setFlat(true);
    setFocusPolicy(Qt::NoFocus);
}

void TitleBarButton::enterEvent(QEvent *event) {
    _hovered = true;
    update();
    QPushButton::enterEvent(event);
}

void TitleBarButton::leaveEvent(QEvent *event) {
    _hovered = false;
    update();
    QPushButton::leaveEvent(event);
}

void TitleBarButton::setHovered(bool hovered) {
    if (_hovered != hovered) {
        _hovered = hovered;
        update();
    }
}

void TitleBarButton::paintEvent(QPaintEvent *) {
    QPainter p(this);
    
    // 背景
    if (_hovered) {
        p.fillRect(rect(), _hoverBgColor);
        p.setPen(Qt::white);
    } else {
        p.setPen(_normalTextColor);
    }
    
    // 文字
    p.drawText(rect(), Qt::AlignCenter, text());
}

ElMainWindow::ElMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _titleBar(nullptr)
    , _minimizeBtn(nullptr)
    , _maximizeBtn(nullptr)
    , _closeBtn(nullptr)
    , _titleBarHeight(32)
    , _resizable(true)
    , _isHoverMaxButton(false)
#ifdef Q_OS_WIN
    , _currentWinID(0)
#endif
{
    setAttribute(Qt::WA_Mapped);
    setMouseTracking(true);
    setMinimumSize(400, 300);
    installEventFilter(this);
    
    setupTitleBar();
}

void ElMainWindow::setupTitleBar() {
    _titleBar = new QWidget(this);
    _titleBar->setFixedHeight(_titleBarHeight);
    _titleBar->setMouseTracking(true);
    _titleBar->setAutoFillBackground(true);
    
    QPalette pal = _titleBar->palette();
    pal.setColor(QPalette::Window, QColor(45, 45, 48));
    _titleBar->setPalette(pal);
    
    auto *layout = new QHBoxLayout(_titleBar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addStretch();
    
    _minimizeBtn = new TitleBarButton("─", QColor(70, 70, 74), _titleBar);
    _minimizeBtn->setFixedSize(46, _titleBarHeight);
    _minimizeBtn->setToolTip("最小化");
    connect(_minimizeBtn, &TitleBarButton::clicked, this, &ElMainWindow::onMinimizeClicked);
    layout->addWidget(_minimizeBtn);
    
    _maximizeBtn = new TitleBarButton("□", QColor(70, 70, 74), _titleBar);
    _maximizeBtn->setFixedSize(46, _titleBarHeight);
    _maximizeBtn->setToolTip("最大化");
    connect(_maximizeBtn, &TitleBarButton::clicked, this, &ElMainWindow::onMaximizeClicked);
    layout->addWidget(_maximizeBtn);
    
    _closeBtn = new TitleBarButton("✕", QColor(232, 17, 35), _titleBar);
    _closeBtn->setFixedSize(46, _titleBarHeight);
    _closeBtn->setToolTip("关闭");
    connect(_closeBtn, &TitleBarButton::clicked, this, &ElMainWindow::onCloseClicked);
    layout->addWidget(_closeBtn);
    
    setMenuWidget(_titleBar);
}

void ElMainWindow::setTitleBarHeight(int height) {
    if (_titleBarHeight != height && height > 0) {
        _titleBarHeight = height;
        if (_titleBar) {
            _titleBar->setFixedHeight(height);
            _minimizeBtn->setFixedHeight(height);
            _maximizeBtn->setFixedHeight(height);
            _closeBtn->setFixedHeight(height);
        }
    }
}

void ElMainWindow::setResizable(bool resizable) {
    _resizable = resizable;
#ifdef Q_OS_WIN
    if (_currentWinID) {
        HWND hwnd = reinterpret_cast<HWND>(_currentWinID);
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        if (resizable) {
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
        } else {
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style & ~WS_THICKFRAME);
        }
    }
#endif
}

void ElMainWindow::onMinimizeClicked() {
    showMinimized();
}

void ElMainWindow::onMaximizeClicked() {
    isMaximized() ? showNormal() : showMaximized();
}

void ElMainWindow::onCloseClicked() {
    Q_EMIT closeButtonClicked();
    close();
}

void ElMainWindow::updateMaximizeButton() {
    if (_maximizeBtn) {
        _maximizeBtn->setText(isMaximized() ? "❐" : "□");
        _maximizeBtn->setToolTip(isMaximized() ? "向下还原" : "最大化");
    }
}

bool ElMainWindow::containsCursorToItem(QWidget *item) const {
    if (!item || !item->isVisible()) return false;
    return QRect(item->mapToGlobal(QPoint(0, 0)), item->size()).contains(QCursor::pos());
}

bool ElMainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Resize && _titleBar) {
        _titleBar->resize(width(), _titleBarHeight);
    } else if (event->type() == QEvent::WindowStateChange) {
        updateMaximizeButton();
    }
#ifdef Q_OS_WIN
    else if (event->type() == QEvent::Show && _resizable && _currentWinID) {
        HWND hwnd = reinterpret_cast<HWND>(_currentWinID);
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        ::SetWindowLongPtr(hwnd, GWL_STYLE, (style & ~WS_SYSMENU) | WS_MAXIMIZEBOX | WS_THICKFRAME);
    }
#endif
    return QMainWindow::eventFilter(obj, event);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElMainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool ElMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    if (eventType != "windows_generic_MSG" || !message) {
        return QMainWindow::nativeEvent(eventType, message, result);
    }
    
    const auto msg = static_cast<MSG *>(message);
    if (!msg->hwnd) return QMainWindow::nativeEvent(eventType, message, result);
    
    _currentWinID = reinterpret_cast<qint64>(msg->hwnd);
    
    switch (msg->message) {
    case WM_NCACTIVATE:
        *result = TRUE;
        return true;
        
    case WM_SIZE:
        if (msg->wParam == SIZE_RESTORED || msg->wParam == SIZE_MAXIMIZED) {
            updateMaximizeButton();
        }
        return false;
        
    case WM_NCCALCSIZE: {
        if (msg->wParam == FALSE) return false;
        auto *rc = &reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam)->rgrc[0];
        const LONG top = rc->top;
        ::DefWindowProcW(msg->hwnd, WM_NCCALCSIZE, msg->wParam, msg->lParam);
        rc->top = top;
        if (::IsZoomed(msg->hwnd)) {
            // 获取窗口所在显示器的工作区
            HMONITOR monitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi;
            mi.cbSize = sizeof(mi);
            if (GetMonitorInfo(monitor, &mi)) {
                rc->top = mi.rcWork.top;
            }
        }
        *result = WVR_REDRAW;
        return true;
    }
    
    case WM_NCHITTEST: {
        POINT pt{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)};
        ::ScreenToClient(msg->hwnd, &pt);
        
        RECT rc;
        ::GetClientRect(msg->hwnd, &rc);
        
        // 使用设备像素比转换坐标
        qreal dpr = devicePixelRatioF();
        int ptX = static_cast<int>(pt.x / dpr);
        int ptY = static_cast<int>(pt.y / dpr);
        int clientWidth = static_cast<int>(rc.right / dpr);
        int titleBarH = _titleBarHeight;
        int btnWidth = 46;
        
        // 计算按钮区域（从右到左：关闭、最大化、最小化）
        int closeBtnLeft = clientWidth - btnWidth;
        int maxBtnLeft = clientWidth - btnWidth * 2;
        int minBtnLeft = clientWidth - btnWidth * 3;
        
        bool inTitleBar = ptY < titleBarH;
        bool inCloseBtn = inTitleBar && ptX >= closeBtnLeft;
        bool inMaxBtn = inTitleBar && ptX >= maxBtnLeft && ptX < closeBtnLeft;
        bool inMinBtn = inTitleBar && ptX >= minBtnLeft && ptX < maxBtnLeft;
        
        // 最大化按钮 - Snap Layout 支持
        if (inMaxBtn && _maximizeBtn->isVisible()) {
            if (!_isHoverMaxButton) {
                _isHoverMaxButton = true;
                _maximizeBtn->setHovered(true);
            }
            *result = HTMAXBUTTON;
            return true;
        }
        if (_isHoverMaxButton) {
            _isHoverMaxButton = false;
            _maximizeBtn->setHovered(false);
        }
        
        // 其他按钮返回 HTCLIENT 让 Qt 处理点击
        if (inCloseBtn || inMinBtn) {
            *result = HTCLIENT;
            return true;
        }
        
        const int m = static_cast<int>(4 * dpr);
        bool l = pt.x < m, r = pt.x > rc.right - m;
        bool t = pt.y < m, b = pt.y > rc.bottom - m;
        
        // 边框调整
        if (_resizable && !isFullScreen() && !isMaximized()) {
            if (l && b) { *result = HTBOTTOMLEFT; return true; }
            if (r && b) { *result = HTBOTTOMRIGHT; return true; }
            if (l && t && !inTitleBar) { *result = HTTOPLEFT; return true; }
            if (r && t && !inTitleBar) { *result = HTTOPRIGHT; return true; }
            if (l) { *result = HTLEFT; return true; }
            if (r) { *result = HTRIGHT; return true; }
            if (t && !inTitleBar) { *result = HTTOP; return true; }
            if (b) { *result = HTBOTTOM; return true; }
        }
        
        // 标题栏拖动
        if (inTitleBar) {
            *result = HTCAPTION;
            return true;
        }
        
        *result = HTCLIENT;
        return true;
    }
    
    case WM_GETMINMAXINFO: {
        auto *mmi = reinterpret_cast<MINMAXINFO *>(msg->lParam);
        
        // 获取窗口所在显示器的工作区
        HMONITOR monitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
        if (GetMonitorInfo(monitor, &mi)) {
            mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
            mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
            mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
            mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
        }
        
        mmi->ptMinTrackSize.x = static_cast<LONG>(minimumWidth() * devicePixelRatioF());
        mmi->ptMinTrackSize.y = static_cast<LONG>(minimumHeight() * devicePixelRatioF());
        return true;
    }
    
    case WM_MOVE: {
        // 窗口移动时刷新，处理跨屏幕 DPI 变化
        static HMONITOR lastMonitor = nullptr;
        HMONITOR currentMonitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
        if (lastMonitor && lastMonitor != currentMonitor) {
            ::SetWindowPos(msg->hwnd, nullptr, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        lastMonitor = currentMonitor;
        return false;
    }
    
    case WM_NCLBUTTONDOWN: {
        POINT pt{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)};
        ::ScreenToClient(msg->hwnd, &pt);
        RECT rc;
        ::GetClientRect(msg->hwnd, &rc);
        qreal dpr = devicePixelRatioF();
        int ptX = static_cast<int>(pt.x / dpr);
        int ptY = static_cast<int>(pt.y / dpr);
        int clientWidth = static_cast<int>(rc.right / dpr);
        int btnWidth = 46;
        bool inMaxBtn = ptY < _titleBarHeight && 
                        ptX >= clientWidth - btnWidth * 2 && 
                        ptX < clientWidth - btnWidth;
        if (inMaxBtn) return true;
        break;
    }
        
    case WM_NCLBUTTONUP: {
        POINT pt{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)};
        ::ScreenToClient(msg->hwnd, &pt);
        RECT rc;
        ::GetClientRect(msg->hwnd, &rc);
        qreal dpr = devicePixelRatioF();
        int ptX = static_cast<int>(pt.x / dpr);
        int ptY = static_cast<int>(pt.y / dpr);
        int clientWidth = static_cast<int>(rc.right / dpr);
        int btnWidth = 46;
        bool inMaxBtn = ptY < _titleBarHeight && 
                        ptX >= clientWidth - btnWidth * 2 && 
                        ptX < clientWidth - btnWidth;
        if (inMaxBtn) { 
            onMaximizeClicked(); 
            return true; 
        }
        break;
    }
        
    case WM_NCLBUTTONDBLCLK:
        if (!_resizable) return true;
        break;
    }
    
    return QMainWindow::nativeEvent(eventType, message, result);
}
#endif

} // namespace Mel
