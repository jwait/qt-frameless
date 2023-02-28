#ifndef QSHADOWHELPER_H
#define QSHADOWHELPER_H

#include <QWidget>

#include "gframeless_global.h"

class QPainter;

class QFRAMELESS_EXPORT QDrawShadowHelper : public QObject
{
    Q_OBJECT

   public:
    explicit QDrawShadowHelper(QWidget *w, int shadowSize = 10, QObject *parent = 0);
    ~QDrawShadowHelper();

    void setShadowSize(int shadow_size);
    int shadowSize();
    void paint(QPainter *p);

    QColor getClientBorderColor() const;
    void setClientBorderColor(const QColor &value);

   public slots:
    void hide();
    void show();

   protected:
    QImage makeShadowImage(int shadow_size, bool activated);
    bool splitRect(const QRect &src_rect, int shadow_size, QRect *parray_rect, int array_count);

   private:
    int m_shadow_size = 0;
    QImage m_img;
    QRect m_array_image_grid[9];
    QColor m_client_border_color;
    QColor m_client_bg_color;
    QWidget *m_widget = nullptr;
    bool m_show = true;
};

#endif  // QSHADOWHELPER_H
