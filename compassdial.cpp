#include "compassdial.h"
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QtMath>
#include <QApplication>
#include <QMouseEvent>
#include <QtDebug>
#include <QTimer>

CompassDial::CompassDial(QWidget *parent): QAbstractSlider(parent)
{
    QPixmap indicator;
    indicator.load(":/res/indicator.svg");
    indicatorWidth = indicator.scaledToHeight(indicatorSize).width();

    this->setRange(0, 359);

    QFont font = this->font();
    font.setBold(true);
    font.setPointSize(font.pointSize() + 2);
    setFont(font);
}

void CompassDial::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    // 绘制背景
    auto rect = this->rect();
    int line = qMin(rect.width(), rect.height()) - indicatorSize * 2 - 2;
    QRectF targetRect(rect.left() + (rect.width() - line) / 2.0 + 1,
                      rect.top() + (rect.height() - line) / 2.0, line, line);
    render.load(QString(":/res/compass.svg"));
    render.render(&painter, targetRect);

    // 绘制数字
    QString value = QString::number(this->value());
    QFontMetrics metrics(this->font());
    QRect textRect(targetRect.left() + (targetRect.width() - metrics.horizontalAdvance(value)) / 2,
                   targetRect.top() + (targetRect.height() - metrics.height()) / 2,
                   metrics.horizontalAdvance(value), metrics.height());
    painter.drawText(textRect, value);

    // 绘制指示三角
    QRectF indicatorRect(-indicatorWidth / 2, line / 2, indicatorWidth, indicatorSize);
    painter.save();
    painter.translate(targetRect.center());
    painter.rotate(this->value() - 180);
    render.load(QString(":/res/indicator.svg"));
    render.render(&painter, indicatorRect);
    painter.restore();

    // 处理disable状态
    if(!this->isEnabled()) {
        painter.fillRect(this->rect(), QColor(0, 0, 0, 50));
    }
}

void CompassDial::wheelEvent(QWheelEvent *ev)
{
    Q_UNUSED(ev)
}

int CompassDial::valueFromPoint(const QPoint &p) const
{
    double yy = this->height() / 2.0 - p.y();
    double xx = p.x() - this->width() / 2.0;
    double a = (xx || yy) ? std::atan2(yy, xx) : 0;
    a += 3 / 2 * M_PI;

    if (a < M_PI / -2)
        a = a + M_PI * 2;

    int dist = 0;
    int minv = minimum(), maxv = maximum();

    if (minimum() < 0) {
        dist = -minimum();
        minv = 0;
        maxv = maximum() + dist;
    }

    int r = maxv - minv;
    int v = (int)(0.5 + minv + r * (M_PI * 3 / 2 - a) / (2 * M_PI));

    if (dist > 0)
        v -= dist;

    return bound(v);
}

int CompassDial::bound(int val) const
{
    if ((val >= minimum()) && (val <= maximum()))
        return val;
    val = minimum() + ((val - minimum()) % (maximum() - minimum()));
    if (val < minimum())
        val += maximum() - minimum();
    return val;
}

void CompassDial::mousePressEvent(QMouseEvent *e)
{
    if (this->maximum() == this->minimum() ||
            (e->button() != Qt::LeftButton)  ||
            (e->buttons() ^ e->button())) {
        e->ignore();
        return;
    }
    e->accept();
    setSliderPosition(this->valueFromPoint(e->pos()));
    setSliderDown(true);
}

void CompassDial::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->buttons() & (~e->button()) ||
            (e->button() != Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
    setValue(this->valueFromPoint(e->pos()));
    setSliderDown(false);
}

void CompassDial::mouseMoveEvent(QMouseEvent * e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
    blockSignals(true);
    setSliderPosition(this->valueFromPoint(e->pos()));
    blockSignals(false);
}

void CompassDial::showEvent(QShowEvent *ev)
{
    QAbstractSlider::showEvent(ev);
}

QSize CompassDial::minimumSizeHint() const
{
    return QSize(160, 160);
}

QSize CompassDial::sizeHint() const
{
    return QSize(160, 160).expandedTo(QApplication::globalStrut());
}

bool CompassDial::event(QEvent *e)
{
    return QAbstractSlider::event(e);
}

