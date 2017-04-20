#ifndef SIGNALLINE_H
#define SIGNALLINE_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

class SignalLine : public QGraphicsItem
{
public:
    SignalLine(QPointF start);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


    QColor color = Qt::black;
    QVector<SignalLine> vec_signalnodes;


};

#endif // SIGNALLINE_H
