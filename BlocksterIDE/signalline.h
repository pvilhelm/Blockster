#ifndef SIGNALLINE_H
#define SIGNALLINE_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "inport.h"
#include "outport.h"
#include "signalsegment.h"

class SignalSegment;

class SignalLine : public QGraphicsItem
{
public:
    SignalLine(QPointF start, Outport* op);
    virtual ~SignalLine();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void addSegment(QPointF startPoint, QPointF endPoint, SegmentType type);
    void removeSegment(SignalSegment * ss);
    void addEndSegment(QPointF startPoint, QPointF endPoint, Inport* ip);
    void addStartSegment(QPointF startPoint, QPointF endPoint, Outport* op);
    QString getSourcePortType();
    QString getSourcePortBlocksId();
    QString getSourcePortId();

    QColor color = Qt::black;
    QVector<SignalSegment*> vec_signalnodes;

    Outport* start_port = 0;
    QList<Inport*> end_port_list;

};

#endif // SIGNALLINE_H
