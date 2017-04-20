#ifndef SIGNALNODE_H
#define SIGNALNODE_H


#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>


class SignalSegment : public QGraphicsItem
{
public:
    SignalSegment(QGraphicsItem* parent, float x0, float y0, float x1, float y1);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    float x0;
    float y0;
    float x1;
    float y1;
};

#endif // SIGNALNODE_H
