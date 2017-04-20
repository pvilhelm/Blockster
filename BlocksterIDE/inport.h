#ifndef INPORT_H
#define INPORT_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>

class Inport : public QGraphicsItem
{
public:
    Inport(int x,int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


    QColor color = Qt::black;
    int x = 0;
    int y = 0;
};

#endif // INPORT_H
