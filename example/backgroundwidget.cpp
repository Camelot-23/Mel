/**
 * @file backgroundwidget.cpp
 * @brief 背景图片控件实现
 */

#include "backgroundwidget.h"
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent), _scaleMode(ScaleMode_Fill), _smoothTransformation(true), _backgroundColor(QColor()) // 默认无效颜色（透明）
  , _transitionAnimation(nullptr), _transitionOpacity(1.0), _transitionDuration(300)                     // 默认300毫秒
{
    // 设置默认属性
    setAttribute(Qt::WA_StyledBackground, true);

    // 创建过渡动画
    _transitionAnimation = new QPropertyAnimation(this, "transitionOpacity", this);
    _transitionAnimation->setDuration(_transitionDuration);
    _transitionAnimation->setStartValue(0.0);
    _transitionAnimation->setEndValue(1.0);
    _transitionAnimation->setEasingCurve(QEasingCurve::InOutQuad);
}

BackgroundWidget::~BackgroundWidget() = default;

// ========== 背景图片设置 ==========

bool BackgroundWidget::setBackgroundImage(const QString &path) {
    QPixmap pixmap;
    if (!pixmap.load(path)) {
        qWarning() << "BackgroundWidget: 无法加载图片:" << path;
        return false;
    }

    qDebug() << "BackgroundWidget: 加载图片成功:" << path << "尺寸:" << pixmap.size();

    setBackgroundPixmap(pixmap);
    return true;
}

void BackgroundWidget::setBackgroundPixmap(const QPixmap &pixmap) {
    // 如果启用了动画且有旧图片
    if (_transitionDuration > 0 && !_scaledBackground.isNull()) {
        // 保存旧地缩放图片用于动画
        _oldScaledBackground = _scaledBackground;

        // 设置新图片
        _backgroundImage = pixmap;
        updateScaledPixmap();

        // 停止当前动画（如果正在运行）
        if (_transitionAnimation->state() == QPropertyAnimation::Running) {
            _transitionAnimation->stop();
        }

        // 启动淡入动画
        _transitionOpacity = 0.0;
        _transitionAnimation->start();

        qDebug() << "BackgroundWidget: 启动背景切换动画，时长:" << _transitionDuration << "ms";
    } else {
        // 无动画或首次设置，直接切换
        _backgroundImage = pixmap;
        updateScaledPixmap();
        _transitionOpacity = 1.0;
        update();
    }
}

void BackgroundWidget::clearBackground() {
    _backgroundImage  = QPixmap();
    _scaledBackground = QPixmap();
    update();
}

// ========== 缩放模式 ==========

void BackgroundWidget::setScaleMode(BackgroundScaleMode mode) {
    if (_scaleMode != mode) {
        _scaleMode = mode;
        updateScaledPixmap();
        update();

        qDebug() << "BackgroundWidget: 缩放模式切换到:" << (mode == ScaleMode_Fill ? "填满" : mode == ScaleMode_Fit ? "适应" : "拉伸");
    }
}

// ========== 背景色和遮罩 ==========

void BackgroundWidget::setBackgroundColor(const QColor &color) {
    _backgroundColor = color;
    update();
}

void BackgroundWidget::setOverlayColor(const QColor &color) {
    _overlayColor = color;
    update();
}

void BackgroundWidget::clearOverlay() {
    _overlayColor = QColor();
    update();
}

// ========== 高级选项 ==========

void BackgroundWidget::setSmoothTransformation(bool smooth) {
    if (_smoothTransformation != smooth) {
        _smoothTransformation = smooth;
        updateScaledPixmap();
        update();
    }
}

// ========== 动画设置 ==========

void BackgroundWidget::setTransitionDuration(int duration) {
    _transitionDuration = qMax(0, duration);
    if (_transitionAnimation) {
        _transitionAnimation->setDuration(_transitionDuration);
    }

    qDebug() << "BackgroundWidget: 动画时长设置为:" << _transitionDuration << "ms";
}

void BackgroundWidget::setTransitionOpacity(const qreal opacity) {
    _transitionOpacity = qBound(0.0, opacity, 1.0);
    update(); // 触发重绘
}

// ========== 受保护方法 ==========

void BackgroundWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 如果没有背景图片
    if (_scaledBackground.isNull()) {
        // 绘制背景色（如果有效）
        if (_backgroundColor.isValid()) {
            painter.fillRect(rect(), _backgroundColor);
        }

        // 绘制遮罩层（如果有）
        if (hasOverlay()) {
            painter.fillRect(rect(), _overlayColor);
        }

        // 如果背景色无效，不绘制任何内容
        QWidget::paintEvent(event);
        return;
    }

    // 如果是 Fit 模式且有空白，先填充背景色（如果背景色有效）
    if (_scaleMode == ScaleMode_Fit && _backgroundColor.isValid()) {
        painter.fillRect(rect(), _backgroundColor);
    }

    // 如果正在进行过渡动画且有旧图片
    if (_transitionOpacity < 1.0 && !_oldScaledBackground.isNull()) {
        // 先绘制旧图片（完全不透明）
        const int oldX = (width() - _oldScaledBackground.width()) / 2;
        const int oldY = (height() - _oldScaledBackground.height()) / 2;
        painter.drawPixmap(oldX, oldY, _oldScaledBackground);

        // 再绘制新图片（带透明度）
        painter.setOpacity(_transitionOpacity);
        const int newX = (width() - _scaledBackground.width()) / 2;
        const int newY = (height() - _scaledBackground.height()) / 2;
        painter.drawPixmap(newX, newY, _scaledBackground);
        painter.setOpacity(1.0); // 恢复透明度

        // 动画完成后清除旧图片
        if (_transitionOpacity >= 1.0) {
            _oldScaledBackground = QPixmap();
        }
    } else {
        // 正常绘制（无动画或动画已完成）
        const int x = (width() - _scaledBackground.width()) / 2;
        const int y = (height() - _scaledBackground.height()) / 2;
        painter.drawPixmap(x, y, _scaledBackground);
    }

    // 绘制遮罩层
    if (hasOverlay()) {
        painter.fillRect(rect(), _overlayColor);
    }

    QWidget::paintEvent(event);
}

void BackgroundWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // 窗口大小改变时重新缩放背景图片
    updateScaledPixmap();
}

// ========== 私有方法 ==========

void BackgroundWidget::updateScaledPixmap() {
    if (_backgroundImage.isNull() || width() <= 0 || height() <= 0) {
        _scaledBackground = QPixmap();
        return;
    }

    // 根据缩放模式选择 Qt::AspectRatioMode
    Qt::AspectRatioMode aspectMode;

    switch (_scaleMode) {
        case ScaleMode_Fill:
            aspectMode = Qt::KeepAspectRatioByExpanding; // 填满（可能裁剪）
            break;
        case ScaleMode_Fit:
            aspectMode = Qt::KeepAspectRatio; // 适应（可能留空）
            break;
        case ScaleMode_Stretch:
            aspectMode = Qt::IgnoreAspectRatio; // 拉伸（可能变形）
            break;
        default:
            aspectMode = Qt::KeepAspectRatioByExpanding;
    }

    // 选择变换质量
    const Qt::TransformationMode transMode = _smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation;

    // 缩放图片
    _scaledBackground = _backgroundImage.scaled(size(), aspectMode, transMode);
}

void BackgroundWidget::drawDefaultBackground(QPainter &painter) const {
    // 只绘制纯色背景
    if (_backgroundColor.isValid()) {
        painter.fillRect(rect(), _backgroundColor);
    }
    // 如果背景色无效，则不绘制任何内容（保持透明或系统默认）
}
