#ifndef OUTPORT_H
#define OUTPORT_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalline.h"

class Outport : public QGraphicsItem
{
public:
    Outport(int x,int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    SignalLine* outSignalLine = 0;
    QColor color = Qt::black;
    int x = 0;
    int y = 0;
};

#endif // OUTPORT_H
