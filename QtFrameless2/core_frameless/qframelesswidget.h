﻿#ifndef QFRAMELESSWIDGET_H
#define QFRAMELESSWIDGET_H

#include <QDialog>
#include "framelesshelper.h"

class FramelessHelper;
class QFramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QFramelessWidget(QWidget *parent = 0, bool resizeEnable = false, bool shadowBorder = true, bool winNativeEvent = true);

    FramelessHelper *framelessHelper();

protected:
    //拦截系统事件用于修复系统休眠后唤醒程序的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

    //Qt4的写法
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef Q_OS_WIN
    bool winEvent(MSG *message, long *result);
#endif
#endif

    virtual void paintEvent(QPaintEvent *e);

private:
    FramelessHelper * m_framelessHelper;
};

#endif // QFRAMELESSWIDGET_H
