#include "signalline.h"

SignalSegment::SignalSegment(QGraphicsItem* parent, float x0, float y0, float x1, float y1)
{
    setParentItem(parent);
    SignalLine* sl = (SignalLine*)parent;
    setPos(QPointF(x0,y0)-sl->scenePos());
    start = QPointF(0,0);
    end = QPointF(x1,y1)-sl->scenePos()-QPointF(x0,y0);
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

void SignalSegment::changePos(QPointF newStart, QPointF newEnd)
{
    this->prepareGeometryChange();
    start = QPointF(0,0);
    SignalLine* sl = (SignalLine*)this->parentItem();
    end = newEnd-sl->scenePos()-newStart;
    setPos(newStart-sl->scenePos());
    update();
}

void SignalSegment::changePos(QPointF newStart)
{
    SignalLine* sl = (SignalLine*)this->parentItem();
    this->changePos(newStart,end+this->scenePos());
}


