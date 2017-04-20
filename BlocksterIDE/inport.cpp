#include "inport.h"

Inport::Inport(int x, int y) : x(x), y(y)
{
    setFlags(ItemIsSelectable);
}

QRectF Inport::boundingRect() const
{
    return QRectF(x-12, y-12, 14, 24);
}

QPainterPath Inport::shape() const
{
    QPainterPath path;
    path.addRect(x-12, y-12, 14, 24);
    return path;
}

void Inport::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QPen pen;
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QLine(x-5,y-5,x,y));
    painter->drawLine(QLine(x,y,x-5,y+5));

    return;
}
