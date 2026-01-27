#ifndef MEL_BACKGROUNDWIDGETEXAMPLE_H
#define MEL_BACKGROUNDWIDGETEXAMPLE_H

#include <QWidget>
#include <QHBoxLayout>

#include "ElaComboBox.h"
#include "widgets/BackgroundWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BackgroundWidgetExample;
}
QT_END_NAMESPACE

class BackgroundWidgetExample : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundWidgetExample(QWidget *parent = nullptr);

    ~BackgroundWidgetExample() override;

    void initUi();
    void setCurrentWallpaper(const QString& resourcePath) const;

public slots:
    void onWallpaperChanged(int index);
    void onModelChange(int index) const;
    void onSelectBackgroundColor();

private:
    Ui::BackgroundWidgetExample *ui;

    QHBoxLayout *mainLayout = nullptr;
    Mel::BackgroundWidget *backgroundWidget = nullptr;
    QWidget *widgetPanel = nullptr;
    QVBoxLayout *layout = nullptr;
    QComboBox *comboBoxResourcePath = nullptr;
    QComboBox *comboBoxMode = nullptr;

    struct WallpaperInfo {
        QString path;        // 资源路径
        QString displayName; // 显示名称
        QString resolution;  // 分辨率
    };

    QList<WallpaperInfo> _wallpapers;
};


#endif //MEL_BACKGROUNDWIDGETEXAMPLE_H