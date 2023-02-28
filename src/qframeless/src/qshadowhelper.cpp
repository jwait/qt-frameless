#include "qshadowhelper.h"

#include <QMainWindow>
#include <QMap>
#include <QPainter>
#include <QVBoxLayout>

#include "qframelesshelper.h"

QDrawShadowHelper::QDrawShadowHelper(QWidget* w, int shadow_size, QObject* parent)
    : QObject(parent),
      m_shadow_size(shadow_size),
      m_client_border_color(QColor(Qt::black)),
      m_widget(w)
{
    m_widget->setAttribute(Qt::WA_TranslucentBackground);
    m_client_bg_color = m_widget->palette().color(QPalette::Window);
    setShadowSize(m_shadow_size);
}

QDrawShadowHelper::~QDrawShadowHelper() {}

void QDrawShadowHelper::setShadowSize(int shadow_size)
{
    m_shadow_size = shadow_size;
    if (!m_img.isNull())
    {
        m_img = QImage();
    }
    m_img = makeShadowImage(m_shadow_size, true);
    int width = m_img.width();
    int height = m_img.height();
    splitRect(QRect(0, 0, width, height), shadow_size, m_array_image_grid, 9);
}

inline unsigned char makeAlpha(int i, double f, int nSize)
{
    if (i == nSize) f *= 1.2;
    double f2 = 1 - cos((double)i / nSize * 3.14 / 2);
    return int(fabs((i * f) * f2));
}

QImage QDrawShadowHelper::makeShadowImage(int shadow_size, bool activated)
{
    int size = shadow_size * 2 + 10;
    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(QColor(Qt::black));

    //
    double f = activated ? 4.0 : 1.0;
    //
    QSize image_size = image.size();
    //
    // left
    for (int y = shadow_size; y < image_size.height() - shadow_size; y++)
    {
        for (int x = 0; x < shadow_size; x++)
        {
            int i = x + 1;
            int alpha = makeAlpha(i, f, shadow_size);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    // right
    for (int y = shadow_size; y < image_size.height() - shadow_size; y++)
    {
        for (int x = image_size.width() - shadow_size - 1; x < image_size.width(); x++)
        {
            int i = image_size.width() - x;
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    // top
    for (int y = 0; y < shadow_size; y++)
    {
        int i = y + 1;
        for (int x = shadow_size; x < image_size.width() - shadow_size; x++)
        {
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
        //
    }
    // bottom
    for (int y = image_size.height() - shadow_size - 1; y < image_size.height(); y++)
    {
        int i = image_size.height() - y;
        for (int x = shadow_size; x < image_size.width() - shadow_size; x++)
        {
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }

    //
    int parentRoundSize = 0;
    //
    for (int x = 0; x < shadow_size + parentRoundSize; x++)
    {
        for (int y = 0; y < shadow_size + parentRoundSize; y++)
        {
            int xx = (shadow_size + parentRoundSize) - x;
            int yy = (shadow_size + parentRoundSize) - y;
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadow_size + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadow_size - i;
            //
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }

    //
    for (int x = image_size.width() - shadow_size - parentRoundSize; x < image_size.width(); x++)
    {
        for (int y = 0; y < shadow_size + parentRoundSize; y++)
        {
            int xx = (shadow_size + parentRoundSize) - (image_size.width() - x);
            int yy = (shadow_size + parentRoundSize) - y;
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadow_size + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadow_size - i;
            //
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    for (int x = 0; x < shadow_size + parentRoundSize; x++)
    {
        for (int y = image_size.height() - shadow_size - parentRoundSize; y < image_size.height();
             y++)
        {
            int xx = (shadow_size + parentRoundSize) - x;
            int yy = (shadow_size + parentRoundSize) - (image_size.height() - y);
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadow_size + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadow_size - i;
            //
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    for (int x = image_size.width() - shadow_size - parentRoundSize; x < image_size.width(); x++)
    {
        for (int y = image_size.height() - shadow_size - parentRoundSize; y < image_size.height();
             y++)
        {
            int xx = (shadow_size + parentRoundSize) - (image_size.width() - x);
            int yy = (shadow_size + parentRoundSize) - (image_size.height() - y);
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadow_size + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadow_size - i;
            //
            int alpha = makeAlpha(i, f, shadow_size);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 6, 0))
            image.setPixel(x, y, QColor(0, 0, 0, alpha).rgba());
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    return image;
}

bool QDrawShadowHelper::splitRect(const QRect& src_rect, int shadow_size, QRect* parray_rect,
                                  int array_count)
{
    Q_ASSERT(array_count == 9);
    //
    QRect* arrayRect = parray_rect;
    //
    int width = src_rect.width();
    int height = src_rect.height();

    int x1 = src_rect.left() + 0;
    int x2 = src_rect.left() + shadow_size;
    int x3 = src_rect.left() + width - shadow_size;
    //
    int y1 = src_rect.top() + 0;
    int y2 = src_rect.top() + shadow_size;
    int y3 = src_rect.top() + height - shadow_size;
    //
    arrayRect[0] = QRect(x1, y1, shadow_size, shadow_size);
    arrayRect[1] = QRect(x2, y1, width - shadow_size * 2, shadow_size);
    arrayRect[2] = QRect(x3, y1, shadow_size, shadow_size);

    arrayRect[3] = QRect(x1, y2, shadow_size, height - shadow_size * 2);
    arrayRect[4] = QRect(x2, y2, width - shadow_size * 2, height - shadow_size * 2);
    arrayRect[5] = QRect(x3, y2, shadow_size, height - shadow_size * 2);

    arrayRect[6] = QRect(x1, y3, shadow_size, shadow_size);
    arrayRect[7] = QRect(x2, y3, width - shadow_size * 2, shadow_size);
    arrayRect[8] = QRect(x3, y3, shadow_size, shadow_size);
    //
    return true;
}

QColor QDrawShadowHelper::getClientBorderColor() const { return m_client_border_color; }

void QDrawShadowHelper::setClientBorderColor(const QColor& color)
{
    if (m_client_border_color != color)
    {
        m_client_border_color = color;
        if (m_widget) m_widget->update();
    }
}

void QDrawShadowHelper::paint(QPainter* p)
{
    p->save();
    if (m_shadow_size > 0 && m_show)
    {
        QRect array_dest[9];
        splitRect(m_widget->rect(), m_shadow_size, array_dest, 9);
        for (int i = 0; i < 9; i++)
        {
            const QRect& src_rect = m_array_image_grid[i];
            const QRect& dst_rect = array_dest[i];
            p->drawImage(dst_rect, m_img, src_rect);
        }
    }
    else
    {
        p->setBrush(m_client_bg_color);
        p->setPen(Qt::NoPen);
        p->drawRect(m_widget->rect());
    }
    p->restore();
}

int QDrawShadowHelper::shadowSize() { return m_shadow_size; }

void QDrawShadowHelper::hide()
{
    m_show = false;
    QLayout* lay = 0;
    if (m_widget->inherits("QMainWindow"))
    {
        QMainWindow* window = qobject_cast<QMainWindow*>(m_widget);
        lay = window->centralWidget()->layout();
    }
    else
    {
        lay = m_widget->layout();
    }
    if (lay) lay->setContentsMargins(0, 0, 0, 0);
}

void QDrawShadowHelper::show()
{
    m_show = true;
    QLayout* lay = 0;
    if (m_widget->inherits("QMainWindow"))
    {
        QMainWindow* window = qobject_cast<QMainWindow*>(m_widget);
        lay = window->centralWidget()->layout();
    }
    else
    {
        lay = m_widget->layout();
    }
    if (lay) lay->setContentsMargins(m_shadow_size, m_shadow_size, m_shadow_size, m_shadow_size);
}
