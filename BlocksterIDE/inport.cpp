#include "inport.h"
#include "programscene.h"

Inport::Inport(float x, float y) : x(x), y(y)
{
    setFlags(ItemIsSelectable);
    setPos(x,y);
}

QRectF Inport::boundingRect() const
{
    return QRectF(-12, -12, 24, 24);
}

QPainterPath Inport::shape() const
{
    QPainterPath path;
    path.addRect(-12, -12, 24, 24);
    return path;
}

void Inport::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QPen pen;
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QLine(-5,-5,0,0));
    painter->drawLine(QLine(0,0,-5,+5));
    painter->drawRect( -12, -12, 24, 24);

    return;
}

void Inport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ProgramScene* prg_scene = (ProgramScene*) this->scene();
    QPointF scenePos = this->scenePos();

    if(prg_scene->mode == ProgramScene::InsertLine){
        if(event->button() == Qt::LeftButton){
            prg_scene->mode = ProgramScene::None;

            SignalSegment* ss = prg_scene->lastSignalSegment;
            if(!ss)
                throw std::runtime_error("ss is null");

            prg_scene->lastSignalSegment = 0;

            SignalLine* sl = (SignalLine*)ss->parentItem();
            sl->addSegment( ss->end, scenePos, SegmentType::END_SEGMENT);

            this->inSignalLine = sl;
        }
    }
    else if(prg_scene->mode == ProgramScene::None){
        return;
    }
}
