#ifndef QFRAMELESSHELPER_H
#define QFRAMELESSHELPER_H

#include <QWidget>

#include "gframeless_global.h"

class QDrawShadowHelper;
class QFRAMELESS_EXPORT QFramelessHelper : public QObject
{
    Q_OBJECT
   public:
    explicit QFramelessHelper(QWidget *w, bool resize_enable = true, bool shadow_border = true,
                              bool win_native_event = true, QObject *parent = nullptr);

    //设置边距+可拖动+可拉伸
    void setPadding(int padding);
    void setMoveEnable(bool move_enable);
    void setResizeEnable(bool resize_enable);

    //设置标题栏窗体
    void setTitleBar(QWidget *titleBar);

    bool isMaximized();

   public slots:
    void showMinimized();
    void showMaximized();
    void showNormal();
    void switchMaximizedNormal();

   protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    //窗体显示的时候触发
    void doShowEvent(QEvent *event);

    //事件过滤器识别拖动拉伸等
    void doWindowStateChange(QEvent *event);
    void doResizeEvent(QEvent *event);

    //更新自绘阴影边框显示状态
    void updateDrawShadowState();

   public:
    //拦截系统事件用于修复系统休眠后唤醒程序的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

    void paintEvent(QPaintEvent *e);

   signals:
    void signalMaximizedChanged(bool max);

   private:
    QWidget *m_widget = nullptr;
    //边距+可移动+可拉伸+阴影边框
    int m_padding = 8;
    bool m_move_enable = true;
    bool m_resize_enable;
    bool m_shadow_border;
    //通过边框进行resize
    bool m_border_resize_enable;

    //标题栏控件
    QWidget *m_title_bar = nullptr;

    //鼠标是否按下+按下坐标+按下时窗体区域
    bool m_mouse_pressed = false;
    QPoint m_mouse_global_point{0, 0};
    QRect m_mouse_rect{0, 0, 0, 0};

    //鼠标是否按下某个区域+按下区域的大小
    //依次为 左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
    QList<bool> m_pressed_area;
    QList<QRect> m_pressed_rect;

    //记录是否最小化
    bool m_is_min = false;
    //存储窗体默认的属性
    Qt::WindowFlags m_flags;

    // windows是否采用NativeEvent进行Resize、阴影边框，linux下该值为false
    bool m_win_native_event = false;

    //自绘制阴影边框
    QDrawShadowHelper *m_draw_shadow = nullptr;
};

#endif  // QFRAMELESSHELPER_H
