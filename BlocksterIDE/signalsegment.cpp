#include "signalline.h"

SignalSegment::SignalSegment(QGraphicsItem* parent, float x0, float y0, float x1, float y1) : x0(x0),y0(y0),x1(x1), y1(y1)
{
    setParentItem(parent);
}

QRectF SignalSegment::boundingRect() const
{
    return QRectF(QPointF(x0,y0),QPointF(x1,y1));
}

QPainterPath SignalSegment::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(QPointF(x0,y0),QPointF(x1,y1)));
    return path;
}

void SignalSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    painter->drawLine(QPointF(x0,y0),QPointF(x1,y1));
    return;
}

void SignalSegment::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
