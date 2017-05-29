#ifndef INPORT_H
#define INPORT_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalline.h"

class Inport : public QGraphicsItem
{
public:
    Inport(float x,float y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    SignalLine* inSignalLine = 0;
    QColor color = Qt::black;
    float x = 0;
    float y = 0;

    QString port_id = "";
    QString signal_type = "";

    QList<QString> valid_signal_types;
};

#endif // INPORT_H
