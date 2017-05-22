#ifndef INPORT_H
#define INPORT_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalline.h"

class Inport : public QGraphicsItem
{
public:
    Inport(int x,int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    SignalLine* inSignalLine = 0;
    QColor color = Qt::black;
    int x = 0;
    int y = 0;
};

#endif // INPORT_H
