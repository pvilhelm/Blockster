#include "signalline.h"

SignalSegment::SignalSegment(QGraphicsItem* parent, float x0, float y0, float x1, float y1)
{
    setParentItem(parent);
    SignalLine* sl = (SignalLine*)parent;
    start = QPointF(x0,y0)-sl->scenePos();
    end = QPointF(x1,y1)-sl->scenePos();
}

QRectF SignalSegment::boundingRect() const
{
    float dx = std::abs(end.x()-start.x());
    float dy = std::abs(end.y()-start.y());
    float x0 = std::min(start.x(),end.x());
    float y0 = std::min(start.y(),end.y());

    return QRectF(x0,y0,dx+2,dy+2);

}

QPainterPath SignalSegment::shape() const
{
    float dx = std::abs(end.x()-start.x());
    float dy = std::abs(end.y()-start.y());
    float x0 = std::min(start.x(),end.x());
    float y0 = std::min(start.y(),end.y());

    QPainterPath path;
    path.addRect(QRectF(x0,y0,dx+2,dy+2));
    return path;
}

void SignalSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    painter->drawLine(start,end);
    return;
}

void SignalSegment::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}
