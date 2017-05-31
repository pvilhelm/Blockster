#include "outport.h"

Outport::Outport(float x, float y) : x(x), y(y)
{
    setFlags(ItemIsSelectable);
    setPos(x,y);
}

QRectF Outport::boundingRect() const
{
    return QRectF(0, -12, 14, 24);
}

QPainterPath Outport::shape() const
{
    QPainterPath path;
    path.addRect(0,-12, 14, 24);
    return path;
}

void Outport::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QPen pen;
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QLine(+5,+5,0,0));
    painter->drawLine(QLine(0,0,+5,-5));

    return;
}

void Outport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ProgramScene* prg_scene = (ProgramScene*) this->scene();
    if(prg_scene->mode == ProgramScene::InsertLine){
        return;
    }
    else if(prg_scene->mode == ProgramScene::None){
        if(event->button() == Qt::LeftButton){
            //dont add another output signal line if one exists
            if(this->outSignalLine)
                return;
            //add a SignalLine to the outport
            prg_scene->mode = ProgramScene::InsertLine; //change scene mode to insert lines
            SignalLine* sig_line = new SignalLine(this->scenePos(),this);
            this->scene()->addItem(sig_line);
            this->outSignalLine = sig_line;
            prg_scene->lastSignalSegment = sig_line->vec_signalnodes.last();
        }
    }

}


