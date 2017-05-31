#ifndef OUTPORT_H
#define OUTPORT_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

#include "signalline.h"
#include "programscene.h"

class Outport : public QGraphicsItem
{
public:
    Outport(float x,float y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QString signal_type = "";
    QString port_id = "";

    QList<QString> valid_signal_types;

    SignalLine* outSignalLine = 0;
    QColor color = Qt::black;
    float x = 0;
    float y = 0;


};

#endif // OUTPORT_H
