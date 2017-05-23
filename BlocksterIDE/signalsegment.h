#ifndef SIGNALNODE_H
#define SIGNALNODE_H


#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalline.h"

enum SegmentType {
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

    SegmentType type;



    QPointF start = QPointF(0,0); //In SignalLine coordinates
    QPointF end = QPointF(0,0);

};

#endif // SIGNALNODE_H
