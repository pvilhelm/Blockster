#include "inport.h"
#include "programscene.h"

Inport::Inport(int x, int y) : x(x), y(y)
{
    setFlags(ItemIsSelectable);
}

QRectF Inport::boundingRect() const
{
    return QRectF(x-12, y-12, 24, 24);
}

QPainterPath Inport::shape() const
{
    QPainterPath path;
    path.addRect(x-12, y-12, 24, 24);
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
    painter->drawRect(x-12, y-12, 24, 24);

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
            sl->addSegment(ss->end,scenePos+QPointF(x,y),SegmentType::END_SEGMENT);


            this->inSignalLine = sl;
        }
    }
    else if(prg_scene->mode == ProgramScene::None){
        return;
    }
}
