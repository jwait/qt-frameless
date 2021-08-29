﻿#ifndef FRAMELESSHELPER_H
#define FRAMELESSHELPER_H

#include <QObject>
#include <QWidget>

class DrawShadowHelper;
class FramelessHelper : public QObject
{
    Q_OBJECT
public:
    explicit FramelessHelper(QWidget* w, QObject *parent = 0);

protected:
    //窗体显示的时候触发
    void doShowEvent(QEvent *event);

    //事件过滤器识别拖动拉伸等
    void doWindowStateChange(QEvent *event);
    void doResizeEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

public:
    //拦截系统事件用于修复系统休眠后唤醒程序的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

//    //Qt4的写法
//#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
//#ifdef Q_OS_WIN
//    bool winEvent(MSG *message, long *result);
//#endif
//#endif

    void paintEvent(QPaintEvent *e);
private:
    QWidget *widget;
    //边距+可移动+可拉伸+阴影边框
    int padding;
    bool moveEnable;
    bool resizeEnable;
    bool shadowEnable;

    //标题栏控件
    QWidget *titleBar;

    //鼠标是否按下+按下坐标+按下时窗体区域
    bool mousePressed;
    QPoint mousePoint;
    QRect mouseRect;

    //鼠标是否按下某个区域+按下区域的大小
    //依次为 左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
    QList<bool> pressedArea;
    QList<QRect> pressedRect;

    //记录是否最小化
    bool isMin;
    //存储窗体默认的属性
    Qt::WindowFlags flags;

    //windows是否采用NativeEvent进行resize，linux下该值为false
    bool nativeResize;

    //自绘制阴影边框
    DrawShadowHelper* drawShadow;

public:
    //设置边距+可拖动+可拉伸
    void setPadding(int padding);
    void setMoveEnable(bool moveEnable);
    void setResizeEnable(bool resizeEnable);

    //设置标题栏控件
    void setTitleBar(QWidget *titleBar);

signals:
    void titleDblClick();
    void windowStateChange(bool max);
};

#endif // FRAMELESSHELPER_H
