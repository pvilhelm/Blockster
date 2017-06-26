#ifndef PORT_ITEM_H
#define PORT_ITEM_H

#include <QObject>
#include <QGraphicsItem>

class InportItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    InportItem(QGraphicsItem* parent, int local_port_nr);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double h = 10.;
    double w = 5.;
    int local_port_nr;

signals:
    void nodeMoved();
};

class OutportItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    OutportItem(QGraphicsItem* parent, int local_port_nr);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double h = 10.;
    double w = 5.;
    int local_port_nr;

signals:
    void nodeMoved();
};


#endif // PORT_ITEM_H

