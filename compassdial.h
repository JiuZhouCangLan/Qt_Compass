#ifndef COMPASSDIAL_H
#define COMPASSDIAL_H

#include <QDial>
#include <QSvgRenderer>

class CompassDial : public QAbstractSlider
{
    Q_OBJECT
public:
    CompassDial(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *pe) override;
    void wheelEvent(QWheelEvent *ev) override;
    void mousePressEvent(QMouseEvent *me) override;
    void mouseReleaseEvent(QMouseEvent *me) override;
    void mouseMoveEvent(QMouseEvent *me) override;
    void showEvent(QShowEvent *ev) override;

private:
    QSvgRenderer render;
    int indicatorSize = 10;
    int indicatorWidth = 0;
    bool light = false;

    int valueFromPoint(const QPoint &p) const;
    int bound(int val) const;
};

#endif // COMPASSDIAL_H
