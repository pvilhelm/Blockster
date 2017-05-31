#ifndef SIGNALNODE_H
#define SIGNALNODE_H


#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

//#include "inport.h"
//#include "outport.h"

class Outport;
class Inport;

enum SegmentType {
    INVALID_TYPE = 0,
    START_SEGMENT = 1,
    END_SEGMENT = 2,
    STARTEND_SEGMENT = 3,
    MIDDLE_SEGMENT = 4
};

class SignalSegment : public QGraphicsItem
{
public:
    SignalSegment(QGraphicsItem* parent, float x0, float y0, float x1, float y1);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void changePos(QPointF newStart,QPointF newEnd);
    void changePos(QPointF newStart);

    SegmentType type = INVALID_TYPE;

    Inport* end_port = 0;
    Outport* start_port = 0;

    QPointF start = QPointF(0,0); //In SignalLine coordinates
    QPointF end = QPointF(0,0);

};

#endif // SIGNALNODE_H
