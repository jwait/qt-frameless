#include "qframelesswidget.h"

#include "qframelesshelper.h"

QFramelessWidget::QFramelessWidget(QWidget *parent, bool resize_enable, bool shadow_border,
                                   bool win_native_event)
    : QWidget(parent)
{
    m_frameless_helper =
        new QFramelessHelper(this, resize_enable, shadow_border, win_native_event, this);
}

QFramelessHelper *QFramelessWidget::framelessHelper() { return m_frameless_helper; }

void QFramelessWidget::paintEvent(QPaintEvent *e)
{
    if (m_frameless_helper == 0) return;
    m_frameless_helper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    if (m_frameless_helper == 0) return false;
    return m_frameless_helper->nativeEvent(eventType, message, result);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
bool QFramelessWidget::winEvent(MSG *message, long *result)
{
    return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif
