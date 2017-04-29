#ifndef SIGNALLINE_H
#define SIGNALLINE_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalsegment.h"

class SignalLine : public QGraphicsItem
{
public:
    SignalLine(QPointF start);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QColor color = Qt::black;
    QVector<SignalSegment*> vec_signalnodes;


};

#endif // SIGNALLINE_H
