﻿#ifndef QFRAMELESSMAINWINDOW_H
#define QFRAMELESSMAINWINDOW_H

#include <QMainWindow>

#include "gframeless_global.h"
#include "qframelesswidget.h"

class QFramelessHelper;
class QFRAMELESS_EXPORT QFramelessMainWindow : public QMainWindow
{
    Q_OBJECT
   public:
    explicit QFramelessMainWindow(QWidget *parent = nullptr, bool resize_enable = true,
                                  bool shadow_border = true, bool win_native_event = true);

    QFramelessHelper *framelessHelper();

   protected:
    //拦截系统事件用于修复系统休眠后唤醒程序的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

    // Qt4的写法
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
    bool winEvent(MSG *message, long *result);
#endif
#endif

    virtual void paintEvent(QPaintEvent *e);

   private:
    QFramelessHelper *m_frameless_helper = nullptr;
};

#endif  // QFRAMELESSMAINWINDOW_H
