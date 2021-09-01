﻿#include "qframelessmainwindow.h"

QFramelessMainWindow::QFramelessMainWindow(QWidget *parent, bool resizeEnable, bool shadowBorder, bool winNativeEvent)
    : QMainWindow(parent)
    , m_framelessHelper(new FramelessHelper(this, resizeEnable, shadowBorder, winNativeEvent, this))
{    
}

FramelessHelper *QFramelessMainWindow::framelessHelper()
{
    return m_framelessHelper;
}

void QFramelessMainWindow::paintEvent(QPaintEvent *e)
{
    m_framelessHelper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool QFramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool QFramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    return m_framelessHelper->nativeEvent(eventType, message, result);
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef Q_OS_WIN
bool QFramelessMainWindow::winEvent(MSG *message, long *result)
{
    return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif

