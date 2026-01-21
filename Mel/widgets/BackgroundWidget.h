/**
 * @file BackgroundWidget.h
 * @brief 背景图片控件 - 支持自动缩放和居中显示
 */

#ifndef MEL_BACKGROUNDWIDGET_H
#define MEL_BACKGROUNDWIDGET_H

#include "Mel_export.h"
#include <QWidget>

class QPropertyAnimation;

namespace Mel {

/**
 * @brief 背景图片缩放模式
 */
enum BackgroundScaleMode {
    ScaleMode_Fill    = 0, // 填满窗口（保持比例，可能裁剪）
    ScaleMode_Fit     = 1, // 适应窗口（保持比例，可能留空）
    ScaleMode_Stretch = 2  // 拉伸填充（忽略比例，可能变形）
};

/**
 * @brief 背景图片控件
 *
 * 功能：
 * - 设置背景图片（从文件或资源）
 * - 自动缩放以适应控件大小
 * - 多种缩放模式（填满/适应/拉伸）
 * - 支持背景色和遮罩
 */
class MEL_EXPORT BackgroundWidget : public QWidget {
    Q_OBJECT
public:
    explicit BackgroundWidget(QWidget *parent = nullptr);

    ~BackgroundWidget() override;

    // ========== 背景图片设置 ==========

    /**
     * @brief 设置背景图片（从文件路径或资源路径）
     * @param path 图片路径（支持 :/ 资源路径）
     * @return 是否加载成功
     */
    bool setBackgroundImage(const QString &path);

    /**
     * @brief 设置背景图片（从 QPixmap）
     * @param pixmap 图片对象
     */
    void setBackgroundPixmap(const QPixmap &pixmap);

    /**
     * @brief 清除背景图片
     */
    void clearBackground();

    /**
     * @brief 获取当前背景图片
     * @return 背景图片（可能为空）
     */
    [[nodiscard]] QPixmap getBackgroundPixmap() const { return _backgroundImage; }

    /**
     * @brief 背景图片是否为空
     */
    [[nodiscard]] bool isBackgroundEmpty() const { return _backgroundImage.isNull(); }

    // ========== 缩放模式 ==========

    /**
     * @brief 设置缩放模式
     * @param mode 缩放模式
     */
    void setScaleMode(BackgroundScaleMode mode);

    /**
     * @brief 获取当前缩放模式
     */
    [[nodiscard]] BackgroundScaleMode getScaleMode() const { return _scaleMode; }

    // ========== 背景色和遮罩 ==========

    /**
     * @brief 设置背景色（当没有背景图片或留空时显示）
     * @param color 背景颜色
     */
    void setBackgroundColor(const QColor &color);

    /**
     * @brief 获取背景色
     */
    [[nodiscard]] QColor getBackgroundColor() const { return _backgroundColor; }

    /**
     * @brief 设置遮罩（半透明覆盖层）
     * @param color 遮罩颜色（包含透明度）
     */
    void setOverlayColor(const QColor &color);

    /**
     * @brief 清除遮罩
     */
    void clearOverlay();

    /**
     * @brief 是否启用遮罩
     */
    [[nodiscard]] bool hasOverlay() const { return _overlayColor.isValid() && _overlayColor.alpha() > 0; }

    // ========== 高级选项 ==========

    /**
     * @brief 设置是否使用平滑变换（高质量但慢）
     * @param smooth true=平滑，false=快速
     */
    void setSmoothTransformation(bool smooth);

    /**
     * @brief 是否使用平滑变换
     */
    [[nodiscard]] bool isSmoothTransformation() const { return _smoothTransformation; }

    // ========== 动画设置 ==========

    /**
     * @brief 设置背景切换动画时长
     * @param duration 动画时长（毫秒），设为0禁用动画
     */
    void setTransitionDuration(int duration);

    /**
     * @brief 获取动画时长
     */
    [[nodiscard]] int getTransitionDuration() const { return _transitionDuration; }

    /**
     * @brief 设置过渡透明度（用于动画，通常不需要手动调用）
     */
    void setTransitionOpacity(qreal opacity);

    /**
     * @brief 获取过渡透明度
     */
    [[nodiscard]] qreal getTransitionOpacity() const { return _transitionOpacity; }

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    /**
     * @brief 更新缩放后的图片
     */
    void updateScaledPixmap();

    /**
     * @brief 绘制默认背景（渐变或纯色）
     */
    void drawDefaultBackground(QPainter &painter) const;

    // 背景图片
    QPixmap _backgroundImage;     // 原始图片
    QPixmap _scaledBackground;    // 缩放后的图片
    QPixmap _oldScaledBackground; // 旧地缩放图片（用于动画）

    // 缩放设置
    BackgroundScaleMode _scaleMode;
    bool                _smoothTransformation;

    // 颜色设置
    QColor _backgroundColor; // 背景色
    QColor _overlayColor;    // 遮罩颜色

    // 动画设置
    QPropertyAnimation *_transitionAnimation; // 过渡动画
    qreal               _transitionOpacity;   // 过渡透明度 (0.0-1.0)
    int                 _transitionDuration;  // 动画时长（毫秒）

    Q_PROPERTY(qreal transitionOpacity READ getTransitionOpacity WRITE setTransitionOpacity)
};

} // namespace Mel

#endif // MEL_BACKGROUNDWIDGET_H
