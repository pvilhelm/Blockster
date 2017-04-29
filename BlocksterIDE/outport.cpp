#include "outport.h"

#include "programscene.h"
#include "signalline.h"

Outport::Outport(int x, int y) : x(x), y(y)
{
    setFlags(ItemIsSelectable);
}

QRectF Outport::boundingRect() const
{
    return QRectF(x, y-12, 14, 24);
}

QPainterPath Outport::shape() const
{
    QPainterPath path;
    path.addRect(x, y-12, 14, 24);
    return path;
}

void Outport::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QPen pen;
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QLine(x+5,y+5,x,y));
    painter->drawLine(QLine(x,y,x+5,y-5));

    return;
}

void Outport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ProgramScene* prg_scene = (ProgramScene*) this->scene();
    prg_scene->mode = ProgramScene::Mode::InsertLine;
    SignalLine* sig_line = new SignalLine(event->scenePos());
    this->scene()->addItem(sig_line);

}


