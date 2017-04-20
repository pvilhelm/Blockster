#include "signalline.h"

SignalLine::SignalLine(QGraphicsItem* parent, float x0, float y0, float x1, float y1) : x0(x0),y0(y0),x1(x1), y1(y1)
{
    setParentItem(parent);
}

QRectF SignalLine::boundingRect() const
{
    return QRectF(QPointF(x0,y0),QPointF(x1,y1));
}

void SignalLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}
