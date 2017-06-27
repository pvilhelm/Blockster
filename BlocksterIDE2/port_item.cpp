#include "port_item.h"

#include <QPainter>

InportItem::InportItem(QGraphicsItem* parent, int local_port_nr) : QGraphicsItem(parent), local_port_nr(local_port_nr)
{

}

QRectF InportItem::boundingRect() const
{
    return QRectF(-w,-h/2,w,h);
}

void InportItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    path.moveTo({-w,-h/2.});
    path.lineTo({0.,0.});
    path.lineTo({-w,h/2.});
    QPen pen;
    pen.setWidthF(2.);
    painter->setPen(pen);
    painter->drawPath(path);
}

OutportItem::OutportItem(QGraphicsItem *parent, int local_port_nr) : QGraphicsItem(parent), local_port_nr(local_port_nr)
{

}

QRectF OutportItem::boundingRect() const
{
    return QRectF(0,-h/2,w,h);
}

void OutportItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPainterPath path;
    path.moveTo({0,-h/2.});
    path.lineTo({w,0.});
    path.lineTo({0,h/2.});
    QPen pen;
    pen.setWidthF(2.);
    painter->setPen(pen);
    painter->drawPath(path);

}
